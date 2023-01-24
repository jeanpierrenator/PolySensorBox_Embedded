#include "memory.h"
#include <cstdint>
Memory* Memory::instance= nullptr;
Memory::Memory(I2C * i2c)
{
      eeprom = new _24LCXXX(i2c, I2C_ADDR_24LCXXX );
}



int Memory::writeConfigFrame(const char *frame){
    int32_t size  =0;
    char offsetToWrite[1];

    readBytes(offsetToWrite,1,CONFIG_START_ADRESS);

    size = getFrame2Size(frame);
    if (size != -1){
        if(writeBytes(frame , size, CONFIG_START_ADRESS+offsetToWrite[0]+1)== -1){
            return -2;
        }
        offsetToWrite[0]+=size;
        writeBytes(offsetToWrite, 1, CONFIG_START_ADRESS);
    }else{
        return -1;
    }
    return 0;
}
int Memory::writeLogFrame(uint8_t id, uint32_t value, uint32_t timestamp){
    int32_t size  =0;
    char offsetToWrite[2];
    char dataframe[9];
    uint16_t offsetToWriteuint =0;
    readBytes(offsetToWrite,2,LOG_START_ADRESS);
    offsetToWriteuint = (offsetToWrite[0]<<8) + offsetToWrite[1];
    
    dataframe[0] = id;
    dataframe[1] = (value >> 24) & 0xFF;
    dataframe[2] = (value >> 16) & 0xFF;
    dataframe[3] = (value >> 8) & 0xFF;
    dataframe[4] = value & 0xFF;
    dataframe[5] = (timestamp >> 24) & 0xFF;
    dataframe[6] = (timestamp >> 16) & 0xFF;
    dataframe[7] = (timestamp >> 8) & 0xFF;
    dataframe[8] = timestamp & 0xFF;

    writeBytes(dataframe , 9, LOG_START_ADRESS+offsetToWriteuint+2);
    offsetToWriteuint += 9;
    offsetToWrite[0] = (offsetToWriteuint >> 8) & 0xFF;
    offsetToWrite[1] = offsetToWriteuint & 0xFF;
    writeBytes(offsetToWrite, 2, LOG_START_ADRESS);
    return 0;
}



int Memory::readAllConfig(const char *config){
    char sizeToRead[1];

    readBytes(sizeToRead,1,CONFIG_START_ADRESS);
    readBytes(config, sizeToRead[0], CONFIG_START_ADRESS+1);

    return sizeToRead[0];
}

int Memory::readCurrentConfigFrame(const char *config){
    char getSize[2] ={0};
    int size = 0;
    char currentSize[1] = {0};
    readBytes(currentSize,1,CONFIG_START_ADRESS);

     if(readConfigOffset >= currentSize[0] ){
         // fin de la memoire atteinte
        return -1;
    }
    readBytes(getSize, 2, CONFIG_START_ADRESS+1+readConfigOffset);
     size = getFrame2Size(getSize);
    if(size != -1){
        readBytes(config, size, CONFIG_START_ADRESS+1+readConfigOffset);
    }else{
        return -1;
    }
    return size;   
}

int Memory::setReadPointerToTheNextFrame(){
    char getSize[2] ={0};
    char currentSize[1] = {0};
    int size = 0;
    readBytes(currentSize,1,CONFIG_START_ADRESS);
    readBytes(getSize, 2, CONFIG_START_ADRESS+1+readConfigOffset);
    size = getFrame2Size(getSize);
    if(size != -1){
        readConfigOffset += size;
    }else{
        return -1;
    }
    if(readConfigOffset >= currentSize[0] ){
        return -1;
    }
  return 0;  
}

int Memory::resetPointer(){
    readConfigOffset = 0;
    return 0;
}


int Memory::writeBytes(const char * data ,uint8_t size, uint32_t address ){
   return eeprom->nbyte_write(address, (void *)data,  size);
}

int Memory::readBytes(const char * data ,uint8_t size, uint32_t address){
    return eeprom->nbyte_read(address,(void *)data,  size);
}

int Memory::getFrame2Size(const char *frame){
    if(sizeof(frame)>=1){
        // compute size of drame 2 
        return (((uint8_t)frame[1] & 0b00111111)*4)+2;
    }
    return -1;

}
int Memory::clearConfig(){
    char reset = 0;
    return writeBytes(&reset, 1, CONFIG_START_ADRESS);
    
}

Memory * Memory::getInstance(){
    if(instance == nullptr ){
        return nullptr;
    }
    return instance;
}

Memory * Memory::getInstance(I2C * i2c){
    if(instance == nullptr ){
        instance = new Memory(i2c);
    }
    return instance;
}