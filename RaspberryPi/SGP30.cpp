#include "headfile.h"

void SGP30_ad_write(uint8_t a,uint8_t b)
{
    IIC_Start();
    IIC_Send_Byte(0xb0);
    IIC_Wait_Ack();
    IIC_Send_Byte(a);
    IIC_Wait_Ack();
    IIC_Send_Byte(b);
    IIC_Wait_Ack();
    IIC_Stop();
    delay(100);
}

uint32_t SGP30_ad_read()
{
    uint32_t dat;
    uint8_t crc;

    IIC_Start();
    IIC_Send_Byte(0xb1);
    IIC_Wait_Ack();
    dat = IIC_Read_Byte(1);
    dat <<= 8;
    dat += IIC_Read_Byte(1);
    crc = IIC_Read_Byte(1);
    dat <<= 8;
    dat += IIC_Read_Byte(1);
    dat <<= 8;
    dat += IIC_Read_Byte(0);
    IIC_Stop();
    return dat;
}

void SGP30_Init(int scl,int sda)
{
    IIC_Init(scl,sda);
    SGP30_ad_write(0x20,0x03);
}

bool Detect_SGP30(uint16_t* Dioxide,uint16_t* TVOC)
{ 
    SGP30_ad_write(0x20,0x08);
    uint32_t dat = SGP30_ad_read();
    *Dioxide = (dat&0xffff0000) >> 16;
    *TVOC = dat&0x0000ffff;
}
