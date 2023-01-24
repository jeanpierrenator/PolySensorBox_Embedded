#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>

class SensorBME280 : public SensorAbstract{
         public:
   uint32_t getTemp() override;
    uint32_t getHumid() override;
    uint32_t getPression() override;
     uint32_t getECo2() override;
};





