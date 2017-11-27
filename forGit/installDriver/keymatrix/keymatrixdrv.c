#include<linux/miscdevice.h>
#include<linux/fs.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/platform_device.h>
#include<asm/io.h>
#include<asm/uaccess.h>

#define KEY_MATRIX_ADDRESS		(0x06000000 + 0x4000)
#define ADDRESS_MAP_SIZE	0x1000

volatile unsigned short *keymx_base;

MODULE_DESCRIPTION("SYSTEM PROGRAMMING PRACTICE 1, KEY MATRIX DRIVER");
MODULE_AUTHOR("Sysprog");
MODULE_LICENSE("GPL");

int keymx_open(struct inode *inode, struct file *pfile) {
	if(check_mem_region(KEY_MATRIX_ADDRESS, ADDRESS_MAP_SIZE)){
		printk("keymatrixdrv : memory already in use\n");
		return -EBUSY;
	}
	if(request_mem_region(KEY_MATRIX_ADDRESS, ADDRESS_MAP_SIZE, "KEYMX") == NULL){
		printk("keymx : fail to allocate mem region\n");
		return -EBUSY;
	}
	
	keymx_base = ioremap(KEY_MATRIX_ADDRESS, ADDRESS_MAP_SIZE);
	if(keymx_base == NULL){
		printk("keymxdrv : fail to io mapping\n");
		release_mem_region(KEY_MATRIX_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	return 0;
}
ssize_t keymx_read(struct file *pfile, char *buf, size_t count, loff_t *filePos) {
	unsigned short rdata;
	unsigned int ret;
	int key = 0;
	int i, j;
	for(i = 0; i < 4; i++) {
		writew(0x10 << i, keymx_base);
		rdata = readw(keymx_base)& 0xFF;
		if(rdata > 0) {
			for(j = 0; j < 4; j++) {
				if(rdata & (0x01<<j)) {
					key = i + 1 + j*4;
					break;
				}
			}
		}
		if(key) break;
	}
	ret = copy_to_user(buf, &key, 4);
	return 4;
}
int keymx_release(struct inode *inode, struct file *pfile) {
	if(keymx_base != NULL) {
		iounmap(keymx_base);
		keymx_base = NULL;
		release_mem_region(KEY_MATRIX_ADDRESS, ADDRESS_MAP_SIZE);
	}
	return 0;
}

struct file_operations keymx_fops = {
	.owner = THIS_MODULE,
	.open = keymx_open,
	.read = keymx_read,
	.release = keymx_release
};
struct miscdevice keymx_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "keymx",
	.fops = &keymx_fops	
};

static int __init keymx_init(void){
	int res;
	res = misc_register(&keymx_device);
	if(res){
		printk("fail to register the device");
		return res;
	}
	return 0;
}

static void __exit keymx_exit(void){
	misc_deregister(&keymx_device);
}

module_init(keymx_init)
module_exit(keymx_exit)
