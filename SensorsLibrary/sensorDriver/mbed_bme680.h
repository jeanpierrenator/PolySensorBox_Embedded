#ifndef BME680_H
#define BME680_H

#include "mbed.h"
#include "bme680_defs.h"

#define BME680_DEFAULT_ADDRESS (0x76 << 1)  // The default I2C address (shifted for MBed 8 bit address)
//#define BME680_DEBUG_MODE  // Use this for enhance debug logs for I2C and more.

extern I2C i2c;

/**
 * BME680 Class for I2C usage.
 * Wraps the Bosch library for MBed usage.
 */
class BME680 {
public:
    BME680();

    BME680(uint8_t adr);

    bool begin();

    bool setTemperatureOversampling(uint8_t os);

    bool setPressureOversampling(uint8_t os);

    bool setHumidityOversampling(uint8_t os);

    bool setIIRFilterSize(uint8_t fs);

    bool setGasHeater(uint16_t heaterTemp, uint16_t heaterTime);

    bool performReading();

    bool isGasHeatingSetupStable();

    int16_t getRawTemperature();
    uint32_t getRawPressure();
    uint32_t getRawHumidity();
    uint32_t getRawGasResistance();

    float getTemperature();

    float getPressure();

    float getHumidity();

    float getGasResistance();

    // legacy library
    int8_t bme680_init(struct bme680_dev *dev);

    int8_t bme680_set_sensor_settings(uint16_t desired_settings, struct bme680_dev *dev);

    int8_t null_ptr_check(const struct bme680_dev *dev);

    int8_t boundary_check(uint8_t *value, uint8_t min, uint8_t max, struct bme680_dev *dev);

    int8_t bme680_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, struct bme680_dev *dev);

    int8_t bme680_set_regs(const uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, struct bme680_dev *dev);

    int8_t bme680_soft_reset(struct bme680_dev *dev);

    int8_t set_mem_page(uint8_t reg_addr, struct bme680_dev *dev);

    int8_t get_mem_page(struct bme680_dev *dev);

    int8_t get_calib_data(struct bme680_dev *dev);

    int8_t bme680_get_sensor_data(struct bme680_field_data *data, struct bme680_dev *dev);

    int8_t read_field_data(struct bme680_field_data *data, struct bme680_dev *dev);

    int16_t calc_temperature(uint32_t temp_adc, struct bme680_dev *dev);

    uint32_t calc_pressure(uint32_t pres_adc, const struct bme680_dev *dev);

    uint32_t calc_humidity(uint16_t hum_adc, const struct bme680_dev *dev);

    uint32_t calc_gas_resistance(uint16_t gas_res_adc, uint8_t gas_range, const struct bme680_dev *dev);

    void bme680_get_profile_dur(uint16_t *duration, const struct bme680_dev *dev);

    int8_t bme680_set_sensor_mode(struct bme680_dev *dev);

    int8_t set_gas_config(struct bme680_dev *dev);

    uint8_t calc_heater_res(uint16_t temp, const struct bme680_dev *dev);

    uint8_t calc_heater_dur(uint16_t dur);

    static BME680 * getInstance();

private:
    static BME680 * instance;
    bool _filterEnabled, _tempEnabled, _humEnabled, _presEnabled, _gasEnabled;
    int32_t _sensorID;
    struct bme680_dev gas_sensor;
    struct bme680_field_data data;
    uint8_t _adr;

    

    static void log(const char *format, ...);

    // BME680 - hardware interface
    static int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

    static int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

    static void delay_msec(uint32_t ms);
};

#endif