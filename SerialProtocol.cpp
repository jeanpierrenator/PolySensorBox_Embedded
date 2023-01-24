#include "SerialProtocol.h"
#include <cstdio>

 char SerialProtocole::recvBuff[1];
 int SerialProtocole::recvLen;
 StateSerialProtocol * SerialProtocole::_state;

SerialProtocole::SerialProtocole(){
       TransitionTo(new Wait_for_begin_Transmit_state());
};

void SerialProtocole::init(){
     TransitionTo(new Wait_for_begin_Transmit_state());
};


void SerialProtocole::onSerialReceived(void)
{
    while ( SerialManager::getinstance()->getSerial()->readable()) {
        // Read serial
        recvLen =  SerialManager::getinstance()->getSerial()->read(recvBuff, 1);
       process(recvBuff[0]);
        
    }
  
}

void SerialProtocole::process(char charToProcess){
     _state->process(charToProcess);
    //sendBeginRes();
}

void SerialProtocole::TransitionTo(StateSerialProtocol *state) {
    if (_state != nullptr)
      delete _state;
    _state = state;
  }


void SerialProtocole::sendBeginRes(void){
    char resp[2] = {START_COM_CODE2,START_COM_CODE};
    SerialManager::getinstance()->write(&resp, 2);
}

void SerialProtocole::sendStatus(char code, char status){
    char resp[2] = {code,status};
    SerialManager::getinstance()->write(&resp, 2);
}
void SerialProtocole::sendChar(char code){
     char resp[1] = {code};
    SerialManager::getinstance()->write(&resp, 1);
}

void SerialProtocole::sendBuffer(char * buffer , char size){
    SerialManager::getinstance()->write(buffer, size);
}





/*==================================define state==================================*/
uint8_t StateSerialProtocol::calculChecksum(const char *buffer, int size){
    int checksum =0;
    for(uint8_t i=0 ; i < size-1 ;i++ ){
        checksum += buffer[i];
    }
    checksum = checksum % 256;


    return (uint8_t)checksum;
}

void Wait_for_begin_Transmit_state::process(char charToProcess) {
      if (charToProcess == START_COM_CODE2 && size == 1 ) {
        SerialProtocole::sendBeginRes();
        SerialProtocole::TransitionTo(new Wait_for_char_state() );
        sensorThread.terminate();
      }else {
        size = 0;
      }
    if(charToProcess == START_COM_CODE && size == 0){
        size++;
    }
  }


  void Wait_for_char_state::process(char charToProcess) {

        if (charToProcess == START_CONFIG_CODE  ) {
        SerialProtocole::TransitionTo(new Wait_for_config_parameter() );
      }
       if (charToProcess == START_READ_CONFIG_CODE  ) {
        SerialProtocole::TransitionTo(new Wait_for_read_config_parameter() );
      }
  }

   void Wait_for_config_parameter::process(char charToProcess) {

        if (charToProcess == START_CONFIG_BEGIN  ) {
        //start configuration transmission 
        SerialProtocole::TransitionTo(new wait_for_config_transmit() );
        Memory::getInstance()->clearConfig();
      }
      if (charToProcess == START_CONFIG_STOP  ) {
          //stop receive Configuration 
        SerialProtocole::TransitionTo(new Wait_for_char_state() );
      }
        SerialProtocole::sendStatus(START_CONFIG_CODE, charToProcess);
  }

  
   void wait_for_config_transmit::process(char charToProcess) {

        if (charToProcess == START_CONFIG_CODE  ) {
        SerialProtocole::TransitionTo(new Wait_for_config_parameter() );
      }
      if (charToProcess == CONFIG_CODE  ) {
        SerialProtocole::TransitionTo(new Trame_config_start() );
      }
  }

void Trame_config_start::process(char charToProcess) {
        tempBuff[size] = charToProcess;
        if(size == 1){
            receiveSize = ((tempBuff[size] & 0b00111111) * 4)+3;
        }
       

       size++;
       if(receiveSize !=0 && size == receiveSize){
           //calcul checksum
           
           if(calculChecksum(tempBuff, receiveSize) == tempBuff[size-1]){
               //data correct 
               // envoie status OK
               SerialProtocole::sendStatus(CONFIG_CODE, 0xFF);
               /***/
               //save data into EEPROME
               Memory::getInstance()->writeConfigFrame(tempBuff);
               /***/
           }else{
               //data incorrect
               // envoie status default
               SerialProtocole::sendStatus(CONFIG_CODE, 0x00);
           }
           SerialProtocole::TransitionTo(new wait_for_config_transmit() );
       }
  }
  


  void Wait_for_read_config_parameter::process(char charToProcess) {

        if (charToProcess == START_READ_CONFIG_BEGIN  ) {
            SerialProtocole::sendStatus(START_READ_CONFIG_CODE, START_READ_CONFIG_BEGIN);
            SerialProtocole::TransitionTo(new Wait_for_Frame_4_response() );
            Memory::getInstance()->resetPointer();
      }else {
      SerialProtocole::sendStatus(0xff, 0xff);
            SerialProtocole::TransitionTo(new Wait_for_char_state() );
      }
  }

    void Wait_for_Frame_4_response::process(char charToProcess) {

     if(charToProcess == READ_CONFIG_CODE){
            SerialProtocole::TransitionTo(new Send_Frame_4() );
        }else{
            SerialProtocole::sendStatus(0xff, 0xff);
            SerialProtocole::TransitionTo(new Wait_for_char_state() );
        }
  }

    void Send_Frame_4::process(char charToProcess) {
        int readSize =0;
         if (charToProcess == STATUS_OK_CODE) {
        //update read pointer
        Memory::getInstance()->setReadPointerToTheNextFrame();
        }else if (charToProcess == STATUS_BAD_CODE) {
        
        }

        readSize = Memory::getInstance()->readCurrentConfigFrame(tempBuff);
        if(readSize != -1){

        
        //Calcul checksum
        tempBuff[readSize] = calculChecksum(tempBuff, readSize+1);
        //send frame 4
        SerialProtocole::sendChar(READ_CONFIG_CODE);
        SerialProtocole::sendBuffer(tempBuff,readSize+1);
        SerialProtocole::TransitionTo(new Wait_for_Frame_4_response() );
        }else{
            SerialProtocole::sendStatus(START_READ_CONFIG_CODE, STATUS_BAD_CODE);
            SerialProtocole::TransitionTo(new Wait_for_char_state() );
        }
        
        

        
      
  }

