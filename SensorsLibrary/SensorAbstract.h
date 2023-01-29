#pragma once
#include <cstdint>
#include <mbed.h>
class SensorAbstract {
  /**
   * @var Context
   */
 private:

protected:
    SensorAbstract(){}
    uint8_t line;

 public:
  virtual ~SensorAbstract() {
  }

  static void convertFloatToUint32(float floatValue, uint32_t& uint32Value) {
    uint32_t* floatAsInt = (uint32_t*) &floatValue;
    uint32Value = *floatAsInt;
    }

    void convertUint32ToFloat(uint32_t uint32Value, float& floatValue) {
    uint32_t* intAsFloat = (uint32_t*) &floatValue;
    *intAsFloat = uint32Value;
    }


   virtual uint32_t getTemp(){return 0;};
   virtual uint32_t getHumid(){return 0;};
   virtual uint32_t getLux(){return 0;};
   virtual uint32_t getECo2(){return 0;};
   virtual uint32_t getTOVC(){return 0;};
   virtual uint32_t getCo2(){return 0;};
   virtual uint32_t getPression(){return 0;};
   virtual uint32_t getUV(){return 0;};
  virtual int32_t init(){return 0;};
  virtual int32_t wakeUp(){return 0;};
  virtual int32_t sleep(){return 0;};
  void setLine(uint8_t line ){this->line = line;};
    uint8_t getLine(){return line;};


};