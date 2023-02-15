#include "errorSignal.h"


/*memory fault error indicator 
led blink 500ms
*/
 
void memoryFaultError(){
     DigitalOut led(errorLed);
     while (1) {
      led.write(1);
      ThisThread::sleep_for(memoryFaultErrorDelay);
      led.write(0);
      ThisThread::sleep_for(memoryFaultErrorDelay);
     }
        
}


/*configuration error indicator 
led blink 100ms
*/
void configurationFaultError(){
     DigitalOut led(errorLed);
     while (1) {
      led.write(1);
      ThisThread::sleep_for(configurationFaultErrorDelay);
      led.write(0);
      ThisThread::sleep_for(configurationFaultErrorDelay);
     }
}

void loraDisconnectedError(){
    DigitalOut led(errorLed);
    uint8_t i = 0;
     while (i< loraNoErrorDelay/(loraDisconnectedErrorDelay*2) ) {
      led.write(1);
      ThisThread::sleep_for(loraDisconnectedErrorDelay);
      led.write(0);
      ThisThread::sleep_for(loraDisconnectedErrorDelay);
      i++;
     }
}
void loraNoError(){
        DigitalOut led(errorLed);
        ThisThread::sleep_for(loraNoErrorDelay);
        led.write(0);

}