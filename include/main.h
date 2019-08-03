
#ifndef _MAIN_H
#define _MAIN_H

#define Address 0x48
#define BASE 64
#define _A0 BASE+0
#define _A1 BASE+1
#define _A2 BASE+2
#define _A3 BASE+3

bool Detect_Rain();

bool Create_Serv_Clint();

bool Detect_Fire();

bool Detect_PCF8591();

bool System_Init();

bool Send_Message();

#endif
