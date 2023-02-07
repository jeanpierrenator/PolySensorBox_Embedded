#include "errorSignal.h"

void memoryFaultError(){
     DigitalOut led(errorLed);
     while (1) {
      led.write(1);
      ThisThread::sleep_for(memoryFaultErrorDelay);
      led.write(0);
      ThisThread::sleep_for(memoryFaultErrorDelay);
     }
        
}

void configurationFaultError(){
     DigitalOut led(errorLed);
     while (1) {
      led.write(1);
      ThisThread::sleep_for(configurationFaultErrorDelay);
      led.write(0);
      ThisThread::sleep_for(configurationFaultErrorDelay);
     }
}