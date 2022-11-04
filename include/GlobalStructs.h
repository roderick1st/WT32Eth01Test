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
    unsigned long SystemTime;
    byte ntripNOWPAcket[10][def_LengthOfNtripString];
    bool LastNTRIPSent;

    unsigned long LastPacketSentTime;
    unsigned long LastGPSMessageTime;
    int gpsCount;
};

extern GlobalVariables myGlobalVariables;

typedef struct StoredPacket{
    bool InUse;
    byte PacketData[def_maxDataSizeOfStoredPacket];
    int PacketDataLen;
} StoredPacket;
extern StoredPacket storedPacket[def_numberOfStoredPakets];


struct ESPNOWData{
    int messageID; //4 bytes
    unsigned long messageTimeStamp; //4 bytes

    char IMU_Roll[6];
    char IMU_Pitch[6];
    char IMU_Heading[6];
    char IMU_Yaw[6];

    char Panda[def_LengthOfPandaString]; //100 bytes
    //132

    int ntripLen; //4 bytes
    //136
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

typedef struct ESPNOWMessage{
    int messageID;                                  //4 bytes
    unsigned long masterTime;                       //4 bytes
    char pandaMessage[def_LengthOfPandaString];     //100
    byte ntripMessage[def_LengthOfNtripString];     //128 bytes
    //int ntripLen; //remove                        //236 bytes
}ESPNOWMessage;
extern ESPNOWMessage espNOWMessage;




#endif