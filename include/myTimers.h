#ifndef MYTIMERS_H
#define MYTIMERS_H

#include <Arduino.h>
#include "GlobalStructs.h"
#include "EspNowHandle.h"


void Clock_Tick();
void hundredthSecond();
void tenthSecond();
void oneSecond();
void RefreshTimeOverNetwork();
void testByte();

#endif