#ifndef ESPNOWHANDLE_H
#define ESPNOWHANDLE_H

#include <Arduino.h>
#include "GlobalStructs.h"
#include <esp_now.h>
#include "Definitions.h"
#include <AsyncUDP.h>

extern esp_now_peer_info_t peerInfo;
extern AsyncUDP udpHandler;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void SendOutNowBroadcast(int messageType);
void SendOverEthernet();
void SendOverUSB();
void SendNtripOverNOW(int messType);

#endif