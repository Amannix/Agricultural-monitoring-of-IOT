#include "headfile.h"

#define I2C_ADDR 0x23

float Detect_light(void)
{
    int fd;
    char buf[3];
    char val,value;
    float flight;
    fd=open("/dev/i2c-1",O_RDWR);
    if(fd<0)
    {
        printf("打开文件错误:%s\r\n",strerror(errno)); return 1;
    }
    if(ioctl( fd,I2C_SLAVE,I2C_ADDR)<0 )
    {
        printf("ioctl 错误 : %s\r\n",strerror(errno));return 1;
    }
    val=0x01;
    if(write(fd,&val,1)<0)
    {
        printf("上电失败\r\n");
    }
    val=0x11;
    if(write(fd,&val,1)<0)
    {
        printf("开启高分辨率模式2\r\n");
    }
    usleep(20000);
    if(read(fd,&buf,3)){
        flight=(buf[0]*256+buf[1])*0.5/1.2;
        return flight;
    }
    else{
        printf("读取错误\r\n");
        return -1;
    }
}
