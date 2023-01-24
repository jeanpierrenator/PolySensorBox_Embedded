#include "ConfigManager.h"
#include <cstdint>


ConfigManager::ConfigManager(){
    sensorMap[TEMP] = &ConfigManager::getTemp;
    sensorMap[HUMID] = &ConfigManager::getHumid;
    sensorMap[LUX] = &ConfigManager::getLux;
    sensorMap[ECO2] = &ConfigManager::getECo2;
    sensorMap[TOVC] = &ConfigManager::getTOVC;
    sensorMap[CO2] = &ConfigManager::getCo2;
    sensorMap[PRESSION] = &ConfigManager::getPression;
    sensorMap[UV] = &ConfigManager::getUV;


    
}

uint32_t ConfigManager::getPhysicalForID(uint8_t idPhysical){
    
    auto it = sensorMap.find(idPhysical);
    if (it != sensorMap.end()) {
        return (this->*(it->second))();
    } else {
        return 0;
    }
}

SensorAbstract * ConfigManager::getSensorWithID(uint8_t idsensor){
    switch (idsensor) {

    case SCD30_ID:
        return new SensorSCD30();
        break;
    case BME280_ID:
         return new SensorBME280();
        break;
    case BME680_ID:
        return new SensorBME680();
        break;
    case SI1145_ID:
        return new SensorSI1145();
        break;
    case VEML7700_ID:
        return new SensorVEML7700();
        break;
    case SGP30_ID:
        return new SensorSGP30();
        break;
    default:
        return nullptr;
        break;
}
return nullptr;
}


uint32_t ConfigManager::getTemp() {
    return sensorAbstractMap[TEMP]->getTemp();
}
uint32_t ConfigManager::getHumid() {
    return sensorAbstractMap[HUMID]->getHumid();
}
uint32_t ConfigManager::getLux() {
    return sensorAbstractMap[LUX]->getLux();
}
uint32_t ConfigManager::getECo2() {
    return sensorAbstractMap[ECO2]->getECo2();
}
uint32_t ConfigManager::getTOVC() {
    return sensorAbstractMap[TOVC]->getTOVC();
}
uint32_t ConfigManager::getCo2() {
    return sensorAbstractMap[CO2]->getCo2();
}
uint32_t ConfigManager::getPression() {
    return sensorAbstractMap[PRESSION]->getPression();
}
uint32_t ConfigManager::getUV() {
    return sensorAbstractMap[UV]->getUV();
}


uint32_t ConfigManager::getNextSleepTime() {
    auto it = std::min_element(physicalPeriodeCurrentMap.begin(), physicalPeriodeCurrentMap.end(),
    [](auto const &lhs, auto const &rhs){
        return lhs.second < rhs.second;
    });
    return it->second;
}

void ConfigManager::updateCurrentTime(uint32_t time) {
    for (auto &element : physicalPeriodeCurrentMap) {
        element.second -= time;
    }
}

uint8_t ConfigManager::getPhysicalForPeridodEqualToZero() {
    for (auto  &element : physicalPeriodeCurrentMap) {
        if (element.second <= 0) {
            element.second = physicalPeriodeBaseMap[element.first];
            return element.first;
        }
    }
    //if no element has a value of 0, return a default value
    return 0;
}



    
uint32_t ConfigManager::parseReadConfigFrame(const char * buffer){
    uint8_t sensorID = buffer[0];
    uint8_t nbPhysical = buffer[1] & 0b111111;
    uint8_t nbline = ((buffer[1]>>6) & 0b11);
    SensorAbstract * currentSensor = getSensorWithID(sensorID);
    currentSensor->setLine(nbline);

    for (int i = 2; i < (nbPhysical * 4) + 2;)
            {
                uint8_t idPhysical = buffer[i];
                int periode = (int)((buffer[i + 1]<<16) + (buffer[i + 2] << 8) + buffer[i + 3]);
                sensorAbstractMap[idPhysical] = currentSensor ;
                physicalPeriodeCurrentMap[idPhysical] = periode;
                physicalPeriodeBaseMap[idPhysical] = periode;
                i += 4;
            }
    return 0;
}


 void ConfigManager::init(){
     char receive[256];
    Memory::getInstance()->resetPointer();
    do{
        Memory::getInstance()->readCurrentConfigFrame(receive);
        parseReadConfigFrame(receive);
    }while(Memory::getInstance()->setReadPointerToTheNextFrame() != -1);


 }
 void ConfigManager::run(){
     uint8_t currentPhysical = 0;
     uint32_t sleepTime =0;
     while(1){
         sleepTime = getNextSleepTime();
         ThisThread::sleep_for(std::chrono::seconds(sleepTime));
         updateCurrentTime(sleepTime);
         currentPhysical = getPhysicalForPeridodEqualToZero();
         while(currentPhysical != 0){
             getPhysicalForID(currentPhysical);
             currentPhysical = getPhysicalForPeridodEqualToZero();
         }
     }
 }

