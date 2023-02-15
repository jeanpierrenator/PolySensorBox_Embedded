#pragma once
#include "mbed.h"
#include <map>

#define SET_ALIME_OFF  1
#define SET_ALIME_ON  0

static  std::map<uint8_t, DigitalOut*> alimMap = {
    {1, new DigitalOut(PA_7)},
    {2, new DigitalOut(PA_4)},
    {3, new DigitalOut(PA_9)}
};

void gpio_init();
void gpio_setAlim_on(uint8_t alimLine);
void gpio_setAlim_off(uint8_t alimLine);
