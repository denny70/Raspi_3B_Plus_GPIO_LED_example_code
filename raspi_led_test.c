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
        printf("Usage : raspi_gpio_test [-w <GPIO number> <on | off> | -r]\n");
        return -1;
    }
    int option;
    char flag ;
    while ((option=getopt(argc, argv, "wr"))!=-1){
        switch (option){
            case ('w'):
                flag = 'w';
                //printf("-p option %d\n", atoi(optarg));
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

        printf("GPIO: %d, on_off: %d\n",wdata.GPIO_port, wdata.on_off);

        

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
            printf("GPIO_FSR_%d, %p, %x\n", i, rdata.GPIO_CTL_ADDR[i], rdata.GPIO_CTL_DATA[i]);
        }
        
        for (i=0; i< 1; i++){
            printf("GPIO_SET_%d, %p, %x\n", i, rdata.GPIO_SET_ADDR[i], rdata.GPIO_SET_DATA[i]);
        }
        for (i=0; i< 1; i++){
            printf("GPIO_CLR_%d, %p, %x\n", i, rdata.GPIO_CLR_ADDR[i], rdata.GPIO_CLR_DATA[i]);
        }
    }

    close(fd);
    return 0;
}
