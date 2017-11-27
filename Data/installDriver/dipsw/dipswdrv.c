#include<linux/miscdevice.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/platform_device.h>
#include<asm/io.h>
#include<asm/uaccess.h>

#define DIP_SW_ADDRESS		(0x06000000 + 0x2000)
#define ADDRESS_MAP_SIZE	0x1000

volatile unsigned short *dipsw_base;

MODULE_DESCRIPTION("SYSTEM PROGRAMMING PRACTICE 1, DIP SWITCH DRIVER");
MODULE_AUTHOR("Sysprog");
MODULE_LICENSE("GPL");

int dipsw_open(struct inode *inode, struct file *pfile) {
	if(check_mem_region(DIP_SW_ADDRESS, ADDRESS_MAP_SIZE)){
		printk("dipswdrv : memory already in use\n");
		return -EBUSY;
	}
	if(request_mem_region(DIP_SW_ADDRESS, ADDRESS_MAP_SIZE, "DIPSW") == NULL){
		printk("dipsw : fail to allocate mem region\n");
		return -EBUSY;
	}
	
	dipsw_base = ioremap(DIP_SW_ADDRESS, ADDRESS_MAP_SIZE);
	if(dipsw_base == NULL){
		printk("dipswdrv : fail to io mapping\n");
		release_mem_region(DIP_SW_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	return 0;
}
ssize_t dipsw_read(struct file *pfile, char *buf, size_t count, loff_t *filePos) {
	unsigned short rdata;
	unsigned int ret;
	rdata = readw(dipsw_base);
	ret = copy_to_user(buf, &rdata, 2);
	return 2;
}
int dipsw_release(struct inode *inode, struct file *pfile) {
	if(dipsw_base != NULL) {
		iounmap(dipsw_base);
		dipsw_base = NULL;
		release_mem_region(DIP_SW_ADDRESS, ADDRESS_MAP_SIZE);
	}
	return 0;
}

struct file_operations dipsw_fops = {
	.owner = THIS_MODULE,
	.open = dipsw_open,
	.read = dipsw_read,
	.release = dipsw_release
};
struct miscdevice dipsw_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dipsw",
	.fops = &dipsw_fops	
};

static int __init dipsw_init(void){
	int res;
	res = misc_register(&dipsw_device);
	if(res){
		printk("fail to register the device");
		return res;
	}
	return 0;
}

static void __exit dipsw_exit(void){
	misc_deregister(&dipsw_device);
}

module_init(dipsw_init)
module_exit(dipsw_exit)
