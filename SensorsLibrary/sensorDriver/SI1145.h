

#include "mbed.h"
#define SI1145_ADDRESS (0x60 << 1)

extern I2C i2c;
/** SI1145 class
 *  SI1145: A library to correct environmental data using adafruit SI1145 device
 */
 
class SI1145
{
public:


    /** Create a SI1145 instance
     *  which is connected to specified I2C pins with specified address
     * @param i2c_obj I2C object (instance)
     * @param slave_adr (option) I2C-bus address (default: 0x60)
     */
    SI1145(char slave_adr = SI1145_ADDRESS);
    /** Destructor of SI1145
     */
    //virtual ~SI1145();
    /** Initialize SI1145 sensor
     *  Configure sensor setting and read parameters for calibration
     */
    int8_t initalize(void);
   /** Begin Initialization SI1145 sensor
     *  Configure sensor setting and read parameters for calibration
     */
    int8_t getUV(uint16_t & temp);
    /** Read the current VIS value from SI1145 sensor
     */
    int8_t getVIS(uint16_t & temp);
    int8_t getUVlsb(float & temp);
    int8_t getUVmsb(float & temp);
    /** Read the current IR value from SI1145 sensor
     */
    int8_t getIR(uint16_t & temp);
    /** Read the current PROX value from SI1145 sensor
     */
    int8_t getPROX(uint16_t & temp);
    static SI1145 * getInstance();
private:
    static SI1145 * Instance;
   
    char        address;
    int32_t     t_fine;
};
