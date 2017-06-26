#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x683cfe8d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x764b0a91, __VMLINUX_SYMBOL_STR(d_path) },
	{ 0x7875fbf2, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xfff5afc, __VMLINUX_SYMBOL_STR(time64_to_tm) },
	{ 0x50f0a941, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x900ff7b6, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x50c045ea, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0x98ef2dca, __VMLINUX_SYMBOL_STR(vfs_read) },
	{ 0x76c7c26a, __VMLINUX_SYMBOL_STR(kern_path) },
	{ 0x2270cd45, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x672c5471, __VMLINUX_SYMBOL_STR(pv_cpu_ops) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xccca000a, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x4f68e5c9, __VMLINUX_SYMBOL_STR(do_gettimeofday) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xaeab1f68, __VMLINUX_SYMBOL_STR(send_sig_info) },
	{ 0xe0c657fb, __VMLINUX_SYMBOL_STR(vfs_write) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
	{ 0xdcb0349b, __VMLINUX_SYMBOL_STR(sys_close) },
	{ 0xf7ef5088, __VMLINUX_SYMBOL_STR(filp_open) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B4B24F2184343EE545F9E4B");
