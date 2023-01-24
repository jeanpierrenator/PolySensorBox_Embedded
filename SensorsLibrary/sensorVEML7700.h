#pragma once 
#include "SensorAbstract.h"
#include <cstdint>
#include <cstdio>

class SensorVEML7700 : public SensorAbstract{
         public:
   uint32_t getLux() override;

};





