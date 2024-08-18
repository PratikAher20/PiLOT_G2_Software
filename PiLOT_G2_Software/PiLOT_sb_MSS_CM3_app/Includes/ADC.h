/**
 * @file ADC.h
 * @author Srinidhi G., Pratik A.
 * @brief : Functions to read data values from ADC: AD7997_7998
 * @version : 1.0
 * @date 2024-08-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef INCLUDES_ADC_H_
#define INCLUDES_ADC_H_

#define ADC_I2C_ADDR 0x20
#define ADC_I2CU1_ADDR 0x21
#define ADC_I2CU2_ADDR 0x22

#define DATAHIGH_MAX_H 0x0F
#define DATAHIGH_MAX_L 0xFC
#define DATA_LOW_MIN_H 0x00
#define DATA_LOW_MIN_L 0x00

#define DATA_LOW_REG(x) ((x) * 3 + 0x4)
#define DATA_HIGH_REG(x) ((x) * 3 + 0x5)
#define ADC_CHX(x) (((x) + 0x8)<<4)

#define TEMP_ADC_CORE_I2C	&g_core_i2c1

#include "core_i2c.h"

/** Function to initialise ADC
 * @brief Initialises the ADC corresponding to the given address
 * 
 * @param i2c_chx: The i2c channel through which the ADC is connected
 * @param address: The address of the ADC
 * 
 */
uint8_t ADC_Init(i2c_instance_t *i2c_chx,uint8_t address);
/**
 * @brief Get the ADC value 
 * 
 * @param i2c_chx   : the i2c channel used
 * @param address   : the address of the ADC
 * @param chx       : the channel to convert  
 * @return double   : returns the value given by the ADC
 */
uint16_t get_ADC_value(i2c_instance_t *i2c_chx,uint8_t address,uint8_t chx,uint8_t *flag);

#endif /* INCLUDES_ADC_H_ */
