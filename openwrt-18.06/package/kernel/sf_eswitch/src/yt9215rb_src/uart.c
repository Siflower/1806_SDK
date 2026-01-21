/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include<errno.h>
#include<termios.h>
#include<fcntl.h>
#include<sys/types.h>
#include "yt_types.h"
#include "uart.h"
#include "osal_print.h"

int32_t uart_fd = 0;
char uart_dev[128] = {0};

static int32_t uart_set_opt(int32_t fd)
{    
    struct termios  new_cfg;
    tcgetattr(fd, &new_cfg);    
    
    new_cfg.c_cflag |= (CLOCAL | CREAD);

    cfsetispeed(&new_cfg, B115200);//set baud rate  
    cfsetospeed(&new_cfg, B115200);    
 
    new_cfg.c_cflag &= ~CSIZE;    
    new_cfg.c_cflag |= CS8;    
    new_cfg.c_cflag &= ~PARENB;    
    new_cfg.c_cflag &= ~CSTOPB;    
    new_cfg.c_cc[VTIME] = 3;    
    new_cfg.c_cc[VMIN] = 1;    

    tcflush(fd,TCIFLUSH);
    tcsetattr(fd, TCSANOW, &new_cfg);
    return 0;
}

/**
can't send '\0' in the middle of str
int uart_send(  int fd,  uint8_t * str)
{
    int wr_num;
    uint8_t buf[1024];
    sprintf(buf, "%s", str);
    printf("send buf=%s\n", buf);
    wr_num = write(fd,  buf, 10);
    //close(fd);
    return wr_num;
}
**/

static int32_t uart_recv(int32_t fd, uint32_t *pRegValue)
{    
    int32_t nread;
    uint8_t buf[128];
    uint8_t i = 0;
    uint8_t loop = 0;

POS:
    *pRegValue = 0;
    nread = read(fd, buf, 128);
    if(nread > 0)            
    {
        for(i=0; i<nread; i++){
            *pRegValue |= buf[i]<<(8*i);
        }
    }
    else
    {
        loop++;
        printf("retry ..%d..\n", loop);
        if(loop < 6)
            goto POS;
    }
    return nread ;
}

void uart_exit(void)
{
    if(uart_fd)
    {
        printf("dev closed!\n");
        close(uart_fd);
        uart_fd = 0;
    }
}

int32_t uart_init(uint8_t *dev)
{
    int32_t fd = 0;
    char buffer[128] = "";
    
    if(dev != NULL)
    {
        sprintf(buffer, "/dev/%s", dev);
    }
    else
    {
        sprintf(buffer, "/dev/%s", "ttyS0");
    }

    if(uart_fd){
        uart_exit();
    }

    fd = open(buffer, O_RDWR|O_NOCTTY|O_NDELAY, 0);
    if(fd < 0)
    {
        printf("open dev %s error (%s)\n", buffer, strerror (errno));
        return -1;
    }
    fcntl(fd, F_SETFL, 0);
    uart_set_opt(fd);

    uart_fd = fd;
    sprintf(uart_dev, "%s", dev);

    return 0;
}

static int32_t switch_write(uint32_t regAddr, uint32_t value)
{
    uint8_t  valueh, valuel;
    char buffer[128];

    valueh = (uint8_t)(value >> 8);
    valuel = value & 0xFF;
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c", 0x55, 0x77, 0x72, regAddr, valueh, valuel, 0xff, 0xff, 0xff, 0xaa);
    write(uart_fd, buffer, 10);
    return 0;
}
static int32_t switch_read(uint32_t regAddr, uint32_t *pRegValue)
{
    char buffer[128];

    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c", 0x55, 0x72, 0x64, regAddr, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa);
    write(uart_fd, buffer, 10);
    uart_recv(uart_fd, pRegValue);
    return 0;
}

// serial interface read switch half word
int32_t uart_switch_read(uint32_t regAddr, uint32_t *pRegAddr)
{
    uint32_t reg = regAddr, value, tmpValueL, tmpValueH;
    uint8_t is_data = 0, sw_id = 0, is_read = 1;

    if(uart_fd == 0)
    {
        printf("serial read error, please connect the serial dev first\n");
        return -1;
    }

    is_data = 0;
    reg = (sw_id<<2) |(is_data<<1)|is_read;
    value = (regAddr>>16) & 0xFFFF;
    switch_write(reg, value);
    value = (regAddr) & 0xFFFF;
    switch_write(reg, value);

    is_data = 1;
    reg = (sw_id<<2) |(is_data<<1)|is_read;
    switch_read(reg, &tmpValueL);
    switch_read(reg, &tmpValueH);
    *pRegAddr = tmpValueH |(tmpValueL<<16);
//osal_printf("Uart Read addr[0x%x] = 0x%x\n",regAddr,*pRegAddr);
    return 0;
}

int32_t uart_switch_write(uint32_t regAddr,uint32_t regValue)
{
    uint32_t reg = regAddr, value;
    uint32_t valueL, valueH;
    uint8_t is_data = 0, sw_id = 0, is_read = 0;

    if(uart_fd == 0)
    {
        printf("serial write error, please connect the serial dev first\n");
        return -1;
    }
    valueL = regValue &0xFFFF;
    valueH = (regValue>>16) &0xFFFF;

    is_data = 0;
    reg = (sw_id<<2) |(is_data<<1)|is_read;
    value = (regAddr >>16)& 0xFFFF;
    switch_write(reg, value);
    value = (regAddr)& 0xFFFF;
    switch_write(reg, value);

    is_data = 1;
    reg = (sw_id<<2) |(is_data<<1)|is_read;
    switch_write(reg, valueH);
    switch_write(reg, valueL);
//osal_printf("Uart Write addr[0x%x] = 0x%x\n",regAddr,regValue);
    return 0;
}

int32_t phy_operation(uint32_t addr)
{
    char buffer[128];

    if(uart_fd == 0)
    {
        printf("serial write error, please connect the serial dev first\n");
        return -1;
    }

    printf("phy addr 0x%x\n", addr);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c", 0x55, 0x70, 0x68, addr, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaa);
    write(uart_fd, buffer, 10);
    return 0;
}

