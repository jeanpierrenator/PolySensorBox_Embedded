#include "SerialManager.h"

SerialManager* SerialManager::_instance= nullptr;
int SerialManager::write(const void * Buffer, int size)
{
 
 return serial_port->write(Buffer, size); 
}

int SerialManager::read( void * Buffer, int size)
{
 return serial_port->read(Buffer, size);
}

SerialManager * SerialManager::getinstance(){
    if(_instance == nullptr){
        _instance = new SerialManager();
    }
    return _instance;
}

