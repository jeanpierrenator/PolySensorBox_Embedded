#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>

class SensorSGP30 : public SensorAbstract{
         public:
    uint32_t getTOVC() override;
     uint32_t getECo2() override;
};





