#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <getopt.h>

#include "raspi_led.h"

struct Parameter parameter;

int main(int argc, char *argv[]){
    int fd;

    if (argc != 3){
        printf("Usage : raspi_gpio_test <GPIO number> <on | off>\n");
        return -1;
    }

    parameter.GPIO_port = atoi(argv[1]);
    parameter.on_off = atoi(argv[2]);

    printf("GPIO: %d, on_off: %d\n",parameter.GPIO_port, parameter.on_off);

    fd = open("/dev/led_device", O_RDWR);
    if (fd == -1){
        printf("open /dev/led_device fail\n");
        return -1;
    }

    ssize_t size_write = write(fd, &parameter, sizeof(struct Parameter));
    printf("write %ld bytes\n", size_write);
    

    return 0;
}
