#pragma once
#include "mbed.h"
#include <cstdint>

static Semaphore i2c_sem(1); 
extern Mutex loraReady_mut;
extern Mutex loraSend_mut;
extern Thread sensorThread;
extern Thread loraThread;
extern uint8_t ram_sendBuffer[256];
extern uint8_t sizeToSendLora;

#define SEND_LORA_SIGNAL 0x01