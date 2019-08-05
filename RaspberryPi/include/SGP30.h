#include "headfile.h"

void SGP30_ad_write(uint8_t a,uint8_t b); 

uint32_t SGP30_ad_read();

void SGP30_Init(int scl,int sda);

bool Detect_SGP30(uint16_t* Dioxide,uint16_t* TVOC);
