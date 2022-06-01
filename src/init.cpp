#include "init.h"

void init_Global_Vars(){
    myGlobalVariables.staticIP = {192, 168, 1, 126};
    myGlobalVariables.netmaskIP = {255, 255, 255, 0};
    myGlobalVariables.gatewayIP = {192, 168, 1, 1};
    myGlobalVariables.broadcastIP = {192, 168, 1, 255};

    myGlobalVariables.currentTime = 0;
    myGlobalVariables.previousTime = 0;
    myGlobalVariables.hundredthSecond = 0;
    myGlobalVariables.tenthSecond = 0;
    myGlobalVariables.oneSecond = 0;
}

void init_Ethernet(){

    bool eth_Status = ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);

    if(!eth_Status){
        Serial.println("No Ethernet - Restarting");
        ESP.restart();
    }

    ETH.config(myGlobalVariables.staticIP, myGlobalVariables.gatewayIP, myGlobalVariables.netmaskIP);

}

void init_WiFi(){

    //WiFi.mode(WIFI_AP_STA);
    WiFi.mode(WIFI_STA);

    uint8_t myMac[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x01};
    uint8_t BroadcastMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    esp_wifi_set_mac(WIFI_IF_STA, &myMac[0]);
    Serial.print("New WiFi Mac address : "); Serial.println(WiFi.macAddress());

    //esp now
    if(esp_now_init() != ESP_OK){
        Serial.println("Error initilising ESP-NOW");
    } else {
        //esp_now_register_send_cb(OnDataSent);
        esp_now_register_recv_cb(OnDataRecv);

        memcpy(peerInfo.peer_addr, BroadcastMac, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;

        if(esp_now_add_peer(&peerInfo) != ESP_OK){
            Serial.println("Adding NOW peer failed. Restarting...");
            delay(1000);
            ESP.restart();
        }
    }


}

void initNtrip(){

    udpHandler.listen(def_NtripPort);

    udpHandler.onPacket([](AsyncUDPPacket packet){     
            
                int dataLen = 0;

                for (unsigned int i = 0; i < packet.length(); i++) //loop through the packet
                {
                    MyEspNowData.Panda[dataLen] = packet.data()[i];
                    dataLen++;

                    if(dataLen == 100){
                        MyEspNowData.ntripLen = dataLen;
                        dataLen = 0;
                        SendOutNowBroadcast(20);
                        memset(MyEspNowData.Panda, 0, sizeof(MyEspNowData.Panda));
                        MyEspNowData.ntripLen = 0;
                    }
                           
                }

                if(dataLen > 0){
                    MyEspNowData.ntripLen = dataLen;
                    SendOutNowBroadcast(20);
                    memset(MyEspNowData.Panda, 0, sizeof(MyEspNowData.Panda));
                    MyEspNowData.ntripLen = 0;
                }
    });

    
}

