#include <Arduino.h>
#include "init.h"
#include "GlobalStructs.h"
#include "myTimers.h"
#include <esp_now.h>
#include <AsyncUDP.h>


GlobalVariables myGlobalVariables;
StoredPacket storedPacket[def_numberOfStoredPakets];
ESPNOWData MyEspNowData;
ESPNOWMessage espNOWMessage;
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
  
    Clock_Tick();

}