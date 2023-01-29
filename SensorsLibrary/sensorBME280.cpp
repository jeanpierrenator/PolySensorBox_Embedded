#include "sensorBME280.h"
#include <cstdint>
#include <cstdio>

uint32_t SensorBME280::getTemp(){
    
    float temp;
    uint32_t tempUint;
        int ret;
        // Measurment time = 1,25 + (2,3xt_OS) + (2,3xP_OS + 0,575) + (2,3xH_OS + 0,575) = 9,3ms -> 10ms
        ThisThread::sleep_for(100ms);
        ret = BME280::getInstance()->getTemperature(&temp);
    printf("temp\n");
    
    convertFloatToUint32(temp,  tempUint);
    return tempUint;
}

uint32_t SensorBME280::getHumid(){
       // retreve the value after the measurment (values already calculated)
        float humid;
        uint32_t humidUint;
        int ret;
        printf("humid\n");
        // Measurment time = 1,25 + (2,3xt_OS) + (2,3xP_OS + 0,575) + (2,3xH_OS + 0,575) = 9,3ms -> 10ms
        ThisThread::sleep_for(10ms);
        ret = BME280::getInstance()->getHumidity(&humid);
        convertFloatToUint32(humid,  humidUint);

    return humidUint;
}
uint32_t SensorBME280::getPression(){
         // retreve the value after the measurment (values already calculated)
        float press;
        uint32_t pressUint;
        int ret;
        // Measurment time = 1,25 + (2,3xt_OS) + (2,3xP_OS + 0,575) + (2,3xH_OS + 0,575) = 9,3ms -> 10ms
        ThisThread::sleep_for(10ms);
        ret = BME280::getInstance()->getPressure(&press);
        convertFloatToUint32(press,  pressUint);
         printf("press\n");
        return pressUint;
       
}

int32_t SensorBME280::init(){
      gpio_setAlim_on(this->getLine());
      BME280::getInstance()->initialize();
      BME280::getInstance()->setForcedMode();
 return 0;
}
int32_t SensorBME280::wakeUp(){
      gpio_setAlim_on(this->getLine());
      BME280::getInstance()->initialize();
      BME280::getInstance()->setForcedMode();
 return 0;
}
int32_t SensorBME280::sleep(){
      gpio_setAlim_off(this->getLine());
 return 0;
}
