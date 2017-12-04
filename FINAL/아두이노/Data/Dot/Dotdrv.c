#define DOT_1_ADDRESS (0x06000000 + 0x1000)
#define ADDRESS_MAP_SIZE 0x1000
#include<linux/miscdevice.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<asm/io.h>
#include<asm/uaccess.h>
#include<linux/init.h>
#include<linux/platform_device.h>

volatile unsigned short *Dot_base;
MODULE_DESCRIPTION("System Programming practice 1, dip switch Driver");
MODULE_AUTHOR("Sysprog");
MODULE_LICENSE("GPL");
int Dot_open(struct inode *inode, struct file *pfile){
	if(check_mem_region(DOT_1_ADDRESS, ADDRESS_MAP_SIZE)){
		printk("dispwdrv : memory already in use \n");
		return -EBUSY;
	}
	if(request_mem_region(DOT_1_ADDRESS, ADDRESS_MAP_SIZE, "DIPSW") == NULL){
		printk("Dotdrv : fail to allocate mem region\n");
		return -EBUSY;
	}

	Dot_base = ioremap(DOT_1_ADDRESS, ADDRESS_MAP_SIZE);
	if(Dot_base == NULL){
		printk("Dotdrv : fail to io mapping\n");
		release_mem_region(DOT_1_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	return 0;
}
ssize_t Dot_write(struct file *pfile, char *buf, size_t count, loff_t *filePos){
	unsigned short rdata[2];
	unsigned int ret;
	ret = copy_from_user(&rdata, buf, 4);
	writew(rdata[0], Dot_base);
	writew(rdata[1], (int)Dot_base+2);
	return 2;
}

int Dot_release(struct inode *inode, struct file *pfile)
{
	if(Dot_base != NULL){
		iounmap(Dot_base);
		Dot_base = NULL;
		release_mem_region(DOT_1_ADDRESS, ADDRESS_MAP_SIZE);
	}
	return 0;
}
struct file_operations Dot_fops = {
        .owner = THIS_MODULE,
        .open = Dot_open,
	.write = Dot_write,
	.release = Dot_release
};

struct miscdevice Dot_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "Dot",
	.fops = &Dot_fops,
};
static int __init Dot_init(void){
	int res;

	res = misc_register(&Dot_device);
	if(res){
		printk("fail to register the device\n");
		return res;
	}
	return 0;
}

static void __exit Dot_exit(void){
	misc_deregister(&Dot_device);
}

module_init(Dot_init)
module_exit(Dot_exit)
