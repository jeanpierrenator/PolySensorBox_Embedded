#pragma once
#include "mbed.h"
#include "BufferedSerial.h"

#define APP_UART_TX  USBTX
#define APP_UART_RX  USBRX

// Declaration of the SerialManager class
class SerialManager
{

private:
    // Constants
    static constexpr int32_t APP_BAUD_RATE = 9600;

    // Static variable to hold the instance of the class
    static SerialManager * _instance;

    // BufferedSerial object for serial communication
    BufferedSerial * serial_port;

    // Private constructor
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
    // Declaration of public member functions

    // Returns the instance of the SerialManager class
    static SerialManager * getinstance();

    // Writes the specified data to the serial port
    int32_t write(const void * Buffer, int32_t size);

    // Reads the specified amount of data from the serial port
    int32_t read(void * Buffer, int32_t size);

    // Returns the BufferedSerial object used for serial communication
    BufferedSerial * getSerial() {
        return serial_port;
    }

    // Sets the callback function to be called when serial data is received
    void setCallbackFunction(Callback<void ()> func){
        serial_port->sigio(func);
    }
};




