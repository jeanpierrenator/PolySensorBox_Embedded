#include "sensorSGP30.h"

uint32_t SensorSGP30::getTOVC(){

    Adafruit_SGP30 * SGP30 = Adafruit_SGP30::getInstance();
    SGP30->IAQmeasure();
    
    return SGP30->TVOC;
}

uint32_t SensorSGP30::getECo2(){
    Adafruit_SGP30 * SGP30 = Adafruit_SGP30::getInstance();
    SGP30->IAQmeasure();
    
    return SGP30->eCO2;
}


int32_t SensorSGP30::init(){
    int ret  = 0;
        Adafruit_SGP30 * SGP30 = Adafruit_SGP30::getInstance();
        ret += SGP30->begin();
        ret += SGP30->IAQinit();
        return ret;
}
int32_t SensorSGP30::wakeUp(){
    return 0;
}
int32_t SensorSGP30::sleep(){

        return 0;
}