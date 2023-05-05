#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm-generic/ioctl.h>

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


int main(void)
{
    int fileDescriptor;
    int32_t val, num;

    printf("\nOpening the Driver\n");
    fileDescriptor = open("/dev/chardev", O_RDWR);
    if(fileDescriptor < 0)
    {
        printf("Unable the Driver\n");
        return 0;
    }
    printf("File Descriptor %d\n", fileDescriptor);
    printf("Enter the value that you want to send \n");
    scanf("%d", &num);

    printf("Eriting the value to the driver \n");
    ioctl(fileDescriptor, SCULL_IOCTQUANTUM, num);

    printf("Reading the value from driver \n");
    ioctl(fileDescriptor, SCULL_IOCGQUANTUM, (int32_t*) &val);

    printf("The value is %d\n", val);

    printf("Closing the driver \n");
    close(fileDescriptor);
    return 0;

}







