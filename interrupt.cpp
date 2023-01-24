#include "interrupt.h"

void UartReceive_it(void)
{
    interruptCallBackQueue.call(SerialProtocole::onSerialReceived);
}
