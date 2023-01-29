#include "mbed.h"
#include "thread_Params.h"
#include "Callback.h"
#include "interrupt.h"
#include "SerialManager.h"
#include "SerialProtocol.h"
#include "memory.h"
#include "ConfigManager.h"
#include "LoraApp/MyLoraInterface.h"
#include "Periph/rtc.h"
#include <cstdint>
#include <cstdio>


#define BUFF_LEN    32
#define MSG_LEN     64
#define DATA_LEN    MSG_LEN - 2


ConfigManager configManager;
Thread interruptUartCallBackThread;
Thread sensorThread;
Thread loraThread;
EventQueue interruptCallBackQueue(32 * EVENTS_EVENT_SIZE);
Mutex loraReady_mut;
Mutex loraSend_mut;

uint8_t ram_sendBuffer[256];
uint8_t sizeToSendLora =0;
I2C i2c(PA_11,PA_12);












/**
 * Entry point for application
 */
int main()
{
  
    char receive[256]={0};
    char appUUID[8] ={0};
    char appKey[16] = {0};
    char devUUID[8] = {0};
    uint32_t loraPeriod = 0;
    /* init memory */
    i2c.start();
    i2c.frequency(400000);
    Memory * mem = Memory::getInstance(&i2c);
    mem->readLoraConfig(appKey, appUUID, devUUID, &loraPeriod);
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
    interruptUartCallBackThread.start(callback(&interruptCallBackQueue, &EventQueue::dispatch_forever));
    SerialManager::getinstance()->setCallbackFunction(callback(UartReceive_it));

    printf("init : %d\n",RTC_init());
   RTC_setTime(1674917150);
    

      configManager.init();
      sensorThread.start(callback(&configManager, &ConfigManager::run));

      LoraInterface_init(appKey,appUUID,devUUID,loraPeriod);
      loraThread.start(callback(LoraInterface_run_when_ready));

    while (1) {
    ThisThread::sleep_for(5s);
    printf("%d\n", RTC_getTime() );
    }
    

}




