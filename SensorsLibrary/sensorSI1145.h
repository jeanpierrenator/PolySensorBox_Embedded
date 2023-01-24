#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>

class SensorSI1145 : public SensorAbstract{
         public:
   uint32_t getUV() override;

};





