#ifndef _VEML7700_H_
#define _VEML7700_H_

#include "mbed.h"

/**
 * Vishay VEML7700
 * High Accuracy Ambient Light Sensor with I2C Interface
 */
 /*
 * @code
* @endcode
*/
extern I2C i2c;
class VEML7700
{
public:
  /**
  * VEML7700 constructor
  *
  * @param sda SDA pin
  * @param sdl SCL pin
  * @param addr addr of the I2C peripheral
  */
  VEML7700( int addr);


  /**
  * VEML7700 destructor
  */
  ~VEML7700();

/**
 * set ALS Configure register
 * @param uint16_t conf value to set
 * @returns none
 * @note bit[15:13] (reserved)
 * @note -
 * @note bit[12:11] ALS_GAIN Gain selection
 * @note 00=ALS gain x1, 01=ALS gain x2, 10=ALS gain x(1/8), 11=ALS gain x (1/4)
 * @note -
 * @note bit[10] (reserved)
 * @note -
 * @note bit[9:6] ALS_IT ALS integration time setting
 * @note 1100=25ms, 1000=50ms, 0000=100ms, 0001=200ms, 0010=400ms, 0011=800ms
 * @note -
 * @note bit[5:4] ALS_PERS ALS persistence protect number setting
 * @note 00=1, 01=2, 10= 4, 11=8
 * @note -
 * @note bit[1] ALS_INT_EN ALS interrupt enable setting
 * @note 0:ALS INT disable, 1:ALS INT enable
 * @note -
 * @note bit[0] ALS_SD ALS shutdown setting
 * @note 0=ALS power on, 1=ALS shutdown
 * default conf = 0000000110000001 (385)
 */
int setALSConf(uint16_t conf) ;

/**
 * set ALS high threshold window setting
 * @param uint16_t wh value to set
 * @returns none
 */
int setALS_WH(uint16_t wh) ;

/**
 * set ALS low threshold window setting
 * @param uint16_t wl value to set
 * @returns none
 */
int setALS_WL(uint16_t wl) ;

/**
 * set Power Saving setting
 * @param uint16_t ps
 * @returns none
 * @note bit[2:1] PSM Power Saving Mode
 * @note bit[0] PSM_EN Power Saving Mode enable setting
 * ON  = 1
 * OFF = 0
 */
int setPowerSaving(uint16_t ps) ;

/**
 * get ALS 
 * @param none
 * @returns uint16_t ALS (Ambient Light Sensed(?))
 */
int getALS(uint16_t & temp) ;

/**
 * get White channel reading
 * @param none
 * @returns uint16_t White channel sensed
 */
int getWHITE(uint16_t & temp) ;

/**
 * get ALS INT trigger event
 * @param none
 * @returns uint16_t ALS_INT
 * @note bit[15] ALS_IF_L ALS crossing low threshold INT trigger event
 * @note bit[14] ALS_IF_H ALS crossing high threshold INT trigger event
 * @note bit[13:0] (reserved)
 */
int getALS_INT(uint16_t & temp);


static VEML7700 * getInstance();

  
private:
  
  int m_addr;
  int readRegs(int addr, uint8_t * data, int len);
  int writeRegs(uint8_t * data, int len);
  static VEML7700 * Instance;


};
#endif /* _VEML6030_H_ */
