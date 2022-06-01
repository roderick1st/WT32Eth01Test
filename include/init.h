#ifndef INIT_H
#define INIT_H

#include <Arduino.h>
#include <ETH.h>
#include "Definitions.h"
#include "GlobalStructs.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include "EspNowHandle.h"
#include <AsyncUDP.h>

extern esp_now_peer_info_t peerInfo;
extern AsyncUDP udpHandler;

void init_Global_Vars();
void init_Ethernet();
void init_WiFi();
void initNtrip();


#endif