#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define LED_ADDRESS (0x06000000 + 0x5000)
#define ADDRESS_MAP_SIZE 0x1000

volatile unsigned short* led_base;

ssize_t led_write(struct file *pfile, char *buf, size_t count, loff_t *filePos){

	unsigned char rdata;
	unsigned int ret;

	ret = copy_from_user(&rdata, buf, 1);

	writew(rdata, led_base);

	return 1;
}



int led_release(struct inode *inode, struct file *pfile)
{
	if(led_base != NULL){
		iounmap(led_base);
		led_base = NULL;
		release_mem_region(LED_ADDRESS, ADDRESS_MAP_SIZE);
	}
	return 0;
}

MODULE_DESCRIPTION("System Programming practice 1, LED switch Driver");
MODULE_AUTHOR("Sysprog");
MODULE_LICENSE("GPL");
int led_open(struct inode *inode, struct file *pfile)
{
        if(check_mem_region(LED_ADDRESS, ADDRESS_MAP_SIZE)){
                printk("leddevice : memory already in use\n");
                return -EBUSY;
        }
        if(request_mem_region(LED_ADDRESS, ADDRESS_MAP_SIZE,"KEY")==NULL){
                printk("leddevice : fail to allocate mem region\n");
                return -EBUSY;
        }

        led_base = ioremap(LED_ADDRESS, ADDRESS_MAP_SIZE);
        if(led_base == NULL){
                printk("leddevice : fail to io mapping\n");
                release_mem_region(LED_ADDRESS, ADDRESS_MAP_SIZE);
                return -ENOMEM;
        }

        return 0;
}

struct file_operations led_fops = {
        .owner = THIS_MODULE,
        .open = led_open,
	.write = led_write,
        .release = led_release
};

struct miscdevice led_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "busled",
	.fops = &led_fops
};
 
	
static int __init led_init(void)
{
	int res;

	res = misc_register(&led_device);
	if(res){
		printk("fail to regiter the device\n");
		return res;
	}
	return 0;
}
static void __exit led_exit(void)
{
	misc_deregister(&led_device);
}
module_init(led_init)
module_exit(led_exit)

