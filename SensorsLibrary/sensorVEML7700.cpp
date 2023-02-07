#include "sensorVEML7700.h"
#include <cstdint>

uint32_t SensorVEML7700::getLux(){
    int res;
    uint32_t luxUint =0;
        uint16_t value;
        res += VEML7700::getInstance()->getALS(value);
        convertFloatToUint32((float)(value/10),luxUint);
        return luxUint;
}
int32_t SensorVEML7700::init(){
        gpio_setAlim_on(this->getLine());
        ThisThread::sleep_for(chrono::milliseconds(100));
        VEML7700::getInstance()->setALSConf(384);
        ThisThread::sleep_for(chrono::milliseconds(150));
        return 0;
}
int32_t SensorVEML7700::wakeUp(){
        gpio_setAlim_on(this->getLine());
        ThisThread::sleep_for(chrono::milliseconds(100));
        VEML7700::getInstance()->setALSConf(384);
        ThisThread::sleep_for(chrono::milliseconds(150));
    return 0;
}
int32_t SensorVEML7700::sleep(){
    gpio_setAlim_off(this->getLine());
        return 0;
}
