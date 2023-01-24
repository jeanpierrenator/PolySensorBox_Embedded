#pragma once 
#include "mbed.h"
#include "BufferedSerial.h"
#include "SerialManager.h"
#include "memory.h"
extern Thread sensorThread;

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


class StateSerialProtocol;
class Wait_for_begin_Transmit_state;
class Wait_for_char_state;
class SerialProtocole
{

private:

    static char recvBuff[1];
    static int recvLen;

    static void process(char charToProcess);
    static StateSerialProtocol * _state;
    


    
    
public:


   SerialProtocole();
   static void init(void);
   static void onSerialReceived(void);
   
   static void TransitionTo(StateSerialProtocol *state);
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
  int size = 0;
  int receiveSize =0;
  char tempBuff[255];

 public:
  virtual ~StateSerialProtocol() {
  }

  uint8_t calculChecksum(const char * buffer,int size);


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




