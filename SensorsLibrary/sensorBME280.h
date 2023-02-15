#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>
#include "sensorDriver/BME280.h"
#include "../Periph/gpio.h"

class SensorBME280 : public SensorAbstract{
         public:
   uint32_t getTemp() override;
    uint32_t getHumid() override;
    uint32_t getPression() override;

    int32_t init() override;
    int32_t wakeUp()override;
    int32_t sleep()override;
};





