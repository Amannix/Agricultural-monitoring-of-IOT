#include "headfile.h"

uint16_t Dioxide,TVOC;
double Temp,Humidity;
uint8_t Oxygen,Power,Light,Soil_Humidity;
bool Rain,Fire;

using namespace std;

int main ()
{
    System_Init();
    while (1){
        Detect_SGP30(&Dioxide,&TVOC);
        Detect_DHT22(&Temp,&Humidity);
        Detect_PCF8591();
        cout << Dioxide << ' ' << TVOC << endl;
        cout << Temp << ' ' << Humidity << endl;
        printf ("%d %d %d %d\n",Light,Power,Soil_Humidity,Oxygen);
        printf ("%s\n%s\n",!Detect_Rain() ? "下雨" : "没下雨", !Detect_Fire() ? "着火" : "没着火");
        delay(1000);
    }

    return 0;
}

bool Detect_PCF8591()
{
    Light = analogRead(_A0);
    delay(10);
    Power = analogRead(_A1);
    delay(10);
    Soil_Humidity = analogRead(_A2);
    delay(10);
    Oxygen = analogRead(_A3);
    delay(10);
}

bool System_Init()
{
    wiringPiSetup();
    pcf8591Setup(BASE,Address);
    pinMode(Rain_Sig_Pin,INPUT);
    pinMode(Fire_Sig_Pin,INPUT);
    SGP30_Init(4,5);
    delay(1000);
}

bool Detect_Rain()
{
    return digitalRead(Rain_Sig_Pin);
}

bool Detect_Fire()
{
    return digitalRead(Fire_Sig_Pin);
}
