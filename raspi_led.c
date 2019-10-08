/*
*  Raspi 3B+, (GPIO) LED Driver
*
*  Usage:
*       (0) use root user                 : su
*       (1) enter project directory       : cd raspi_led
*       (2) compile kernel module         : make
*       (3) Load driver                   : insmod ./raspi_led.ko
*       (4) remove driver                 : rmmod ./raspi_led
*       (5) compile user space application: gcc raspi_led_test.c -o raspi_led_test
*       (6) Usage: 
            ./raspi_led_test -w <GPIO pin number(BCM cloumn)> <ON_OFF>
*           ex: (pull high on GPIO BCM pin 2) ./raspi_led_test -w 2 1
            ex: (pull  low on GPIO BCM pin 3) ./raspi_led_test -w 3 0
        (7) Shell script usage            : ./respi_led_test.sh
            will pull high on GPIO BCM pin one by one from 2->3->4->5->6->7->8->9
            then
            will pull  low on GPIO BCM pin one by one from 2->3->4->5->6->7->8->9
        (8) There is PyQt5 GUI interface to make GPIO on/off
            But you will need to insatll PyQt5 package
            (8-1) Ubuntu:sudo apt-get install python3-pyqt5 or
            (8-2) to install by pip/pip3 : pip install PyQt5
            Usage: to launch GUI APP.
            ./raspi_led_GUI/python3  pyqt.py
            
            
--(GPIO BCM Pin number)----the Table source is from WiringPi project
 +-----+-----+---------+------+---+---Pi 3+--+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 |  OUT | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 |  OUT | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |  OUT | 1 |  7 || 8  | 1 | ALT5 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT5 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |  OUT | 1 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK | ALT0 | 0 | 23 || 24 | 1 | OUT  | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | OUT  | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |  OUT | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |  OUT | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 3+--+---+------+---------+-----+-----+
*
*  
*  Copyright (C) 2019 Denny chen <denny70@gmail.com>
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#include "raspi_led.h"

static volatile uint32_t __iomem *gpio_base;
#define BCM2837_GPIO_BASE 0x3F200000    //for Rasppi 3B+
#define BCM2837_GPIO_SET_OFFSET (0x1C/4)
#define BCM2837_GPIO_CLR_OFFSET (0x28/4)

#define GPIO_CTL(g) (*(gpio_base + ((g)/10)))
#define GPIO_SET(g) (*(gpio_base + BCM2837_GPIO_SET_OFFSET + ((g)/32)))
#define GPIO_CLR(g) (*(gpio_base + BCM2837_GPIO_CLR_OFFSET + ((g)/32)))

// GPIO_INPUT and GPIO_CLEAN are the same function, please check next next line 
#define GPIO_INPUT(g) ((*(gpio_base + ((g)/10))) &= ~(0x7 << (((g)%10)*3)))
#define GPIO_CLEAN(g) ((*(gpio_base + ((g)/10))) &= ~(0x7 << (((g)%10)*3)))
// If want to set GPIO as output, will need (1) GPIO_CLEAN(g) then (2) GPIO_OUTPUT(g)
// because set register to "000 first then 001"
#define GPIO_OUTPUT(g) ((*(gpio_base + ((g)/10))) |= (0x1 << (((g)%10)*3)))

//to pull high on GPIO pin(g)
#define GPIO_HIGH(g) ((*(gpio_base + BCM2837_GPIO_SET_OFFSET + ((g)/32))) = (0x1 << ((g)%32)))
//to pull low on GPIO pin(g)
#define GPIO_LOW(g) ((*(gpio_base + BCM2837_GPIO_CLR_OFFSET + ((g)/32))) = (0x1 << ((g)%32)))

//if to use register_chrdev_region() function, you will need to unmark below 2 lines
//#define LED_MAJOR 256
//#define LED_MINOR 0
#define LED_DEV_CNT 1
#define LED_DEV_NAME "led_device"

static struct LED_DEVICE {
    dev_t devno;
    struct class *LED_class;
    struct device *LED_dev;
    struct cdev cdev;
} LED_device;

static int __init LED_init(void);
static int LED_open(struct inode *inode, struct file *filp);
static int LED_release(struct inode *indoe, struct file *filp);
static ssize_t LED_read(struct file *filp, char __user *buf, size_t len, loff_t *pos);
static ssize_t LED_write(struct file *filp, const char *buf, size_t len, loff_t *pos);

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

    //transfer GPIO memory address -> Kernel virtual address
    gpio_base = ioremap(BCM2837_GPIO_BASE, 0xA0);
    if (!gpio_base){
        printk(KERN_INFO "ioremap for GPIO_BASE error\n");
        goto iomem_err;
    }

    for (i=2; i<10; i++){
        GPIO_CLEAN(i);  //set register to 0b000 for GPIO BCM pin
        GPIO_OUTPUT(i); //set register to 0b001 for GPIO BCM pin
        GPIO_HIGH(i);   //set register to 0b1 for GPIO BCM pin
    }

    // register device number in kernel by fixed number which is defined
    /*
    LED_device.devno = MKDEV(LED_MAJOR, LED_MINOR);
    ret = register_chrdev_region(LED_device.devno, LED_DEV_CNT, LED_DEV_NAME);
    */

    // kernel allocate device number to driver  
    ret = alloc_chrdev_region(&LED_device.devno, 0,LED_DEV_CNT, LED_DEV_NAME);
    if (ret){
        printk(KERN_INFO "apply dev number fail\n");
        goto reg_err;
    }

    // connect 2 structs, cdev and file_operations 
    cdev_init(&LED_device.cdev, &LED_ops);
    LED_device.cdev.owner = THIS_MODULE;

    // to add cdev, cdevno into kernel
    ret = cdev_add(&LED_device.cdev, LED_device.devno, LED_DEV_CNT);
    if (ret){
        printk(KERN_INFO "add cdev in Kernel fail\n");
        goto add_err;
    }

    // create class
    LED_device.LED_class = class_create(THIS_MODULE, LED_DEV_NAME);
    if (IS_ERR(LED_device.LED_class)){
        printk(KERN_INFO "Class Create fail\n");
        goto class_err;
    }

    // create device, "class and device" are to be able to add device under "/dev/" automatically when load module
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
iomem_err:
    return -EBUSY;
}

static int LED_open(struct inode *inode, struct file *filp){
    filp->private_data = container_of(inode->i_cdev, struct LED_DEVICE, cdev);
    return 0;
}
static int LED_release(struct inode *indoe, struct file *filp){
    return 0;
}
static ssize_t LED_read(struct file *filp, char __user *buf, size_t len, loff_t *pos){
    unsigned long ret;
    RData rdata;

    rdata.GPIO_ctl_addr[0] =  (gpio_base + 0);  //GPIO Function Select 0
    rdata.GPIO_ctl_data[0] = *(gpio_base + 0);
    rdata.GPIO_ctl_addr[1] =  (gpio_base + 1);  //GPIO Function Select 1
    rdata.GPIO_ctl_data[1] = *(gpio_base + 1);
    rdata.GPIO_ctl_addr[2] =  (gpio_base + 2);  //GPIO Function Select 2
    rdata.GPIO_ctl_data[2] = *(gpio_base + 2);

    ret = copy_to_user(buf, (void *)&rdata, sizeof(rdata));
    if (ret){
        printk(KERN_INFO "copy_to_user do not finish\n");
        return -1;
    }
    return len;
}
static ssize_t LED_write(struct file *filp, const char *buf, size_t len, loff_t *pos){
    unsigned long ret;
    WData wdata;
    ret = copy_from_user((void *)&wdata, buf, len);
    if (ret){
        printk(KERN_INFO "copy_from_user do not finish\n");
        return -1;
    }
    
    if (wdata.on_off == 0){
        GPIO_LOW(wdata.GPIO_port);
    }else{
        GPIO_HIGH(wdata.GPIO_port);
    }
    return len;
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

