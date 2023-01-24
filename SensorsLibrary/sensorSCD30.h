#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>

class SensorSCD30 : public SensorAbstract{
         public:
   uint32_t getTemp() override;
    uint32_t getHumid() override;
    uint32_t getCo2() override;
};





