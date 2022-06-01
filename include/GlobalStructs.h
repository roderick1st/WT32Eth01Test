#ifndef GLOBALSTRUCTS_H
#define GLOBALSTRUCTS_H

#include <Arduino.h>
#include "Definitions.h"

struct GlobalVariables {
    IPAddress staticIP;
    IPAddress netmaskIP;
    IPAddress gatewayIP;
    IPAddress broadcastIP;   
    unsigned long currentTime; 
    unsigned long previousTime;
    unsigned long hundredthSecond;
    unsigned long tenthSecond;
    unsigned long oneSecond;
};

extern GlobalVariables myGlobalVariables;


struct ESPNOWData{
    int messageID;
    unsigned long messageTimeStamp;

    char IMU_Roll[6];
    char IMU_Pitch[6];
    char IMU_Heading[6];
    char IMU_Yaw[6];

    char Panda[def_LengthOfPandaString];

    int ntripLen;
};

extern ESPNOWData MyEspNowData;


struct IMUData{
    unsigned long messageTimeStamp;
    char IMU_Roll[6];
    char IMU_Pitch[6];
    char IMU_Heading[6];
    char IMU_Yaw[6];
};

extern IMUData myIMUData[100];



#endif