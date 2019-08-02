#include "iic.h"

inline void SDA_DATA(uint8_t a)
{
    if (a){
        digitalWrite(sda,HIGH);
    }else{
        digitalWrite(sda,LOW);
    }
}

inline void SCL_OUT(uint8_t a)
{
    if (a){
        digitalWrite(scl,HIGH);
    }else{
        digitalWrite(scl,LOW);
    }
}

inline void delay_us(uint32_t n)
{
    delayMicroseconds(n);
}

void IIC_Init(int _scl,int _sda)
{
    sda = _sda;
    scl = _scl;
    wiringPiSetup();
    pinMode(scl,OUTPUT);
    pinMode(sda,OUTPUT);
    digitalWrite(scl,HIGH);
    digitalWrite(sda,HIGH);
}

void SDA_OUT()
{
    pinMode(sda,OUTPUT);
}

void SDA_IN()
{
    pinMode(sda,INPUT);
}

void IIC_Start(void)
{
    SDA_OUT();     	//sdaÏßÊä³ö
    SDA_DATA(1);	
    SCL_OUT(1);  	  
    delay_us(20);

    SDA_DATA(0);	//START:when CLK is high,DATA change form high to low 
    delay_us(20);
    SCL_OUT(0); 	//Ç¯×¡I2C×ÜÏß£¬×¼±¸·¢ËÍ»ò½ÓÊÕÊý¾Ý 
}	

void IIC_Stop(void)
{
    SDA_OUT();		//sdaÏßÊä³ö
    SCL_OUT(0);  
    SDA_DATA(0);	//STOP:when CLK is high DATA change form low to high
    delay_us(20);
    SCL_OUT(1);  
    SDA_DATA(1);	//·¢ËÍI2C×ÜÏß½áÊøÐÅºÅ
    delay_us(20);
}  

//µÈ´ýÓ¦´ðÐÅºÅµ½À´
//·µ»ØÖµ£º1£¬½ÓÊÕÓ¦´ðÊ§°Ü
//        0£¬½ÓÊÕÓ¦´ð³É¹¦
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA_DATA(1);
    SDA_IN();      		//SDAÉèÖÃÎªÊäÈë  
    delay_us(10);
    SCL_OUT(1);  	   
    delay_us(10);
    while(SDA_IO_IN)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            printf("no ack!\r\n");
            IIC_Stop();
            return 1;
        }
    }
    SCL_OUT(0);  		//Ê±ÖÓÊä³ö0 	
    return 0;  
} 

//²úÉúACKÓ¦´ð
void IIC_Ack(void)
{
    SCL_OUT(0);  	
    SDA_OUT();
    SDA_DATA(0);
    delay_us(20);
    SCL_OUT(1); 
    delay_us(20);
    SCL_OUT(0); 
} 

//²»²úÉúACKÓ¦´ð		    
void IIC_NAck(void)
{
    SCL_OUT(0); 
    SDA_OUT();
    SDA_DATA(1);
    delay_us(20);
    SCL_OUT(1); 
    delay_us(20);
    SCL_OUT(0); 
}		

//IIC·¢ËÍÒ»¸ö×Ö½Ú
//·µ»Ø´Ó»úÓÐÎÞÓ¦´ð
//1£¬ÓÐÓ¦´ð
//0£¬ÎÞÓ¦´ð			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
    SDA_OUT(); 
    SCL_OUT(0); 	    	//À­µÍÊ±ÖÓ¿ªÊ¼Êý¾Ý´«Êä
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)
            SDA_DATA(1); 
        else
            SDA_DATA(0); 
        txd<<=1; 	  
        delay_us(20);   
        SCL_OUT(1); 
        delay_us(20); 
        SCL_OUT(0); 	
        delay_us(20);
    }	
    delay_us(20);

} 	    

//¶Á1¸ö×Ö½Ú£¬ack=1Ê±£¬·¢ËÍACK£¬ack=0£¬·¢ËÍnACK   
uint16_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint16_t receive=0;
    SDA_IN();//SDAÉèÖÃÎªÊäÈë
    for(i=0;i<8;i++ )
    {
        SCL_OUT(0);  
        delay_us(20);
        SCL_OUT(1); 
        receive<<=1;
        if(SDA_IO_IN)	
            receive++;   
        delay_us(20); 
        //printf("%d,%d ",i,receive);
    }		
    //printf("\n");
    if (!ack)
        IIC_NAck();//·¢ËÍnACK
    else
        IIC_Ack(); //·¢ËÍACK   
    return receive;
}
