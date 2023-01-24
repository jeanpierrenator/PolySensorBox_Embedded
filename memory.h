#pragma once
#include "eeprom.h"
#include <cstdint>

/* macro -------------------------------------------------------------------- */
#define  CONFIG_START_ADRESS 256
#define  LOG_START_ADRESS 1024
/* variables ---------------------------------------------------------------- */
/* class -------------------------------------------------------------------- */

class Memory
{
public:
    // Constructor
    Memory(I2C * i2c);

    // Write methods
    int writeConfigFrame(const char * frame);
    int writeLogFrame(uint8_t id, uint32_t value, uint32_t timestamp);

    // Read methods
    int readAllConfig(const char * config);
    int readCurrentConfigFrame(const char * config);
    int setReadPointerToTheNextFrame();
    int resetPointer();

    // Utility methods
    int clearConfig();
    int getReadConfigOffset();
    static Memory * getInstance();
    static Memory * getInstance(I2C * i2c);

private:
    // Variables
    _24LCXXX * eeprom;
    int readConfigOffset;

    // Helper methods
    int readBytes(const char * data ,uint8_t size, uint32_t address);
    int writeBytes(const char * data ,uint8_t size, uint32_t address );
    int getFrame2Size(const char * frame);

    // Singleton implementation
    static Memory * instance;
};