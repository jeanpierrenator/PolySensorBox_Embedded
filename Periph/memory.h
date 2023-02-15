#pragma once
#include "eeprom.h"
#include <cstdint>

/* macro -------------------------------------------------------------------- */
#define  CONFIG_START_ADRESS 256
#define  LOG_START_ADRESS 1024
#define LORA_START_ADRESS 0

/* variables ---------------------------------------------------------------- */
/* class -------------------------------------------------------------------- */

class Memory
{
public:
    // Constructor
    Memory(I2C * i2c);

    // Write methods
    int32_t writeConfigFrame(const char * frame);
    int32_t writeLogFrame(uint8_t id, uint32_t value, uint32_t timestamp);
    int32_t writeLoraData(const char * frame);

    // Read methods
    int32_t readAllConfig(const char * config);
    int32_t readLoraConfig(char * appKey,char * appUUID,char * devUUID,uint32_t * LoraPeriod);
    int32_t readCurrentConfigFrame(const char * config);
    int32_t readAllLogFrameFrom(uint16_t start_address, char * logs);
    int32_t setReadPointerToTheNextFrame();
    int32_t resetPointer();

    // Utility methods
    int32_t clearConfig();
    int32_t clearLogs();
    uint16_t getCurrentLogPointer();
    int32_t getReadConfigOffset();
    static Memory * getInstance();
    static Memory * getInstance(I2C * i2c);

private:
    // Variables
    _24LCXXX * eeprom;
    int32_t readConfigOffset;

    // Helper methods
    int32_t readBytes(const char * data ,uint8_t size, uint32_t address);
    int32_t writeBytes(const char * data ,uint8_t size, uint32_t address );
    int32_t getFrame2Size(const char * frame);

    // Singleton implementation
    static Memory * instance;
};