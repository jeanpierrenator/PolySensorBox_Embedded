#pragma once
#include "mbed.h"
#include "Thread.h"
#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"
#include"../Periph/rtc.h"
#include "../thread_Params.h"
#include "../SensorManager.h"
#include "../errorSignal.h"
#include <cstring>


#include <cstdint>

/**
 * Maximum number of retries for CONFIRMED messages before giving up
 */
#define CONFIRMED_MSG_RETRY_COUNTER     3

    // Max payload size can be LORAMAC_PHY_MAXPAYLOAD.
// This example only communicates with much shorter messages (<30 bytes).
// If longer messages are used, these buffers must be changed accordingly.
static    uint8_t tx_buffer[LORAMAC_PHY_MAXPAYLOAD];
static    uint8_t rx_buffer[LORAMAC_PHY_MAXPAYLOAD];

/**
 * Constructing Mbed LoRaWANInterface and passing it the radio object from lora_radio_helper.
 */
static LoRaWANInterface * lorawan;
/**
* Application specific callbacks
*/
static lorawan_app_callbacks_t callbacks;
    /**
 * Event handler.
 *
 * This will be passed to the LoRaWAN stack to queue events for the
 * application which in turn drive the application.
 */



static void LoraInterface_lora_event_handler(lorawan_event_t event);
static void LoraInterface_sendMessage();
static void LoraInterface_receiveMessage();

void LoraInterface_connectedTest();
void LoraInterface_resetLora();


int8_t LoraInterface_init(uint8_t * APPKEY,  uint8_t * APPUUID, uint8_t * DevUUID,uint32_t sendPeriod);
void LoraInterface_run_timed();
void LoraInterface_run_when_ready();

    

