#ifndef RASPI_GPIO_H_

#define RASPI_GPIO_H_

typedef struct WData{
    unsigned int GPIO_port;
    int on_off;
} WData;

typedef struct RData{
    unsigned int *GPIO_CTL_ADDR[5];
    unsigned int GPIO_CTL_DATA[5];
    unsigned int *GPIO_SET_ADDR[2];
    unsigned int GPIO_SET_DATA[2];
    unsigned int *GPIO_CLR_ADDR[2];
    unsigned int GPIO_CLR_DATA[2];
} RData;

#endif
