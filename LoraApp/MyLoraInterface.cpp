#include "MyLoraInterface.h"
// Application helpers
#include "trace_helper.h"
#include "lora_radio_helper.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>

static EventQueue Lora_ev_queue(10*EVENTS_EVENT_SIZE);
bool connected =0;
uint32_t period = 0;
bool messageEndTransmit = false;
bool receiveMessage = false;
uint16_t packet_len = 0;

static constexpr char SEND_WHEN_READY_FRAME_CODE = 0x02;

static constexpr char RECEIVE_GPIO = 0x04;
static constexpr char RECEIVE_NEW_PERIOD_FRAME = 0x05;
static constexpr char RECEIVE_TIME_STAMPE_FRAME = 0x07;

/*Protocol*/
/* send frame*/
void fillTxbufferWithFrame2(){
    tx_buffer[0] = 0x02;
    tx_buffer[1] = sizeToSendLora/5;

    for(int i =0 ; i < sizeToSendLora;i++){
       tx_buffer[i+2] =ram_sendBuffer[i];
    }
    packet_len = sizeToSendLora+2;
}

void fillTxbufferWithFrame1(uint16_t * currentStartLogAddress){
    char tempbuf[256];

    i2c_sem.acquire();
    packet_len = Memory::getInstance()->readAllLogFrameFrom(*currentStartLogAddress, tempbuf);
    *currentStartLogAddress = Memory::getInstance()->getCurrentLogPointer();
    i2c_sem.release();
    if(packet_len == 0){
        //nodata to read send heartbeat 
        tx_buffer[0] = 0x03;
        packet_len = 1;
    }else {
         tx_buffer[0] = 0x01;
        tx_buffer[1] = packet_len/9;
    for(int32_t i = 0; i < packet_len; i++){
        tx_buffer[i+2] = tempbuf[i];
    }
    packet_len = packet_len+2;
    }
   

}


/* end send frame*/

/* receive frame*/
int processFrame4(uint32_t currentStart){
    uint8_t nbGPIO = (rx_buffer[currentStart+1]>>4) & 0x0f;
    uint8_t state = rx_buffer[currentStart+1] & 0x01;

    if(nbGPIO ==1){
        DigitalOut led(PA_5);
        led.write(state);
    }
    if(nbGPIO == 2){
        DigitalOut led(LED2);
        led.write(state);
    }
    if(nbGPIO == 3){
        DigitalOut led(LED3);
        led.write(state);
    }
    return 2;
}
int processFrame5(uint32_t currentStart){
    uint32_t physicalValue = rx_buffer[currentStart+1];
    uint32_t value = 0;
    value |= static_cast<uint32_t>(rx_buffer[currentStart+2]) << 16;
    value |= static_cast<uint32_t>(rx_buffer[currentStart+3]) << 8;
    value |= static_cast<uint32_t>(rx_buffer[currentStart+4]) << 0;
    SensorManager::physicalPeriodeBaseMap[physicalValue] = value;
    return 5;
}

int processFrame7(uint32_t currentStart){
    uint32_t timeStamp = 0;
    timeStamp |= static_cast<uint32_t>(rx_buffer[currentStart+1]) << 24;
    timeStamp |= static_cast<uint32_t>(rx_buffer[currentStart+2]) << 16;
    timeStamp |= static_cast<uint32_t>(rx_buffer[currentStart+3]) << 8;
    timeStamp |= static_cast<uint32_t>(rx_buffer[currentStart+4]) << 0;
    RTC_setTime(timeStamp);
    return 5;
}
void parseReaceiveFrame(){
    uint32_t size = 0;
    uint32_t sizeToRead = rx_buffer[0];
    size+=1;
    do{
        switch (rx_buffer[size]) {
        case RECEIVE_GPIO:
        size += processFrame4(size);
        break;
        case RECEIVE_NEW_PERIOD_FRAME:
        size+= processFrame5(size);
        break;
        case RECEIVE_TIME_STAMPE_FRAME:
        size+= processFrame7(size);
        break;
        default:
        size = sizeToRead+1;
        break;
        }
    }while(size-1 < sizeToRead);


}

/* end receive frame*/

/* end Protocol*/

int8_t LoraInterface_init( uint8_t * APPKEY,  uint8_t * APPUUID, uint8_t * DevUUID ,uint32_t sendPeriod){
    lorawan = new LoRaWANInterface(radio);
    period = sendPeriod;
     // setup tracing
    setup_trace();
     lorawan_connect_t connect_params;
        //OTAA:
    connect_params.connect_type = LORAWAN_CONNECTION_OTAA;
    connect_params.connection_u.otaa.dev_eui = DevUUID;
    connect_params.connection_u.otaa.app_eui = APPUUID;
    connect_params.connection_u.otaa.app_key = APPKEY;
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

    // make your event queue dispatching while not connected
    do {
        Lora_ev_queue.dispatch_for(std::chrono::milliseconds(10000));
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
    
    int16_t retcode;
   

    retcode = lorawan->send(MBED_CONF_LORA_APP_PORT, tx_buffer, packet_len,
                           MSG_UNCONFIRMED_FLAG);

    memset(tx_buffer, 0, sizeof(tx_buffer));
}

/**
 * Receive a message from the Network Server
 */
 void LoraInterface_receiveMessage()
{
    printf("receive message\n");
    uint8_t port;
    int flags;
    int16_t retcode = lorawan->receive(rx_buffer, sizeof(rx_buffer), port, flags);

    parseReaceiveFrame();

    memset(rx_buffer, 0, sizeof(rx_buffer));
}

void LoraInterface_run_timed(){

    int time = 0;
    uint16_t currentStartLogAddress = 0;
    i2c_sem.acquire();
    currentStartLogAddress = Memory::getInstance()->getCurrentLogPointer();
    i2c_sem.release();
    while(1){
    ThisThread::sleep_for(std::chrono::seconds(period));
     fillTxbufferWithFrame1(&currentStartLogAddress);
     Lora_ev_queue.call_in(1,LoraInterface_sendMessage);
     do {
        Lora_ev_queue.dispatch_for(std::chrono::milliseconds(1000));
    } while(messageEndTransmit == false);
    messageEndTransmit = false;
    receiveMessage = false;
    }
}


void LoraInterface_run_when_ready(){

    int time = 0;
    while(1){
    
    loraSend_mut.lock();
    loraReady_mut.lock();
    loraReady_mut.unlock();
    loraSend_mut.unlock();
    
    fillTxbufferWithFrame2();


     Lora_ev_queue.call_in(1,LoraInterface_sendMessage);
     do {
        Lora_ev_queue.dispatch_for(std::chrono::milliseconds(1000));
    } while(messageEndTransmit == false);
    messageEndTransmit = false;
    receiveMessage = false;
    }
    
}









 void LoraInterface_connectedTest(){
    if(connected){
        loraNoError();
    }else {
        loraDisconnectedError();
    }
}
 void LoraInterface_resetLora(){
    NVIC_SystemReset();
}

