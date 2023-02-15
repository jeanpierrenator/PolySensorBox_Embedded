#include "mbed.h"  // Include the Mbed library
#include "thread_Params.h"  // Include thread parameters library
#include "Callback.h"  // Include callback library
#include "interrupt.h"  // Include interrupt library
#include "SerialManager.h"  // Include SerialManager library
#include "SerialProtocol.h"  // Include SerialProtocol library
#include "SensorManager.h"  // Include SensorManager library
#include "LoraApp/MyLoraInterface.h"  // Include MyLoraInterface library
#include "Periph/rtc.h"  // Include real-time clock library
#include "Periph/gpio.h"  // Include general-purpose I/O library
#include "Periph/memory.h"  // Include memory library
#include "errorSignal.h"  // Include errorSignal library
#include <cstdint>  // Include C++ standard integer types
#include <cstdio>  // Include C standard input/output functions



// Initialize objects and variables
SensorManager sensorManager;  // SensorManager object
Thread interruptUartCallBackThread;  // Interrupt UART callback thread
Thread sensorThread;  // Sensor thread
Thread loraThread;  // LoRa thread
EventQueue interruptCallBackQueue(32 * EVENTS_EVENT_SIZE);  // Interrupt callback queue
Mutex loraReady_mut;  // LoRa ready mutex
Mutex loraSend_mut;  // LoRa send mutex
uint8_t ram_sendBuffer[256];  // RAM send buffer
uint8_t sizeToSendLora = 0;  // Size to send to LoRa
I2C i2c(PA_11,PA_12);  // I2C object


InterruptIn bpResetLora(PA_5); //bp reset lora 
InterruptIn bpTestLora(PA_6); // bp test lora




// Entry point for application
int main()
{
    uint8_t mode = 0;  // Lora Mode 0 : send periodic
    char receive[256] = {0};  // Receive buffer
    char appUUID[8] = {0};  // App UUID
    char appKey[16] = {0};  // App key
    char devUUID[8] = {0};  // Device UUID
    uint32_t loraPeriod = 0;  // LoRa period

    /* innitialize BP Lora*/
    bpResetLora.mode(PullUp);
    bpResetLora.rise(&handlerRiseBpReset);
    bpResetLora.fall(&handlerFallBpReset);
    
    bpTestLora.mode(PullUp);
    bpTestLora.rise(&handlerRiseBpTest);
    bpTestLora.fall(&handlerFallBpTest);

    /* Initialize memory */
    i2c.start();  // initialise I2C communication
    i2c.frequency(400000);  // Set I2C frequency

    gpio_setAlim_on(1);  // Set power supply for line 1
    gpio_setAlim_on(2);  // Set power supply for line 2
    gpio_setAlim_on(3);  // Set power supply for line 3

    Memory * mem = Memory::getInstance(&i2c);  // Get instance of Memory class

    mem->clearLogs();  // Clear logs in memory

    // Read in LoRa configuration values from memory
    if (mem->readLoraConfig(appKey, appUUID, devUUID, &loraPeriod) == -1) {
        memoryFaultError();  // Handle memory fault error
    }

    if (loraPeriod > 0) {
        mode = 1;  // Set mode to 1 :Push when ready
    }

    /* End memory initialization */


 


    SerialProtocol::init();  // Initialize SerialProtocol
    interruptUartCallBackThread.start(callback(&interruptCallBackQueue, &EventQueue::dispatch_forever));  // Start
    SerialManager::getinstance()->setCallbackFunction(callback(UartReceive_it));//set

        // init lora
      LoraInterface_init((uint8_t *)appKey,(uint8_t *)appUUID,(uint8_t *)devUUID,loraPeriod);
      if(mode == 0){
          loraThread.start(callback(LoraInterface_run_when_ready));
      }else {
        loraThread.start(callback(LoraInterface_run_timed));
      }
    
    // init SensorManager
      if(sensorManager.init(mode)){
        configurationFaultError();
      }
      sensorThread.start(callback(&sensorManager, &SensorManager::run));
      
   
      
    
    

}




