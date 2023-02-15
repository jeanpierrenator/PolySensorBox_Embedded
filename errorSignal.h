#pragma once 
#include "mbed.h"



#define errorLed LED3
#define memoryFaultErrorDelay 500ms
#define configurationFaultErrorDelay 100ms
#define loraDisconnectedErrorDelay 250ms
#define loraNoErrorDelay 3000ms


void memoryFaultError();
void loraDisconnectedError();
void loraNoError();
void configurationFaultError();