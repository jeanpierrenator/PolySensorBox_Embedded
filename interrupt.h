#pragma once 
#include "mbed.h"
#include "SerialProtocol.h"
extern Thread interruptCallBackThread;
extern EventQueue interruptCallBackQueue;

void UartReceive_it(void);
