#!/bin/sh
'''
A simple Shell script for LED on/off
will pull high on GPIO BCM pin one by one from 2->3->4->5->6->7->8->9
then
will pull  low on GPIO BCM pin one by one from 2->3->4->5->6->7->8->9

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
'''

GPIO_BCM_PIN="2 3 4 5 6 7 8 9"
while [ 1 ]; do
    for pin in $GPIO_BCM_PIN; do
        sudo ./raspi_led_test -w $pin 1
        sleep 0.1
    done
    for pin in $GPIO_BCM_PIN; do
        sudo ./raspi_led_test -w $pin 0
        sleep 0.1
    done
done

