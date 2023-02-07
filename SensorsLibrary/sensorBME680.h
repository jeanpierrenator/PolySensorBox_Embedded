#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>
#include "sensorDriver/mbed_bme680.h"

class SensorBME680 : public SensorAbstract{
         public:
   uint32_t getTemp() override;
    uint32_t getHumid() override;
    uint32_t getPression() override;
     uint32_t getECo2() override;

    int32_t init() override;
    int32_t wakeUp()override;
    int32_t sleep()override;
};





