/*
*  Raspi 3B+, (GPIO) LED 
*
*  Usage:
gcc raspi_led_test.c -o raspi_led_test (gcc for raspi test file)
- raspi_led_test                   
            
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <getopt.h>

#include "raspi_led.h"

WData wdata;

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Usage : raspi_gpio_test [-w <GPIO BCM Pin number> <on | off> | -r]\n");
        return -1;
    }
    int option;
    char flag ;
    while ((option=getopt(argc, argv, "wr"))!=-1){
        switch (option){
            case ('w'):
                flag = 'w';
                break;
            case ('r'):
                flag = 'r';
                break;
            default:
                printf("wrong option\n");
                break;
        }
    }
    
    int fd;
    ssize_t count;
    fd = open("/dev/led_device", O_RDWR);
    if (fd == -1){
        printf("open /dev/led_device fail\n");
        return -1;
    }
    if (flag == 'w'){
        if (argc !=4){
            printf("argv number is not correct\n");
            exit(-1);
        }
        wdata.GPIO_port = atoi(argv[2]);
        wdata.on_off = atoi(argv[3]);

        printf("GPIO: %d, on_off: %s\n",wdata.GPIO_port, wdata.on_off != 0 ? "On":"Off");

        count = write(fd, (void *) &wdata, sizeof(WData));
        if (count == -1){
            printf("write fail\n");
            exit(-1);
        }
        printf("write %ld bytes\n", count);

    }else if (flag == 'r'){
        if (argc !=2){
            printf("argv number is not correct\n");
            exit(-1);
        }
        RData rdata;
        count = read(fd, (void *) &rdata, sizeof(RData));
        if (count == -1){
            printf("read fail\n");
            exit(-1);
        }
        printf("read %ld bytes\n", count);
        int i;
        for (i=0; i< 3; i++){
            printf("GPIO_FSR_%d, %p, 0x%x\n", i, rdata.GPIO_ctl_addr[i], rdata.GPIO_ctl_data[i]);
        }
        
    }

    close(fd);
    return 0;
}
