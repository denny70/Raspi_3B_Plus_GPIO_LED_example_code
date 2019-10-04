#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#include "raspi_led.h"

//#include <linux/err.h>

static unsigned int __iomem *gpio_base;
#define BCM2835_GPIO_BASE 0x3f200000
#define BCM2835_GPIO_SET_OFFSET (0x1C/4)
#define BCM2835_GPIO_CLR_OFFSET (0x28/4)
#define GPIO_CTL(g) (*(gpio_base + ((g)/10)))
#define GPIO_SET(g) (*(gpio_base + BCM2835_GPIO_SET_OFFSET + ((g)/32)))
#define GPIO_CLR(g) (*(gpio_base + BCM2835_GPIO_CLR_OFFSET + ((g)/32)))

#define GPIO_INPUT(g) ((*(gpio_base + ((g)/10))) &= ~(0x7 << (((g)%10)*3)))
#define GPIO_CLEAN(g) ((*(gpio_base + ((g)/10))) &= ~(0x7 << (((g)%10)*3)))
#define GPIO_OUTPUT(g) ((*(gpio_base + ((g)/10))) |= (0x1 << (((g)%10)*3)))
#define GPIO_HIGH(g) ((*(gpio_base + BCM2835_GPIO_SET_OFFSET + ((g)/32))) |= (0x1 << ((g)%32)))
#define GPIO_LOW(g) ((*(gpio_base + BCM2835_GPIO_CLR_OFFSET + ((g)/32))) |= (0x1 << ((g)%32)))

//#define LED_MAJOR 256
//#define LED_MINOR 0
#define LED_DEV_CNT 1
#define LED_DEV_NAME "led_device"

static struct LED_DEVICE {
    dev_t devno;
    bool led_flag;
    struct class *LED_class;
    struct device *LED_dev;
    struct cdev cdev;
} LED_device;

static int LED_open(struct inode *inode, struct file *filp){
    filp->private_data = container_of(inode->i_cdev, struct LED_DEVICE, cdev);
    return 0;
}
static int LED_release(struct inode *indoe, struct file *filp){
    return 0;
}
static ssize_t LED_read(struct file *filp, char __user *buf, size_t len, loff_t *pos){

}
static ssize_t LED_write(struct file *filp, const char *buf, size_t len, loff_t *pos){
    int copied = 0;
    struct Parameter parameter;
    copied = copy_from_user((void *)&parameter, buf, len);
    

    if (parameter.on_off == 0){
        GPIO_LOW(parameter.GPIO_port);
    }else{
        GPIO_HIGH(parameter.GPIO_port);
    }
    return copied;
}
static struct file_operations LED_ops={
    .owner = THIS_MODULE,
    .open = LED_open,
    .release = LED_release,
    .read = LED_read,
    .write = LED_write,

};

static int __init LED_init(void){
    int ret = -1;
    int i = 0;

    LED_device.led_flag=0;

    gpio_base = ioremap(BCM2835_GPIO_BASE, 0xA0);
    for (i=2; i<28; i++){
        GPIO_CLEAN(i);
        GPIO_OUTPUT(i);
        GPIO_HIGH(i);
    }

    if (IS_ERR(gpio_base)){
        printk(KERN_INFO "ioremap for GPIO_BASE error\n");
        goto reg_err;
    }

    //LED_device.devno = MKDEV(LED_MAJOR, LED_MINOR);
    //ret = register_chrdev_region(LED_device.devno, LED_DEV_CNT, LED_DEV_NAME);
    ret = alloc_chrdev_region(&LED_device.devno, 0,LED_DEV_CNT, LED_DEV_NAME);
    if (ret){
        printk(KERN_INFO "apply dev number fail\n");
        goto reg_err;
    }
    cdev_init(&LED_device.cdev, &LED_ops);
    LED_device.cdev.owner = THIS_MODULE;

    ret = cdev_add(&LED_device.cdev, LED_device.devno, LED_DEV_CNT);
    if (ret){
        printk(KERN_INFO "add cdev in Kernel fail\n");
        goto add_err;
    }

    LED_device.LED_class = class_create(THIS_MODULE, LED_DEV_NAME);
    if (IS_ERR(LED_device.LED_class)){
        printk(KERN_INFO "Class Create fail\n");
        goto class_err;
    }

    LED_device.LED_dev = device_create(LED_device.LED_class, NULL, LED_device.devno, NULL, LED_DEV_NAME);
    if (IS_ERR(LED_device.LED_dev)){
        printk(KERN_INFO "device create fail\n");
        goto dev_err;
    }

    return 0;
dev_err:
    class_destroy(LED_device.LED_class);
class_err:
    cdev_del(&LED_device.cdev);
add_err:
    unregister_chrdev_region(LED_device.devno, LED_DEV_CNT);
reg_err:
    return ret;
}
static void __exit LED_exit(void){
    int i=0;
    for (i=2; i<28; i++){
        GPIO_LOW(i);
    }
    device_destroy(LED_device.LED_class, LED_device.devno);
    class_destroy(LED_device.LED_class);
    cdev_del(&LED_device.cdev);
    unregister_chrdev_region(LED_device.devno, LED_DEV_CNT);
}

module_init(LED_init);
module_exit(LED_exit);

MODULE_AUTHOR("Denny Chen");
MODULE_LICENSE("GPL");

