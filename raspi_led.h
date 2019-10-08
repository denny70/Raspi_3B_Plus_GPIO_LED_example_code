#ifndef RASPI_GPIO_H_

#define RASPI_GPIO_H_

typedef struct WData{
    unsigned int GPIO_port;
    int on_off;
} WData;

typedef struct RData{
    //Because There are GPIO BCM pin 2-27 on the board, you do not have physical pin (28-49) you can plug in
    volatile unsigned int *GPIO_ctl_addr[3];    //GPIO Function Select 0,1,2 (3,4,5 will not to be used)
    unsigned int  GPIO_ctl_data[3];             //GPIO Function Select value 0,1,2
} RData;

#endif
