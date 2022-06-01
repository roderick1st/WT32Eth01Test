#include <Arduino.h>
#include "init.h"
#include "GlobalStructs.h"
#include "myTimers.h"
#include <esp_now.h>
#include <AsyncUDP.h>


GlobalVariables myGlobalVariables;
ESPNOWData MyEspNowData;
esp_now_peer_info_t peerInfo;
IMUData myIMUData[100];
AsyncUDP udpHandler;


bool eth_Status;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  init_Global_Vars();
  init_Ethernet();
  init_WiFi();
  initNtrip();

}

void loop() {
  // put your main code here, to run repeatedly:
  //IPAddress myIp = ETH.localIP();
  
  //Serial.println(ETH.localIP());
  //delay(500);

  Clock_Tick();

}