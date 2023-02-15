/**
 *  SI1145 Digital UV Index / IR / Visible Light Sensor library
 *  @author  N Good
 *  @date    5-May-2015
 *  Library for "SI1145 Digital UV Index / IR / Visible Light Sensor" from adafruit
 *  https://www.adafruit.com/products/1777
 */

#include "mbed.h"
#include "SI1145.h"


SI1145::SI1145( char slave_adr)
    :
    address(slave_adr),
    t_fine(0)
{
}

//SI1145::~SI1145()
//{
//    if (NULL != i2c_p)
//        delete  i2c_p;
//}
    
int8_t SI1145::initalize(void) {
    uint16_t idu;
    int8_t ret;
    //i2c.frequency(100000);
   char id[1] = {0x00};
    ret += i2c.write(address,id,1);
    char data[1] = {0};
    ret += i2c.read(address,data,1);
    float idf;
    idu = data[0];
    idf = (float)idu;
      //  pc.printf("%4.0f ID\n",idf);  COMMENTED OUT B/C only one Serial object can exist in main...???
// reset______________________________________________
    //write MEASRATE0
        char MEASRATE0[2] = {0x08,0x0};
        ret += i2c.write(address,MEASRATE0,2);
    //write MEASRATE1
        char MEASRATE1[2] = {0x09,0x0};
        ret += i2c.write(address,MEASRATE1,2);
    //write visQEN
        char visQEN[2] = {0x04,0x0};
        ret += i2c.write(address,visQEN,2);
    // visQMODE1
        char visQMODE1[2] = {0x05,0x0};
        ret += i2c.write(address,visQMODE1,2);
    //write visQMODE2
       char visQMODE2[2] = {0x06,0x0};
        ret += i2c.write(address,visQMODE2,2);
    //write INTCFG
        char INTCFG[2] = {0x03,0x0};
        ret += i2c.write(address,INTCFG,2);
    //write visQSTAT
        char visQSTAT[2] = {0x21,0xFF};
        ret += i2c.write(address,visQSTAT,2);
    //write COMMAND
       char COMMAND[2] = {0x18,0x01};
        ret += i2c.write(address,COMMAND,2);
     // wait
        ThisThread::sleep_for(100ms);
    //write HWKEY
       char HWKEY[2] = {0x07, 0x17};
        ret += i2c.write(address,HWKEY,2);
    // wait
        ThisThread::sleep_for(100ms);
//__________________________________________________________
    // enable UVindex measurement coefficients!
    char REG_UCOEFF0[2] = {0x13,0x29};           
    ret += i2c.write(address, REG_UCOEFF0, 2);
    char REG_UCOEFF1[2] = {0x14,0x89};           
    ret += i2c.write(address, REG_UCOEFF1, 2);
    char REG_UCOEFF2[2] = {0x15,0x02};           
    ret += i2c.write(address, REG_UCOEFF2, 2);
    char REG_UCOEFF3[2] = {0x16,0x00};           
    ret += i2c.write(address, REG_UCOEFF3, 2);
//__________________________________________________________
    
    // enable UV sensor
    char PARAM_CHLIST[2] = {0x17, 0x80 | 0x20 | 0x10 | 0x01};           
    ret += i2c.write(address, PARAM_CHLIST, 2);
    char COMMD_CHLIST[2] = {0x18, 0x01 | 0xA0};
    ret += i2c.write(address, COMMD_CHLIST, 2);
    // enable interrupt on every sample
    char REG_INTCFG[2] = {0x03,0x01};           
    ret += i2c.write(address, REG_INTCFG, 2);
    char REG_visQEN[2] = {0x04,0x01};        
    ret += i2c.write(address, REG_visQEN, 2);
    // program proximity sensor LED current
    char REG_PSLED21[2] = {0x0F,0x03};           
    ret += i2c.write(address, REG_PSLED21, 2); // 20mA for LED 1 only
    char PARAM_ADCMUX_LARGEvis[2] = {0x17, 0x03};           
    ret += i2c.write(address, PARAM_ADCMUX_LARGEvis, 2);
    char PARAM_PS1ADCMUX[2] = {0x18, 0x07 | 0xA0};
    ret += i2c.write(address, PARAM_PS1ADCMUX, 2);
    // prox sensor #1 uses LED #1
    char PARAM_PSLED12SEL_PS1LED1[2] = {0x17, 0x01};           
    ret += i2c.write(address, PARAM_PSLED12SEL_PS1LED1, 2);
    char PARAM_PSLED12SEL[2] = {0x18, 0x02 | 0xA0};
    ret += i2c.write(address, PARAM_PSLED12SEL, 2);
    // fastest clocks, clock div 1
    char PARAM_0[2] = {0x17, 0x0 | 0xA0};
    ret += i2c.write(address, PARAM_0, 2);
    char PARAM_PSADCGAIN[2] = {0x18, 0x0B};           
    ret += i2c.write(address, PARAM_PSADCGAIN, 2);
    // take 511 clocks to measure
    char PARAM_ADCCOUNTER_511CLK[2] = {0x17, 0x70};           
    ret += i2c.write(address, PARAM_ADCCOUNTER_511CLK, 2);
    char PARAM_PSADCOUNTER[2] = {0x18, 0x0A | 0xA0};
    ret += i2c.write(address, PARAM_PSADCOUNTER, 2);
    // in prox mode, high range
    char PARAM_PSADCMISC_RANGE_PARAM_PSADCMISC_PSMODE[2] = {0x17, 0x20 | 0x04};           
    ret += i2c.write(address, PARAM_PSADCMISC_RANGE_PARAM_PSADCMISC_PSMODE, 2);
    char PARAM_PSADCMISC[2] = {0x18, 0x0C | 0xA0};
    ret += i2c.write(address, PARAM_PSADCMISC, 2);
    //
    char PARAM_ADCMUX_SMALLvis[2] = {0x17, 0x00};           
    ret += i2c.write(address, PARAM_ADCMUX_SMALLvis, 2);
    char PARAM_ALSvisADCMUX[2] = {0x18, 0x0E | 0xA0};
    ret += i2c.write(address, PARAM_ALSvisADCMUX, 2);
    // fastest clocks, clock div 1   
    ret += i2c.write(address, PARAM_0, 2);
    char PARAM_ALSvisADCGAIN[2] = {0x18, 0x1E | 0xA0};
    ret += i2c.write(address, PARAM_ALSvisADCGAIN, 2);
    // take 511 clocks to measure         
    ret += i2c.write(address, PARAM_ADCCOUNTER_511CLK, 2);
    char PARAM_ALSvisADCOUNTER[2] = {0x18, 0x1D | 0xA0};
    ret += i2c.write(address, PARAM_ALSvisADCOUNTER, 2);
    // in high range mode
    char PARAM_ALSvisADCMISC_RANGE[2] = {0x17, 0x20};           
    ret += i2c.write(address, PARAM_ALSvisADCMISC_RANGE, 2);
    char PARAM_ALSvisADCMISC[2] = {0x18, 0x1F | 0xA0};
    ret += i2c.write(address, PARAM_ALSvisADCMISC, 2);
    // fastest clocks, clock div 1
    char PARAM_000[2] = {0x17, 0x0};           
    ret += i2c.write(address, PARAM_000, 2);
    char PARAM_ALSuvADCGAIN[2] = {0x18, 0x11 | 0xA0};
    ret += i2c.write(address, PARAM_ALSuvADCGAIN, 2);
    // take 511 clocks to measure       
    ret += i2c.write(address, PARAM_ADCCOUNTER_511CLK, 2);
    char PARAM_ALSuvADCOUNTER[2] = {0x18, 0x10 | 0xA0};
    ret += i2c.write(address, PARAM_ALSuvADCOUNTER, 2);
    // in high range mode (not normal signal)
    char PARAM_ALSuvADCMISC_uvRANGE[2] = {0x17, 0x20};           
    ret += i2c.write(address, PARAM_ALSuvADCMISC_uvRANGE, 2);
    char PARAM_ALSuvADCMISC[2] = {0x18, 0x12 | 0xA0};
    ret += i2c.write(address, PARAM_ALSuvADCMISC, 2);
    // measurement rate for auto
    char REG_MEASRATE0[2] = {0x08,0xFF};           
    ret += i2c.write(address, REG_MEASRATE0, 2);// 255 * 31.25uS = 8ms
    // auto run
    char REG_COMMAND[2] = {0x18,0x0F};           
    ret += i2c.write(address, REG_COMMAND, 2);
    return ret;
}


int8_t SI1145::getUV(uint16_t & temp)
{
    // Variables
    //float valf; 
    int8_t ret;                         // uv value
    char reg[1];                         // register
    char data[2] = {0,0};                // data 2 x 8 bits
    uint16_t vali;                       // uv value
    // Set register    
    reg[0] = 0x2C;
    // Read registers
    ret += i2c.write(address, reg, 1);
    ret += i2c.read(address, data, 2);
    // Merge bytes
    vali = data[0] | (data[1] << 8);    // int
    temp = vali;

    //valf = (float)vali;                 // convert to float
    // Return value
    return ret;
}


int8_t SI1145::getVIS(uint16_t & temp)
{
    // Variables
    //float valf; 
    int8_t ret;                         // vis (IR+UV) value
    char reg[1];                         // register
    char data[2] = {0,0};                // data 2 x 8 bits
    uint16_t vali;                       // vis value
    // Set register    
    reg[0] = 0x22;
    // Read registers
    ret += i2c.write(address, reg, 1);
    ret += i2c.read(address, data, 2);
    // Merge bytes
    vali = data[0] | (data[1] << 8);    // int
    temp = vali;
    //valf = (float)vali;                // convert to float
    // Return value
    return ret;
}

int8_t SI1145::getIR(uint16_t & temp)
{
    // Variables
    //float valf;  
    int8_t ret;                        // ir value
    char reg[1];                         // register
    char data[2] = {0,0};                // data 2 x 8 bits
    uint16_t vali;                       // ir value
    // Set register    
    reg[0] = 0x24;
    // Read registers
    ret = i2c.write(address, reg, 1);
    ret = i2c.read(address, data, 2);
    // Merge bytes
    vali = data[0] | (data[1] << 8);    // int
    //valf = (float)vali;                 // convert to float
    temp = vali;
    // Return value
    return ret;
}

int8_t SI1145::getPROX(uint16_t & temp)
{
    // Variables
    //float valf;   
    int8_t ret;                       // prox value
    char reg[1];                         // register
    char data[2] = {0,0};                // data 2 x 8 bits
    uint16_t vali;                       // prox value
    // Set register    
    reg[0] = 0x26;
    // Read registers
    ret += i2c.write(address, reg, 1);
    ret += i2c.read(address, data, 2);
    // Merge bytes
    vali = data[0] | (data[1] << 8);    // int
    //valf = (float)vali;                 // convert to float
    temp = vali;
    // Return value
    return ret;
}

SI1145 * SI1145::Instance = nullptr;

SI1145* SI1145::getInstance(){
    if (Instance == nullptr){
        Instance = new SI1145();
    }
    
    return Instance;

}
