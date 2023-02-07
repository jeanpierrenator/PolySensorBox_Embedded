#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include "sensorDriver/SI1145.h"
#include "../Periph/gpio.h"

class SensorSI1145 : public SensorAbstract{
         public:
   uint32_t getUV() override;
   
      int32_t init() override;
    int32_t wakeUp()override;
    int32_t sleep()override;

};





