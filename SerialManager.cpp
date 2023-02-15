#include "SerialManager.h"

SerialManager* SerialManager::_instance= nullptr;
int32_t SerialManager::write(const void * Buffer, int32_t size)
{
 
 return serial_port->write(Buffer, size); 
}

int32_t SerialManager::read( void * Buffer, int32_t size)
{
 return serial_port->read(Buffer, size);
}

SerialManager * SerialManager::getinstance(){
    if(_instance == nullptr){
        _instance = new SerialManager();
    }
    return _instance;
}

