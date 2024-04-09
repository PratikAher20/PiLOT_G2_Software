/*
 * ADC.h
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
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


uint8_t ADC_Init(i2c_instance_t *i2c_chx,uint8_t address);
uint16_t get_ADC_value(i2c_instance_t *i2c_chx,uint8_t address,uint8_t chx,uint8_t *flag);

#endif /* INCLUDES_ADC_H_ */
