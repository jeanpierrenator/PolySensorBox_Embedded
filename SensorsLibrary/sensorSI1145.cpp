#include "sensorSI1145.h"
#include <cstdint>

uint32_t SensorSI1145::getUV(){
    int ret;
    uint32_t uvUint =0;
        uint16_t uvf;
        ret += SI1145::getInstance()->getUV(uvf);
        ret += SI1145::getInstance()->getUV(uvf);
         convertFloatToUint32((float)uvf,uvUint);
    return uvUint;
}

int32_t SensorSI1145::init(){

         int ret;
        gpio_setAlim_on(this->getLine());
        ret += SI1145::getInstance()->initalize();
        return ret ;
}
int32_t SensorSI1145::wakeUp(){
    int ret;
        gpio_setAlim_on(this->getLine());
        ret += SI1145::getInstance()->initalize();
        return ret ;
}
int32_t SensorSI1145::sleep(){
    gpio_setAlim_off(this->getLine());
    return 0;
}



