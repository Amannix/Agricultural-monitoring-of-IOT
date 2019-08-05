#include "headfile.h"

#define HIGH_TIME  32
// Board-11  wiring-pin-0
#define SIG_GPIO_PIN 7

uint32_t dataBuf = 0;
bool ReadSensorData()
{
    uint8_t i;
    uint8_t crc; 
    // 配置引脚为输出引脚
    pinMode(SIG_GPIO_PIN, OUTPUT);
    // 拉低引脚电平25ms后拉高
    digitalWrite(SIG_GPIO_PIN, 1);
    delayMicroseconds(4);
    digitalWrite(SIG_GPIO_PIN, 0);
    delay(25);
    digitalWrite(SIG_GPIO_PIN, 1);
    delayMicroseconds(60);

    pinMode(SIG_GPIO_PIN, INPUT);
    pullUpDnControl(SIG_GPIO_PIN,PUD_UP);
    //主机开始信号结束后，DHT11发送80us低电平响应
    if(digitalRead(SIG_GPIO_PIN) == 0) {
        while(!digitalRead(SIG_GPIO_PIN));

        for(i=0;i<32;i++)
        {
            while(digitalRead(SIG_GPIO_PIN)); 
            while(!digitalRead(SIG_GPIO_PIN)); 
            delayMicroseconds(HIGH_TIME);
            dataBuf<<=1;
            if(digitalRead(SIG_GPIO_PIN)==1) 
            {
                dataBuf++;
            }
        }

        for(i=0;i<8;i++)
        {
            while(digitalRead(SIG_GPIO_PIN)); 
            while(!digitalRead(SIG_GPIO_PIN)); 
            delayMicroseconds(HIGH_TIME);
            crc<<=1;  
            if(digitalRead(SIG_GPIO_PIN)==1) 
            {
                crc++;
            }
        }
        if ((dataBuf>>24)&0xff+(dataBuf>>16)&0xff + (dataBuf>>8)&0xff + (dataBuf&0xff) == crc && crc != 0x00)   return 0;
        // 此处不对数据进行校验
        return 1;
    }
    else {
        return 0;
    }
}
bool WiringPiInit()
{
    if (-1 == wiringPiSetup()) {
        printf("Setup wiringPi failed!");
        return 1;
    }
    return 0;
}
bool Detect_DHT22(double* Temp,double* Hum)
{
    if(WiringPiInit()){
        return false;
    }
    pinMode(SIG_GPIO_PIN, OUTPUT); 
    digitalWrite(SIG_GPIO_PIN, 1); 
    delay(100);
    if(ReadSensorData()){
        *Temp = (double)((dataBuf>>16)&0xffff)*0.1;
        *Hum = (double)((dataBuf)&0xffff)*0.1;
        dataBuf=0;
        return true;
    }
    else{
        dataBuf=0;
        return false;
    }
}
