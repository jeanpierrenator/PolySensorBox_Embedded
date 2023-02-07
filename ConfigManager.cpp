#include "ConfigManager.h"
#include <cstdint>
#include <cstdio>
#include "Periph/rtc.h"


  std::map<uint8_t, uint32_t> ConfigManager::physicalPeriodeBaseMap;
   std::map<uint8_t, uint32_t> ConfigManager::physicalPeriodeCurrentMap;

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
        return new SensorAbstract();
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
    printf("sensorID %d \n",sensorID);
    printf("nbPhysical %d \n",nbPhysical);
    printf("nbline %d \n",nbline);
    if(nbPhysical >= 20){
        //bad config error
        return 1;
    }
    SensorAbstract * currentSensor = getSensorWithID(sensorID);
    currentSensor->setLine(nbline);
    if(nbPhysical != 0){
    for (int i = 2; i < (nbPhysical * 4) + 2;)
            {
                uint8_t idPhysical = buffer[i];
                int periode = (int)((buffer[i + 1]<<16) + (buffer[i + 2] << 8) + buffer[i + 3]);
                sensorAbstractMap[idPhysical] = currentSensor ;
                physicalPeriodeCurrentMap[idPhysical] = periode;
                physicalPeriodeBaseMap[idPhysical] = periode;
                i += 4;
            }
    }
    return 0;
}


 uint8_t ConfigManager::init( uint8_t mode){
     char receive[256];
    Memory::getInstance()->resetPointer();
    printf("pointer reset\n");
    do{
        Memory::getInstance()->readCurrentConfigFrame(receive);
        
        if(parseReadConfigFrame(receive)){
            return 1;
        }
       
    }while(Memory::getInstance()->setReadPointerToTheNextFrame() != -1);
    if(mode){
         strategy_ptr = &ConfigManager::run_lora_differ;
    }else {
         strategy_ptr = &ConfigManager::run_lora_push;
    }
    return 0;
   

 }
 void ConfigManager::run(){
     (this->*strategy_ptr)();
 }

void ConfigManager::run_lora_differ(){
     uint8_t currentPhysical = 0;
     uint32_t sleepTime =0;
     while(1){
         sleepTime = getNextSleepTime();
         ThisThread::sleep_for(std::chrono::seconds(sleepTime));
         updateCurrentTime(sleepTime);

         currentPhysical = getPhysicalForPeridodEqualToZero();
         while(currentPhysical != 0){
             uint32_t value =0;
             sensorAbstractMap[currentPhysical]->wakeUp();
             i2c_sem.acquire(); // Wait for the i2c to be available
             value = getPhysicalForID(currentPhysical);
             Memory::getInstance()->writeLogFrame(currentPhysical, value, RTC_getTime());
             i2c_sem.release(); // Release the i2c
             sensorAbstractMap[currentPhysical]->sleep();
             currentPhysical = getPhysicalForPeridodEqualToZero();
         }
         
     }
 }

 void ConfigManager::run_lora_push(){
     uint8_t currentPhysical = 0;
     uint32_t sleepTime =0;
     uint32_t currentIndex =0;
     while(1){
         sleepTime = getNextSleepTime();

         loraSend_mut.lock();
         loraSend_mut.unlock();

         loraReady_mut.lock();

         
         ThisThread::sleep_for(std::chrono::seconds(sleepTime));
         sizeToSendLora =0;
         memset(ram_sendBuffer, 0, sizeof(ram_sendBuffer));
         currentIndex = 0;
         updateCurrentTime(sleepTime);

         currentPhysical = getPhysicalForPeridodEqualToZero();
         while(currentPhysical != 0){
             uint32_t value =0;
             sensorAbstractMap[currentPhysical]->wakeUp();
             i2c_sem.acquire(); // Wait for the i2c to be available
             value = getPhysicalForID(currentPhysical);
             i2c_sem.release(); // Release the i2c
             sensorAbstractMap[currentPhysical]->sleep();
             ram_sendBuffer[currentIndex++] = currentPhysical;
             //put uint32_t into send buffer
            for (int i = 0; i < 4; i++) {
                ram_sendBuffer[currentIndex++] = (value >> (8 * i)) & 0xFF;
            }
            currentPhysical = getPhysicalForPeridodEqualToZero();
            sizeToSendLora += 5;
         }
        loraReady_mut.unlock();
        printf("signal set\n");
         
     }
 }
