#!/bin/sh

gcc raspi_led_test.c -o raspi_led_test
echo "gcc"

make
echo "make module"

ret=$(cat /proc/modules | grep raspi_led)
if [ -n "$ret" ]; then
    echo "rmmod raspi_led module"
    sudo rmmod raspi_led
    echo "rmmod raspi_led ===============================" > /dev/kmsg
fi

sudo insmod raspi_led.ko
echo "insmod module"
echo "insmod raspi_led ===============================" > /dev/kmsg
echo "======================================" > /dev/kmsg

