#pragma once
#include "mbed.h"
#include "BufferedSerial.h"

#define APP_UART_TX  USBTX
#define APP_UART_RX  USBRX
class SerialManager
{

private:
    static constexpr int APP_BAUD_RATE = 9600;
    static SerialManager * _instance;

    BufferedSerial * serial_port;
       SerialManager() {
       serial_port = new BufferedSerial(APP_UART_TX, APP_UART_RX);
        serial_port->set_baud(APP_BAUD_RATE);
        serial_port->set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );
    };
    
    
public:


 


    static SerialManager * getinstance();
    int write(const void * Buffer, int size);
    int read(void * Buffer, int size);
    BufferedSerial * getSerial(){
        return serial_port;
    }

    void setCallbackFunction(Callback<void ()> func){
        serial_port->sigio(func);
    }

    
};



