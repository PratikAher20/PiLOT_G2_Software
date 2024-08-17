/**
 * @file    :   counter.h
* @author   :
 * @brief   :   This file contains the drivers to interface with the counter IC(S-35770E01I-K8T2U)
 * @version :   1.0
 * @date    :   2024-03-04
 *
 * @copyright Copyright (c) 2023
 *
 */


/*
 * This is the driver for the GMC counter (built with S-35770E01I-K8T2U)
 * define following variables
 * 1. RESET_GMC_GPIO for the GPIO reset pin of the counter
 * 2. base_addr for the I2C base address
 * 3. counter address will be 0x32 as long as we are using the same chip
 * 4. DUMMY_SLAVE_ADDRESS is address is the address for using the I2C in MSS as slave
 * */

#include "core_i2c.h"
#include "mss_gpio.h"
#include "CoreI2C/core_i2c.h"
#include "PiLOT_G2_hw_platform.h"
#include "ADC.h"
//#include "peripherals.h"


#define     DUMMY_SLAVE_ADDRESS     0x10
#define     counter_addr            0x32
#define     base_addr               COREI2C_2_0
#define     RESET_GMC_GPIO          MSS_GPIO_6
#define 	GMC_ADC_address 			0x23

i2c_instance_t counter_i2c;


// function to initialize IC
i2c_status_t counter_init(i2c_instance_t *i2c_GMC_counter);

// To reset counter from Software(it writes 010 in RST2, RST1, RST0 of the counter IC's free register to reset the IC(source: IC Datasheet pg.13))
i2c_status_t reset_counter(i2c_instance_t *i2c_GMC_counter); //should i return uint8_t 0 for successful reset


// function to get the current value of count
i2c_status_t get_count(i2c_instance_t *i2c_GMC_counter, uint8_t* count);

//to get values of free register
i2c_status_t set_free_res(i2c_instance_t *i2c_GMC_counter, uint8_t byte1, uint8_t byte2, uint8_t byte3);

i2c_status_t get_free_res(i2c_instance_t *i2c_GMC_counter, uint8_t* free_res);

i2c_status_t get_gmc_voltages(i2c_instance_t *i2c_GMC_ADC, uint8_t* ADC_voltages);
