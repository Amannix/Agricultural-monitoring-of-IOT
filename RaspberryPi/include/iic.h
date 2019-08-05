#ifndef _IIC_H
#define _IIC_H

#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

#define SDA_IO_IN   digitalRead(sda)

static int scl,sda;

inline void SDA_DATA(uint8_t a);

inline void SCL_OUT(uint8_t a);

inline void delay_us(uint32_t n);

void IIC_Init(int _scl,int _sda);

void SDA_OUT(void);

void SDA_IN(void);

void IIC_Start(void);

void IIC_Stop(void);

uint8_t IIC_Wait_Ack(void);

void IIC_Ack(void);

void IIC_NAck(void);

void IIC_Send_Byte(uint8_t txd);

uint16_t IIC_Read_Byte(uint8_t ack);

#endif
