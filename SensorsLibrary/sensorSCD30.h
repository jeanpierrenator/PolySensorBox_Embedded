#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>
#include "sensorDriver/scd30.h"

class SensorSCD30 : public SensorAbstract{
         public:
   uint32_t getTemp() override;
    uint32_t getHumid() override;
    uint32_t getCo2() override;

    int32_t init() override;
    int32_t wakeUp()override;
    int32_t sleep()override;
};





