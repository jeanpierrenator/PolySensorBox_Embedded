#include "interrupt.h"

Ticker timePushButton;
//UartReceivCallBack
void UartReceive_it(void)
{
    interruptCallBackQueue.call(SerialProtocol::onSerialReceived);
}


/**
 * Capte un front montant sur le bouton reset lora
 */
void handlerRiseBpReset()
{
    timePushButton.detach();
}

/**
 * Capte un front descendant sur le bouton reset lora
 */
void handlerFallBpReset()
{
    timePushButton.attach(&LoraInterface_resetLora, chrono::milliseconds(TIME_BUTTON_RESET_PRESSED));
}


/**
 * Capte un front montant sur le bouton etat lora
 */
void handlerRiseBpTest()
{
    timePushButton.detach();
}


/**
 * Capte un front descendant sur le bouton etat lora
 */
void handlerFallBpTest()
{
    timePushButton.attach(&LoraInterface_connectedTest, TIME_BUTTON_STATE_PRESSED);
}