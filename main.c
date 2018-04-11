#include "hook_func.h"
#include "parse.h"
#include "log.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <asm/uaccess.h> //copy_to_user()
#include <linux/fs.h>	//register_chrdev(), unregister_chrdev()

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHOI");
MODULE_DESCRIPTION("Anti-Ransomeware");

module_param(log_path, charp, 0);
MODULE_PARM_DESC(log_path, "A char string");

module_param(backup_path, charp, 0);
MODULE_PARM_DESC(log_change, "A char string");

static int __init syscall_init(void)
{
	find_sys_table();	
	
	hook_start();	

	if(log_open())
		printk(KERN_ALERT "[GUARD] Log open failed\n");

	if(reg_guard_chr(DEVICE_NUM, "RansomGuard"))
		printk(KERN_ALERT "Reg error\n");
	return 0;
}

static void __exit syscall_release(void)
{
	hook_end();

	unreg_guard_chr(DEVICE_NUM, "RansomGuard");
	log_close();
/*
	printk(KERN_INFO "PRO_DIR ===================\n");
	TTN_printAll(pro_dir_root);
	printk(KERN_INFO "PRO_DECOY =================\n");
	TTN_printAll(pro_decoy_root);
	printk(KERN_INFO "PRO_EXTENTION==============\n");
	TTN_printAll(pro_extentions_root);
	printk("LOG_PATH : %s[]\n", log_path);
	printk("BACKUP_PATH : %s[]\n", backup_path);
*/
	TTN_deleteAll(&pro_dir_root);
	TTN_deleteAll(&pro_decoy_root);
	TTN_deleteAll(&pro_extentions_root);
}

module_init(syscall_init);
module_exit(syscall_release);
