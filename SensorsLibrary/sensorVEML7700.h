#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>
#include "sensorDriver/VEML7700.h"
#include "../Periph/gpio.h"
class SensorVEML7700 : public SensorAbstract{
         public:
   uint32_t getLux() override;
   
      int32_t init() override;
    int32_t wakeUp()override;
    int32_t sleep()override;

};





