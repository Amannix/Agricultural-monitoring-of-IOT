
#ifndef _DHT22_H
#define _DHT22_H

bool ReadSensorData();

bool WiringPiInit();

bool Detect_DHT22(double* Temp,double* Hum);

#endif
