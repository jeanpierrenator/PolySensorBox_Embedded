#pragma once 
#include "mbed.h"
#include "BufferedSerial.h"
#include "SerialManager.h"
#include "thread_Params.h"
#include "Periph/memory.h"
#include <cstdint>

static constexpr char STATUS_OK_CODE = 0xFF;
static constexpr char STATUS_BAD_CODE = 0x00;
// Frame Begin
static constexpr char START_COM_CODE = 0xAA;
static constexpr char START_COM_CODE2 = 0x55;
// Frame 0x01
static constexpr char START_CONFIG_CODE = 0x01;
static constexpr char START_CONFIG_BEGIN = 0xFF;
static constexpr char START_CONFIG_STOP = 0x00;
// Frame 0x02
static constexpr char CONFIG_CODE = 0x02;
// frame 0x03
static constexpr char START_READ_CONFIG_CODE = 0x03;
static constexpr char START_READ_CONFIG_BEGIN = 0xFF;
static constexpr char START_READ_CONFIG_STOP = 0x00;

//frame 0x04
static constexpr char READ_CONFIG_CODE = 0x04;

//frame 0x05
static constexpr char LORA_PARAMETER_CODE = 0x05;
static constexpr char SIZE_LORA_PARAMETER_FRAME = 36;

// Forward declarations for the StateSerialProtocol class and its subclasses
class StateSerialProtocol;
class Wait_for_begin_Transmit_state;
class Wait_for_char_state;


class SerialProtocol
{

private:

      static char recvBuff[1];   // Define a static char array to store received data
    static int32_t recvLen;    // Define a static integer to store the length of the received data
    static StateSerialProtocol* _state;  // Define a pointer to the current state of the SerialProtocol

    static void process(char charToProcess); // function to processe input data
    


    
    
public:


   SerialProtocol();

   static void init(void);
   static void onSerialReceived(void);
   
   static void TransitionTo(StateSerialProtocol *state);

   //Response Frame
   static void sendBeginRes(void);
   static void sendStatus(char code,char status);
   static void sendChar(char code);
   static void sendBuffer(char * buffer , char size);
    
};

class StateSerialProtocol {
  /**
   * @var Context
   */
 protected:
  int32_t size = 0;
  int32_t receiveSize =0;
  char tempBuff[255];

 public:
  virtual ~StateSerialProtocol() {
  }
  //function to compute checksum of an input buffer 
  uint8_t calculChecksum(const char * buffer,int32_t size);


  virtual void process(char charToProcess) = 0;
};


//Wait begin Frame 0xAA 0x55
class Wait_for_begin_Transmit_state : public StateSerialProtocol {
 public:
   void process(char charToProcess) override;
};

//Basic State wait for any code 
class Wait_for_char_state : public StateSerialProtocol {
     public:
   void process(char charToProcess) override;
};

//Wait for the parametter for frame 1
class Wait_for_config_parameter : public StateSerialProtocol {
     public:
   void process(char charToProcess) override;
};

//wait for th next frame 2 or frame 1 to finish the transmitions 
class wait_for_config_transmit : public StateSerialProtocol {
     public:
   void process(char charToProcess) override;
};

//process Frame 2 reception 
class Trame_config_start : public StateSerialProtocol {
     public:
   void process(char charToProcess) override;
};



//Wait for the parametter for frame 3
class Wait_for_read_config_parameter : public StateSerialProtocol {
     public:
   void process(char charToProcess) override;
};

class Wait_for_Frame_4_response : public StateSerialProtocol {
     public:
   void process(char charToProcess) override;
};

class Send_Frame_4 : public StateSerialProtocol {
     public:
   void process(char charToProcess) override;
};

//Wait for the parametter for frame 5
class Wait_For_Lora_parameter : public StateSerialProtocol {
     public:
   void process(char charToProcess) override;
};




