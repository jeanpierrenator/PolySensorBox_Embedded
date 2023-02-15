#pragma once
#include "mbed.h"
#include "SensorsLibrary/activeSensorInclude.h"
#include <cstdint>
#include <map>
#include "Periph/memory.h"
#include "thread_Params.h"


class SensorManager{

private: 


enum SensorType {
    TEMP = 1,
    HUMID =2,
    LUX = 3,
    ECO2 = 4,
    TOVC = 5,
    CO2 = 6,
    PRESSION = 7,
    UV = 8
};

enum SensorID {
    SCD30_ID = 1,
    BME280_ID =2,
    BME680_ID = 3,
    SI1145_ID = 4,
    VEML7700_ID = 5,
    SGP30_ID = 6,
    SCD41 = 7,
};


    //run pointer strategy
    void (SensorManager::*strategy_ptr)();

     std::map<uint8_t, uint32_t(SensorManager::*)()> sensorMap;
     std::map<uint8_t, SensorAbstract *> sensorAbstractMap;
     
    

    uint32_t getTemp();
    uint32_t getHumid();
    uint32_t getLux();
    uint32_t getECo2();
    uint32_t getTOVC();
    uint32_t getCo2();
    uint32_t getPression();
    uint32_t getUV();

    SensorAbstract * sensorTemp;
    SensorAbstract * sensorHumid;
    SensorAbstract * sensorLux;
    SensorAbstract * sensorECo2;
    SensorAbstract * sensorTOVC;
    SensorAbstract * sensorCo2;
    SensorAbstract * sensorPression;
    SensorAbstract * sensorUV;

    uint32_t periodTemp;
    uint32_t periodHumid;
    uint32_t periodLux;
    uint32_t periodECo2;
    uint32_t periodTOVC;
    uint32_t periodCo2;
    uint32_t periodPression;
    uint32_t periodUV;

    uint32_t getPhysicalForID(uint8_t idPhysical);

    SensorAbstract * getSensorWithID(uint8_t idsensor);
    void updateCurrentTime(uint32_t time);
    uint8_t getPhysicalForPeridodEqualToZero();
    uint32_t getNextSleepTime();
    uint32_t parseReadConfigFrame(const char * buffer);

    void run_lora_push();
    void run_lora_differ();
public: 
    SensorManager();
    static std::map<uint8_t, uint32_t> physicalPeriodeBaseMap;
    static std::map<uint8_t, uint32_t> physicalPeriodeCurrentMap;
    uint8_t init(uint8_t mode);
    void run();
};