#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x37a0cba, "kfree" },
	{ 0x62d33f31, "module_put" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xa0378bea, "try_module_get" },
	{ 0x4ca346e5, "kmalloc_caches" },
	{ 0xeaa1fdc0, "kmalloc_trace" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x92997ed8, "_printk" },
	{ 0x3a099605, "__get_user_nocheck_4" },
	{ 0x8d6aff89, "__put_user_nocheck_4" },
	{ 0xc3aaf0a9, "__put_user_1" },
	{ 0xed830d9b, "__register_chrdev" },
	{ 0x30e967fb, "__class_create" },
	{ 0xe775b64e, "device_create" },
	{ 0x48618e0, "device_destroy" },
	{ 0xdd1b3227, "class_destroy" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xb83992f2, "module_layout" },
};

MODULE_INFO(depends, "");

