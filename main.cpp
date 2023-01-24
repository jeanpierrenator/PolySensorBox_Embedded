#include "mbed.h"
#include "Callback.h"
#include "interrupt.h"
#include "SerialManager.h"
#include "SerialProtocol.h"
#include "memory.h"
#include "ConfigManager.h"
#include "LoraApp/MyLoraInterface.h"


#define BUFF_LEN    32
#define MSG_LEN     64
#define DATA_LEN    MSG_LEN - 2

Thread interruptCallBackThread;
Thread sensorThread;
Thread loraThread;
ConfigManager configManager;
EventQueue interruptCallBackQueue(32 * EVENTS_EVENT_SIZE);

I2C i2c(PA_11,PA_12);












/**
 * Entry point for application
 */
int main()
{
  
    char receive[256]={0};
    /* init memory */
    i2c.start();
    i2c.frequency(400000);
    Memory * mem = Memory::getInstance(&i2c);
    /* end init memory*/ 
    
    int size  =0;
    /*mem->resetPointer();
    do{
        size = mem->readCurrentConfigFrame(receive);
        for(int i =0 ; i < size; i++){
            printf("%c", receive[i]);
        }
        printf("\n");
    }while(mem->setReadPointerToTheNextFrame() != -1);*/
    SerialProtocole::init();
    interruptCallBackThread.start(callback(&interruptCallBackQueue, &EventQueue::dispatch_forever));
    SerialManager::getinstance()->setCallbackFunction(callback(UartReceive_it));


    

    configManager.init();
    sensorThread.start(callback(&configManager, &ConfigManager::run));

    LoraInterface_init(20);
    loraThread.start(callback(LoraInterface_run));

    while (1) {
    ThisThread::sleep_for(10000s);
    }
    

}




