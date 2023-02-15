#include "sensorBME680.h"
#include <cstdint>

uint32_t SensorBME680::getTemp(){
    uint32_t tempUint32 =0;
    float temp = BME680::getInstance()->getTemperature();
    convertFloatToUint32(temp,tempUint32);
    return tempUint32;
}

uint32_t SensorBME680::getHumid(){
        // retreve the value after the measurment (values already calculated)
        uint32_t humidUint32 =0;
        float humid = BME680::getInstance()->getHumidity();
        convertFloatToUint32(humid,humidUint32);
    return 0;
}
uint32_t SensorBME680::getPression(){
   // retreve the value after the measurment (values already calculated)
        uint32_t pressUint32 =0;
        float press = BME680::getInstance()->getPressure()/100;
        convertFloatToUint32(press,  pressUint32);
        return pressUint32;
}
uint32_t SensorBME680::getECo2(){
// error
 return 0;
}

int32_t SensorBME680::init(){
   return BME680::getInstance()->begin();
}
int32_t SensorBME680::wakeUp(){
   // wake the sensore and read the environement and do to sleep
    // the wait for measurment buit-in
    // true = 0K / false = not good
        return BME680::getInstance()->performReading();
}
int32_t SensorBME680::sleep(){
        //already done in the performReading method (after the measurments)
        return 0;
}
