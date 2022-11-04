#include "EspNowHandle.h"


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){


    //memcpy(&MyEspNowData, incomingData, sizeof(MyEspNowData));
    memcpy(&espNOWMessage, incomingData, sizeof(espNOWMessage));

    //Serial.print("Message ID recieved: "); Serial.println(MyEspNowData.messageID);

    switch(espNOWMessage.messageID){

        case 1: //GPS message

            /*unsigned long myGpsReadings = millis() - myGlobalVariables.LastPacketSentTime;
            myGpsReadings = 1000/myGpsReadings;
            myGlobalVariables.LastPacketSentTime = millis();
            Serial.print("GPS Readings per seconds :::: "); Serial.println(myGpsReadings);
        
            if((millis() - myGlobalVariables.LastGPSMessageTime) > 1000){
                Serial.print("GPS readings per second : "); Serial.println(myGlobalVariables.gpsCount);
                myGlobalVariables.LastGPSMessageTime = millis();
                myGlobalVariables.gpsCount = 0;
            }
            myGlobalVariables.gpsCount++;
            */
            myGlobalVariables.SystemTime = espNOWMessage.masterTime;
            SendOverEthernet();
            //SendOverUSB();
        break;

        //case 3: //imu message
            //StoreIMUData();
            
        //break;
    }
    
    //Serial.print(espNOWMessage.pandaMessage);
    
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void SendOutNowBroadcast(int messageType){

    uint8_t BroadcastMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    MyEspNowData.messageID = messageType;
    esp_err_t result = esp_now_send(BroadcastMac, (uint8_t *) &MyEspNowData, sizeof(MyEspNowData));

}



void SendOverEthernet(){

    //udpHandler.broadcastTo(MyEspNowData.Panda, 9999);
    udpHandler.broadcastTo(espNOWMessage.pandaMessage, 9999);

}

void SendOverUSB(){
    Serial.print(MyEspNowData.Panda);
}

/*
void SendNtripOverNOW(int messType){

    uint8_t BroadcastMac[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x02};

    espNOWMessage.masterTime = myGlobalVariables.SystemTime;
    espNOWMessage.messageID = 2;


    //MyEspNowData.messageID = messType;
    //esp_err_t result = esp_now_send(BroadcastMac, (uint8_t *) &MyEspNowData, sizeof(MyEspNowData));
    //Serial.println(result);

}
*/

void SendOutNTRIPOverNOW(int packetNumber){

    myGlobalVariables.LastNTRIPSent = false;

    uint8_t GPSMac[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x02};

    if(esp_now_is_peer_exist(GPSMac) == 0){

        memcpy(peerInfo.peer_addr, GPSMac, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;

        if(esp_now_add_peer(&peerInfo) != ESP_OK){
            Serial.println("Peer add failed");
            return;
        } else {
            Serial.println("Peer add successfull");
        }
    }

    //send the packet out
    int espPackets = std::ceil((long double)storedPacket[packetNumber].PacketDataLen / (long double)def_LengthOfNtripString);
    int dataStart = 0;
    int dataEnd = 0; 

    if(storedPacket[packetNumber].PacketDataLen < def_LengthOfNtripString){ //check the length of the data and generate the first end
        dataEnd = storedPacket[packetNumber].PacketDataLen - 1;
    } else {
        dataEnd = def_LengthOfNtripString - 1;
    }

    int dataRemaining = storedPacket[packetNumber].PacketDataLen;
    int dataSent = 0;

    Serial.println("");
    Serial.println("Passed Packet");
    for(int testint = 0; testint < def_maxDataSizeOfStoredPacket; testint++){
            Serial.print(storedPacket[packetNumber].PacketData[testint]);
    }
    Serial.println("");
    Serial.println("");
    Serial.println("Broken down packet");

    for (int arrayPart = 0; arrayPart < espPackets; arrayPart++){
        std::copy(storedPacket[packetNumber].PacketData+dataStart, storedPacket[packetNumber].PacketData+dataEnd,std::begin(espNOWMessage.ntripMessage));
        //send the message

        for(int testint = 0; testint < def_LengthOfNtripString; testint++){
           Serial.print(espNOWMessage.ntripMessage[testint]); 
        }
        
        std::fill_n(espNOWMessage.ntripMessage, def_LengthOfNtripString, 0); //clear the data

        dataSent = dataEnd - dataStart + 1;
        dataRemaining = dataRemaining - dataSent;
        dataStart = dataEnd + 1;
        if(dataRemaining < def_LengthOfNtripString){
            dataEnd = dataStart + def_LengthOfNtripString;
        } else {
            dataEnd = dataStart + dataRemaining;
        }
    }
    std::fill_n(storedPacket[packetNumber].PacketData, def_maxDataSizeOfStoredPacket, 0); //blank the packet data
    storedPacket[packetNumber].PacketDataLen = 0;
    storedPacket[packetNumber].InUse = false;

    Serial.println("");
    Serial.println("");
    //std::copy()

    //break the packet apart into smaller chuncks
    /*
    for(int messageCount = 0; messageCount <= packetNumber; messageCount++){
        std::copy(std::begin(myGlobalVariables.ntripNOWPAcket[messageCount]), std::end(myGlobalVariables.ntripNOWPAcket[messageCount]), std::begin(espNOWMessage.ntripMessage));
        espNOWMessage.messageID = 2; //NTRIP data
        esp_err_t result = esp_now_send(GPSMac, (uint8_t *) &espNOWMessage, sizeof(espNOWMessage));
    }

    myGlobalVariables.LastNTRIPSent = true;
    */

}

void CheckForNewNtripPacket(){

    for(int currentPacket = 0; currentPacket < def_numberOfStoredPakets; currentPacket++){
        if(storedPacket->InUse){
            //pass the packet for sending via ESPNOW
            SendOutNTRIPOverNOW(currentPacket);

        }
    }

}
