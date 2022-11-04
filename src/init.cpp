#include "init.h"

void init_Global_Vars(){
    //myGlobalVariables.staticIP = {192, 168, 1, 126};
    //myGlobalVariables.netmaskIP = {255, 255, 255, 0};
    //myGlobalVariables.gatewayIP = {192, 168, 1, 1};
    //myGlobalVariables.broadcastIP = {192, 168, 1, 255};

    myGlobalVariables.staticIP = {192, 168, 2, 22};
    myGlobalVariables.netmaskIP = {255, 255, 255, 0};
    myGlobalVariables.gatewayIP = {192, 168, 2, 1};
    myGlobalVariables.broadcastIP = {192, 168, 2, 255};

    myGlobalVariables.currentTime = 0;
    myGlobalVariables.previousTime = 0;
    myGlobalVariables.hundredthSecond = 0;
    myGlobalVariables.tenthSecond = 0;
    myGlobalVariables.oneSecond = 0;

    myGlobalVariables.LastNTRIPSent = true;
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
    //uint8_t BroadcastMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    esp_wifi_set_mac(WIFI_IF_STA, &myMac[0]);
    Serial.print("New WiFi Mac address : "); Serial.println(WiFi.macAddress());

    //esp now
    if(esp_now_init() != ESP_OK){
        Serial.println("Error initilising ESP-NOW");
        ESP.restart();
    } else {
        //esp_now_register_send_cb(OnDataSent);

        //memcpy(peerInfo.peer_addr, BroadcastMac, 6);
        //peerInfo.channel = 0;
        //peerInfo.encrypt = false;

        //if(esp_now_add_peer(&peerInfo) != ESP_OK){
        //    Serial.println("Adding NOW peer failed. Restarting...");
        //    delay(1000);
        //    ESP.restart();
        //}
    }

    esp_now_register_recv_cb(OnDataRecv);
    esp_now_register_send_cb(OnDataSent);


}

void initNtrip(){

    udpHandler.listen(def_NtripPort);

    udpHandler.onPacket([](AsyncUDPPacket packet){ //packet recieved interupt

        Serial.println("");
        Serial.println("");
        Serial.println("");
        Serial.print("UDP Packet arrived : Length = "); Serial.println(packet.length());

        for(int loopP = 0; loopP < packet.length(); loopP++){
            Serial.print(packet.data()[loopP]);
        }
        Serial.println("");
        Serial.println("++++++++++++++++++++++++++++++");
        Serial.println("");

        int availablePacket = -1;

        for(int currentStore = 0; currentStore < def_numberOfStoredPakets; currentStore++){
            if(!storedPacket[currentStore].InUse){ //we have a spare
            //Serial.print("Spare store found : "); Serial.println(currentStore);
                availablePacket = currentStore;
                break; //exit the for loop
            }
        }

        //copy the packet to the store
        if(packet.length() <= sizeof(storedPacket[availablePacket].PacketData)){
            //memcpy(storedPacket[availablePacket].PacketData, packet.data(), sizeof(packet.data())); //copy the packet data into storedPacket
            std::copy(packet.data(), packet.data()+packet.length(), storedPacket[availablePacket].PacketData);
            storedPacket[availablePacket].PacketDataLen = packet.length();
            storedPacket[availablePacket].InUse = true;
        }

        
        Serial.println("");
        Serial.println("Memcpy Packet arrived");
        Serial.println("");

        for(int loopP = 0; loopP < 256; loopP++){
            Serial.print(storedPacket[availablePacket].PacketData[loopP]);
        }
        Serial.println("");
        Serial.println("++++++++++++++++++++++++++++++");
        Serial.println("");


    delay(1000);
    

/*
        

        if(myGlobalVariables.LastNTRIPSent == true){ 

            //Serial.println("UDP packet recieved");
            //Serial.println("000000000000 #   * \t S \t - \t 3 \t . \t 1 \t : \t nu \t :");

            //new idea to make ESP-NOW packets

            //we have the whole pachet...
            //size of header is 7
            //size of footer is 3 so 10 over head

            //int def_ntripPacketHeaderLen = 10;

            int packetNumber = 0;
            int nTripPacketItemCounter = def_ntripPacketHeaderLen;
            bool newNOWPacket = true;
            int currentPacketLen = 0;
            char nTripPacketLenString[2];


            for(int resetP = 0; resetP < 10; resetP++){
                memset(myGlobalVariables.ntripNOWPAcket[resetP], 0, sizeof(myGlobalVariables.ntripNOWPAcket[resetP]));
            }

            for(int packetItem = 0; packetItem < packet.length(); packetItem++){

                if(newNOWPacket){ //starting a new now packet so add header
                    newNOWPacket = false; //not a new packet
                    //add the header
                    //#*?-?.?:??:
                    myGlobalVariables.ntripNOWPAcket[packetNumber][0] = 35; //#
                    myGlobalVariables.ntripNOWPAcket[packetNumber][1] = 42; //*
                    myGlobalVariables.ntripNOWPAcket[packetNumber][3] = 45; //-
                    myGlobalVariables.ntripNOWPAcket[packetNumber][4] = 48; //0
                    myGlobalVariables.ntripNOWPAcket[packetNumber][5] = 46; //.
                    myGlobalVariables.ntripNOWPAcket[packetNumber][6] = 48; //0
                    myGlobalVariables.ntripNOWPAcket[packetNumber][7] = 58; //:
                    myGlobalVariables.ntripNOWPAcket[packetNumber][8] = 48; //0 //packet byte length
                    myGlobalVariables.ntripNOWPAcket[packetNumber][9] = 58; //:
                
                        if(packetNumber == 0){ //first packet
                            myGlobalVariables.ntripNOWPAcket[packetNumber][2] = 83; //'S'; //start packet #*S-?.?:??:
                        } else {
                            myGlobalVariables.ntripNOWPAcket[packetNumber][2] = 77; //'M'; //middle packet #*M-?.?:??:
                        }

                }
    
        

                myGlobalVariables.ntripNOWPAcket[packetNumber][nTripPacketItemCounter] = packet.data()[packetItem];
                nTripPacketItemCounter++; //increment by 1
                currentPacketLen++;

                    if(nTripPacketItemCounter == (def_LengthOfNtripString)){ //length of total packet minus the header
                        newNOWPacket = true; //start new packet heading
                        //we have filled the packet
                        myGlobalVariables.ntripNOWPAcket[packetNumber][8] = currentPacketLen;
                        currentPacketLen = 0; //reset the packet length counter
                        packetNumber++; //move to next packet
                        nTripPacketItemCounter = def_ntripPacketHeaderLen; //reset the couter to the begining of data.
                    }
    
            }
    
    
                //finished looping through the udp packet
                if(nTripPacketItemCounter == def_ntripPacketHeaderLen) {//have we created part of a new packet.
                    packetNumber--; //go back to finnished packet
                    myGlobalVariables.ntripNOWPAcket[packetNumber][2] = 69; //'E'; //end packet #*E-?.?:??:
                    //myGlobalVariables.ntripNOWPAcket[packetNumber][8] = currentPacketLen;
                } else {
                    myGlobalVariables.ntripNOWPAcket[packetNumber][2] = 69; //'E'; //end packet #*E-?.?:??:
                    myGlobalVariables.ntripNOWPAcket[packetNumber][0] = 35; //#
                    myGlobalVariables.ntripNOWPAcket[packetNumber][1] = 42; //*
                    myGlobalVariables.ntripNOWPAcket[packetNumber][3] = 45; //-
                    myGlobalVariables.ntripNOWPAcket[packetNumber][4] = 48; //0
                    myGlobalVariables.ntripNOWPAcket[packetNumber][5] = 46; //.
                    myGlobalVariables.ntripNOWPAcket[packetNumber][6] = 48; //0
                    myGlobalVariables.ntripNOWPAcket[packetNumber][7] = 58; //:
                    myGlobalVariables.ntripNOWPAcket[packetNumber][8] = currentPacketLen;
                    myGlobalVariables.ntripNOWPAcket[packetNumber][9] = 58; //:
                }

                for(int packetNumberSort = 0; packetNumberSort <= packetNumber; packetNumberSort++){ //lets lable the packet
                    myGlobalVariables.ntripNOWPAcket[packetNumberSort][4] = packetNumber + 1;
                    myGlobalVariables.ntripNOWPAcket[packetNumberSort][6] = packetNumberSort + 1;
                }

                

                
                //lets test our packets
                for(int loopPacket = 0; loopPacket < 10; loopPacket++){
                    Serial.print("Packet : "); Serial.print(loopPacket); Serial.print(" : ");
                    //for(int loopContents = 0; loopContents < def_LengthOfNtripString; loopContents++){
                    for(int loopContents = 0; loopContents < def_ntripPacketHeaderLen; loopContents++){
                        Serial.print(myGlobalVariables.ntripNOWPAcket[loopPacket][loopContents]);
                        Serial.print("\t"); //tab
                    }
                    for(int loopc = def_ntripPacketHeaderLen; loopc < def_LengthOfNtripString; loopc++){
                        Serial.print(myGlobalVariables.ntripNOWPAcket[loopPacket][loopc]);
                    }
                    Serial.println("");
                }
                //delay(5000);
                
                myGlobalVariables.LastPacketSentTime = millis();

                SendOutNTRIPOverNOW(packetNumber);
        }
        //}
        */
    
    });
}
