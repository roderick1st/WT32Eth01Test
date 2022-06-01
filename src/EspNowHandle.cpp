#include "EspNowHandle.h"


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){


    memcpy(&MyEspNowData, incomingData, sizeof(MyEspNowData));

    switch(MyEspNowData.messageID){
        case 1: //time adjust - not likely to be recieved by this devices as its the sender for time updates
        break;

        case 2: //GPS message
            SendOverEthernet();
            //SendOverUSB();
        break;

        case 3: //imu message
            //StoreIMUData();
            
        break;
    }
    
}

void SendOutNowBroadcast(int messageType){

    uint8_t BroadcastMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    MyEspNowData.messageID = messageType;
    esp_err_t result = esp_now_send(BroadcastMac, (uint8_t *) &MyEspNowData, sizeof(MyEspNowData));

}

void SendOverEthernet(){

    udpHandler.broadcastTo(MyEspNowData.Panda, 9999);

}

void SendOverUSB(){
    Serial.print(MyEspNowData.Panda);
}

void SendNtripOverNOW(int messType){

    uint8_t BroadcastMac[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x02};

    MyEspNowData.messageID = messType;
    esp_err_t result = esp_now_send(BroadcastMac, (uint8_t *) &MyEspNowData, sizeof(MyEspNowData));
    Serial.println(result);

}
