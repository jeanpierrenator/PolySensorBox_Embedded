#pragma once 
#include "mbed.h"

#define errorLed LED3
#define memoryFaultErrorDelay 500ms
#define configurationFaultErrorDelay 100ms


void memoryFaultError();
void loraFaultError();
void configurationFaultError();