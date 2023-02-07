#include "sensorSCD30.h"
#include <cstdint>

uint32_t SensorSCD30::getTemp(){
    uint32_t tempUint =0 ;
    scd30 * scd = scd30::getInstance();
            scd->getReadyStatus();
        uint16_t redy = scd->scdSTR.ready;
        if(redy == scd30::SCDisReady) {
            uint8_t crcc = scd->readMeasurement();
            if(crcc != scd30::SCDnoERROR) return crcc;
            
            convertFloatToUint32(scd->scdSTR.tempf,  tempUint);
            return tempUint;
        }
    return 0;
}

uint32_t SensorSCD30::getHumid(){
       uint32_t humidUint =0 ;
    scd30 * scd = scd30::getInstance();
            scd->getReadyStatus();
        uint16_t redy = scd->scdSTR.ready;
        if(redy == scd30::SCDisReady) {
            uint8_t crcc = scd->readMeasurement();
            if(crcc != scd30::SCDnoERROR) return crcc;
            
            convertFloatToUint32(scd->scdSTR.humf,  humidUint);
            return humidUint;
        }
    return 0;
}
uint32_t SensorSCD30::getCo2(){
      uint32_t co2Uint =0 ;
    scd30 * scd = scd30::getInstance();
            scd->getReadyStatus();
        uint16_t redy = scd->scdSTR.ready;
        if(redy == scd30::SCDisReady) {
            uint8_t crcc = scd->readMeasurement();
            if(crcc != scd30::SCDnoERROR) return crcc;
            
            convertFloatToUint32(scd->scdSTR.co2f,  co2Uint);
            return co2Uint;
        }
    return 0;
}

int32_t SensorSCD30::init(){
     int ret =0;
        scd30 * scd = scd30::getInstance();
        if(scd->getInitialise() != true){
        ret += scd->softReset();
        ThisThread::sleep_for(2000);
        ret += scd->getSerialNumber();
        
        ret += scd->setMeasInterval(60);
        ret += scd->startMeasurement(0);
        scd->setInitialise();
        }
        return ret;
}
int32_t SensorSCD30::wakeUp(){
    return 0;
}
int32_t SensorSCD30::sleep(){

        return 0;
}