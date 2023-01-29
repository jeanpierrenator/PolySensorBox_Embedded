#include "gpio.h"

void gpio_init(){
     for( auto &element : alimMap)
    { 
        element.second->write(SET_ALIME_OFF);
    }
}

void gpio_setAlim_on(uint8_t alimLine){
    alimMap[alimLine]->write(SET_ALIME_ON);
}
void gpio_setAlim_off(uint8_t alimLine){
    alimMap[alimLine]->write(SET_ALIME_OFF);
}
