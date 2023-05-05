#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kstrtox.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/irq.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "chardev"


/* Используем 'k' как системный номер */
#define SCULL_IOC_MAGIC 'k'
/* Пожалуйста, используйте в вашем коде другое 8-ми битовое число */
#define SCULL_IOCRESET    _IO(SCULL_IOC_MAGIC, 0)
/*
* S означает "Set" ("Установить") через ptr,
* T означает "Tell" ("Сообщить") прямо с помощью значения аргумента
* G означает "Get" ("Получить"): ответ устанавливается через указатель
* Q означает "Query" ("Запрос"): ответом является возвращаемое значение
* X означает "eXchange" ("Обменять"): переключать G и S автоматически
* H означает "sHift" ("Переключить"): переключать T и Q автоматически
*/
#define SCULL_IOCSQUANTUM _IOW(SCULL_IOC_MAGIC, 1, int)
#define SCULL_IOCSQSET    _IOW(SCULL_IOC_MAGIC, 2, int)
#define SCULL_IOCTQUANTUM _IO(SCULL_IOC_MAGIC, 3)
#define SCULL_IOCTQSET    _IO(SCULL_IOC_MAGIC, 4)
#define SCULL_IOCGQUANTUM _IOR(SCULL_IOC_MAGIC, 5, int)
#define SCULL_IOCGQSET    _IOR(SCULL_IOC_MAGIC, 6, int)
#define SCULL_IOCQQUANTUM _IO(SCULL_IOC_MAGIC, 7)
#define SCULL_IOCQQSET    _IO(SCULL_IOC_MAGIC, 8)
#define SCULL_IOCXQUANTUM _IOWR(SCULL_IOC_MAGIC, 9, int)
#define SCULL_IOCXQSET    _IOWR(SCULL_IOC_MAGIC,10, int)
#define SCULL_IOCHQUANTUM _IO(SCULL_IOC_MAGIC, 11)
#define SCULL_IOCHQSET    _IO(SCULL_IOC_MAGIC, 12)
#define SCULL_IOC_MAXNR 14




static int sensor_value = 0;
static DEFINE_MUTEX(sensor_value_mtx);

static int major;
static struct class *cls;

typedef struct ChardevPrivateData {
    char buff[32];
    int cnt;
} ChardevPrivateData;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *,
                           size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *,
                            size_t, loff_t *);
static long int my_ioctl (struct file *, unsigned int, unsigned long);

static struct file_operations chardev_fops = {
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = my_ioctl,
    .open = device_open,
    .release = device_release,
};

int init_module(void) {
    struct device* dev;
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops);

    if(major < 0) {
        pr_alert("register_chrdev() failed: %d\n", major);
        return -EINVAL;
    }

    pr_info("major = %d\n", major);

    cls = class_create(THIS_MODULE, DEVICE_NAME);
    if(IS_ERR(cls)) {
        pr_alert("class_create() failed: %ld\n", PTR_ERR(cls));
        return -EINVAL;
    }

    dev = device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if(IS_ERR(dev)) {
        pr_alert("device_create() failed: %ld\n", PTR_ERR(dev));
        return -EINVAL;
    }

    pr_info("/dev/%s created\n", DEVICE_NAME);
    return 0;
}

void cleanup_module(void) {
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
    unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file) {
    ChardevPrivateData* pd;
    int val;

    if(!try_module_get(THIS_MODULE)) {
        pr_alert("try_module_get() failed\n");
        return -EINVAL;
    }

    pd = kmalloc(sizeof(ChardevPrivateData), GFP_KERNEL);
    if(pd == NULL) {
        pr_alert("kmalloc() failed\n");
        module_put(THIS_MODULE);
        return -EINVAL;
    }

    mutex_lock(&sensor_value_mtx);
    val = sensor_value;
    sensor_value++;
    mutex_unlock(&sensor_value_mtx);

    sprintf(pd->buff, "Dummy sensor value: %d\n", val);
    pd->cnt = 0;
    file->private_data = pd;
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    kfree(file->private_data);
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *file,
                           char __user *buffer,
                           size_t length,
                           loff_t *offset) {
    int bytes_read = 0;
    ChardevPrivateData* pd = file->private_data;

    while(length && (pd->buff[pd->cnt] != '\0')) {
        if(put_user(pd->buff[pd->cnt], buffer++) != 0)
            return -EINVAL;
        pd->cnt++;
        bytes_read++;
        length--;
    }

    return bytes_read;
}

static ssize_t device_write(struct file *file,
                            const char __user *buff,
                            size_t size,
                            loff_t *offset) {

    mutex_lock(&sensor_value_mtx);
    sensor_value = 0;
    mutex_unlock(&sensor_value_mtx);

    return size;
}



static long int my_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
    int err = 0;
    int retval = 0;

    if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
    if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;

    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));

    if (err) return -EFAULT;

    switch(cmd) {
        case SCULL_IOCRESET:
            sensor_value = 0;
        break;

        case SCULL_IOCSQUANTUM: /* Установить: arg указывает на значение */
            retval = __get_user(sensor_value, (int __user *)arg);
        break;

        case SCULL_IOCTQUANTUM: /* Сообщить: arg является значением */
            sensor_value = arg * 10;
        break;

        case SCULL_IOCGQUANTUM: /* Получить: arg является указателем на результат */
            retval = __put_user(sensor_value, (int __user *)arg);
        break;
    }
    return 0;
}












MODULE_LICENSE("GPL");








