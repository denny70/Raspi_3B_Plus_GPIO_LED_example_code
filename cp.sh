#!/bin/sh

scp -r denny70@10.242.232.36:/home/denny70/Nextcloud/10_test/linux_driver/raspi_led /home/pi/
echo "cp file done"
gcc raspi_led_test.c
echo "gcc"
make
echo "make module"
ret=$(cat /proc/modules | grep raspi_led)
if [ -n "$ret" ]; then
    echo "rmmod module"
    sudo rmmod raspi_led
    echo "rmmod ===============================" > /dev/kmsg
fi

sudo insmod raspi_led.ko
echo "insmod module"
echo "insmod ===============================" > /dev/kmsg
echo "======================================" > /dev/kmsg
dmesg

