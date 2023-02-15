#pragma once 
#include "mbed.h"
#include <cstdint>


int8_t RTC_init();
void RTC_setTime(uint32_t timeStamp);
uint32_t RTC_getTime();