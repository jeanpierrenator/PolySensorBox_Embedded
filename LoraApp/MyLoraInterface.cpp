#include "MyLoraInterface.h"
// Application helpers
#include "trace_helper.h"
#include "lora_radio_helper.h"
#include <chrono>
#include <cstdio>

static EventQueue Lora_ev_queue(10*EVENTS_EVENT_SIZE);
bool connected =0;
uint32_t period = 0;
bool messageEndTransmit = false;
bool receiveMessage = false;


int8_t LoraInterface_init(uint32_t sendPeriod){
    lorawan = new LoRaWANInterface(radio);
    period = sendPeriod;
     // setup tracing
    setup_trace();
     lorawan_connect_t connect_params;
    unsigned char DevUUID[8] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x9B, 0x52 };
    unsigned char APPUUID[8] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x04, 0xE3, 0xA7 };
    unsigned char APP_KEY[16] = { 0xB3,0xF8,0x72,0xEC,0xE5,0x22,0xCE,0xEF,0xDB,0xD4,0x4F,0xC2,0xE5,0xC3,0xAB,0x13 };
        //OTAA:
    connect_params.connect_type = LORAWAN_CONNECTION_OTAA;
    connect_params.connection_u.otaa.dev_eui = DevUUID;
    connect_params.connection_u.otaa.app_eui = APPUUID;
    connect_params.connection_u.otaa.app_key = APP_KEY;
    connect_params.connection_u.otaa.nb_trials = 3;

    // stores the status of a call to LoRaWAN protocol
    lorawan_status_t retcode;

    // Initialize LoRaWAN stack
    if (lorawan->initialize(&Lora_ev_queue) != LORAWAN_STATUS_OK) {
        printf("\r\n LoRa initialization failed! \r\n");
        return -1;
    }

    printf("\r\n Mbed LoRaWANStack initialized \r\n");

    // prepare application callbacks
    callbacks.events = mbed::callback(LoraInterface_lora_event_handler);
    lorawan->add_app_callbacks(&callbacks);

    // Set number of retries in case of CONFIRMED messages
    if (lorawan->set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER)
            != LORAWAN_STATUS_OK) {
        printf("\r\n set_confirmed_msg_retries failed! \r\n\r\n");
        return -1;
    }

    printf("\r\n CONFIRMED message retries : %d \r\n",
           CONFIRMED_MSG_RETRY_COUNTER);

    // Enable adaptive data rate
    if (lorawan->enable_adaptive_datarate() != LORAWAN_STATUS_OK) {
        printf("\r\n enable_adaptive_datarate failed! \r\n");
        return -1;
    }

    printf("\r\n Adaptive data  rate (ADR) - Enabled \r\n");

    retcode = lorawan->connect(connect_params);

    if (retcode == LORAWAN_STATUS_OK ||
            retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS) {
    } else {
        printf("\r\n Connection error, code = %d \r\n", retcode);
        return -1;
    }

    printf("\r\n Connection - In Progress ...\r\n");

    // make your event queue dispatching events forever
    Lora_ev_queue.dispatch(0);
    do {
        Lora_ev_queue.dispatch_for(std::chrono::milliseconds(1000));
    } while(connected == false);
    
     return 0;  
}


void LoraInterface_lora_event_handler(lorawan_event_t event){

    switch (event) {
        case CONNECTED:
            /// return connected 
            connected = true;
            break;
        case DISCONNECTED:
            Lora_ev_queue.break_dispatch();
            break;
        case TX_DONE:
            ///indicate message and 
            messageEndTransmit = true;
            break;
        case TX_TIMEOUT:
        case TX_ERROR:
        case TX_CRYPTO_ERROR:
        case TX_SCHEDULING_ERROR:
            // try again
            
                
            Lora_ev_queue.call(LoraInterface_sendMessage);
            
            break;
        case RX_DONE:
            //receivedMessage
            receiveMessage = true;
            LoraInterface_receiveMessage();
            break;
        case RX_TIMEOUT:
        case RX_ERROR:
            printf("\r\n Error in reception - Code = %d \r\n", event);
            break;
        case JOIN_FAILURE:
            printf("\r\n OTAA Failed - Check Keys \r\n");
            break;
        case UPLINK_REQUIRED:
            //send heart beat
             LoraInterface_sendMessage();
            break;
        default:
            MBED_ASSERT("Unknown Event");
    }
}


/**
 * Sends a message to the Network Server
 */
void LoraInterface_sendMessage()
{
    uint16_t packet_len;
    int16_t retcode;
    int32_t sensor_value;

   
        sensor_value = 5;
        printf("\r\n Dummy Sensor Value = %d \r\n", sensor_value);
     

    packet_len = sprintf((char *) tx_buffer, "Dummy Sensor Value is %d",
                         sensor_value);

    retcode = lorawan->send(MBED_CONF_LORA_APP_PORT, tx_buffer, packet_len,
                           MSG_UNCONFIRMED_FLAG);

    memset(tx_buffer, 0, sizeof(tx_buffer));
}

/**
 * Receive a message from the Network Server
 */
 void LoraInterface_receiveMessage()
{
    uint8_t port;
    int flags;
    int16_t retcode = lorawan->receive(rx_buffer, sizeof(rx_buffer), port, flags);

    if (retcode < 0) {
        printf("\r\n receive() - Error code %d \r\n", retcode);
        return;
    }

    printf(" RX Data on port %u (%d bytes): ", port, retcode);
    for (uint8_t i = 0; i < retcode; i++) {
        printf("%02x ", rx_buffer[i]);
    }
    printf("\r\n");
    
    memset(rx_buffer, 0, sizeof(rx_buffer));
}

void LoraInterface_run(){

    int time = 0;
    while(1){
    ThisThread::sleep_for(std::chrono::seconds(period));
     Lora_ev_queue.call_in(1,LoraInterface_sendMessage);
     do {
        Lora_ev_queue.dispatch_for(std::chrono::milliseconds(1000));
    } while(messageEndTransmit == false);
    messageEndTransmit = false;
     do {
         time++;
     }while(receiveMessage == false && time < 5);
    time = 0;
    }
}

