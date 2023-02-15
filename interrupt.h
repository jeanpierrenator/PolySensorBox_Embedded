#pragma once 
#include "mbed.h"
#include "SerialProtocol.h"
#include "MyLoraInterface.h"
extern Thread interruptCallBackThread;
extern EventQueue interruptCallBackQueue;

constexpr uint16_t TIME_BUTTON_RESET_PRESSED = 3000;
constexpr uint16_t TIME_BUTTON_STATE_PRESSED = 5000;

void UartReceive_it(void);


/**
 * Capte un front montant sur le bouton reset lora
 */
void handlerRiseBpReset();

/**
 * Capte un front descendant sur le bouton reset lora
 */
void handlerFallBpReset();

/**
 * Capte un front montant sur le bouton etat lora
 */
void handlerRiseBpTest();

/**
 * Capte un front descendant sur le bouton etat lora
 */
void handlerFallBpTest();
