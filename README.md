# Raspi_3bPlus_GPIO_LED_example_code
To want to learn how to write Linux driver, this will be a reference 

# Usage:
1) use root user                            : su
2) enter project directory                  : cd raspi_led
3) compile and load module/user space app   : ./build.sh
    2-1) gcc raspi_led_test.c -o raspi_led_test (gcc for raspi test file)
        - raspi_led_test
    2-2) make (reapi led kernel module)
        - raspi_led.ko
    2-3) remove raspi_led module if you had loaded
    2-4) load raspi led module
        - sudo insmod raspi_led.ko

4) Usage: 
   ./raspi_led_test -w <GPIO pin number(BCM cloumn)> <ON_OFF>
   ex: (pull high on GPIO BCM pin 2) ./raspi_led_test -w 2 1
   ex: (pull  low on GPIO BCM pin 3) ./raspi_led_test -w 3 0
5) Shell script usage            : ./respi_led_test.sh
   will pull high on GPIO BCM pin one by one from 2->3->4->5->6->7->8->9
   then
   will pull  low on GPIO BCM pin one by one from 2->3->4->5->6->7->8->9
6) There is a PyQt5 GUI interface to make GPIO on/off
   But you will need to insatll PyQt5 package
   (8-1) Ubuntu:sudo apt-get install python3-pyqt5 or
   (8-2) to install by pip/pip3 : pip install PyQt5
   Usage: to launch GUI APP.
   ./raspi_led_GUI/python3  pyqt.py

Image is came from "https://www.raspberrypi.org/documentation/usage/gpio/images/gpio-numbers-pi2.png"
![Image](http://https://www.raspberrypi.org/documentation/usage/gpio/images/gpio-numbers-pi2.png)




