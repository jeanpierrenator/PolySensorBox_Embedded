#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>
#include "sensorDriver/Adafruit_SGP30.h"

class SensorSGP30 : public SensorAbstract{
         public:
    uint32_t getTOVC() override;
     uint32_t getECo2() override;

      int32_t init() override;
    int32_t wakeUp()override;
    int32_t sleep()override;
};





