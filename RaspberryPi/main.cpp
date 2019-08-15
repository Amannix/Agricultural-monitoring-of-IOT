#include "headfile.h"

uint16_t Dioxide,TVOC;
double Temp,Humidity,Light;
uint16_t Oxygen,Power,Soil_Humidity;
bool Rain,Fire;
char buff[1024];
int sock,len_t;
struct sockaddr_in serv_addr;
const int Rain_Sig_Pin = 0;
const int Fire_Sig_Pin = 2;

SensorNode Data;

using namespace std;

char IP[20];
int prot;

int main (int args,char** argv)
{
    if(args != 3){
        printf ("Usage : %s <IP> <port> \r\n",argv[0]);
        return -1;
    }
    strcpy(IP,argv[1]);
    prot = htons(atoi(argv[2]));
    if (System_Init() == false){
        printf ("初始化失败\r\n");
        return -1;
    }
    while (1){
        Detect_SGP30(&Data.Dioxide,&Data.TVOC);
        Detect_DHT22(&Data.Temp,&Data.Humidity);
        Detect_PCF8591();
        Data.Rain = Detect_Rain();
        Data.Fire = Detect_Fire();

        cout << Data.Dioxide << ' ' << Data.TVOC << endl;
        cout << Data.Temp << ' ' << Data.Humidity << endl;
        printf ("%lf %d %d %d\n",Data.Light,Data.Power,Data.Soil_Humidity,Data.Oxygen);
        printf ("%s\n%s\n",Detect_Rain() ? "下雨" : "没下雨", Detect_Fire() ? "着火" : "没着火");
        Send_Message();
        delay(1000);
    }

    return 0;
}

bool Create_Serv_Clint()
{
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        return false;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = prot;

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        return false;
    }
    write(sock, "hello", 5);
    return true;
}

bool Send_Message()
{
    write(sock, (char*)(&Data), sizeof Data);
}

bool Detect_PCF8591()
{
    Data.Light = Detect_light();
    delay(10);
    Data.Power = analogRead(_A1);
    delay(10);
    Data.Soil_Humidity = analogRead(_A2);
    delay(10);
    Data.Oxygen = analogRead(_A3);
    delay(10);
}

bool System_Init()
{
    if (!Create_Serv_Clint()){
        printf ("连接服务器失败\r\n");
        return false;
    }
    wiringPiSetup();
    pcf8591Setup(BASE,Address);
    pinMode(Rain_Sig_Pin,INPUT);
    pinMode(Fire_Sig_Pin,INPUT);
    SGP30_Init(4,5);
    delay(1000);
    return true;
}

bool Detect_Rain()
{
    Data.Rain = !digitalRead(Rain_Sig_Pin);
    return Data.Rain;
}

bool Detect_Fire()
{
    Data.Fire = !digitalRead(Fire_Sig_Pin);
    return Data.Fire;
}
