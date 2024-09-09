/**
 * @file 	:			counter.c
 * @author 	:			Abhishek Verma, Pratik A.
 * @brief 	:			This is the driver for the GMC counter IC(S-35770E1I-K8T2U)
 * @version :			1.0
 * @date 	:			2024-03-04
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "counter.h"

uint8_t rx_size = 3u;
i2c_status_t status;
uint8_t flag;
uint8_t gmc_voltage_flags;

i2c_status_t counter_init(i2c_instance_t *i2c_GMC_counter){
    MSS_GPIO_config(RESET_GMC_GPIO, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_set_output(RESET_GMC_GPIO, 1);
	I2C_init(
				i2c_GMC_counter,
				base_addr,
				DUMMY_SLAVE_ADDRESS,
				I2C_PCLK_DIV_256
				);
	status = I2C_wait_complete(i2c_GMC_counter, I2C_NO_TIMEOUT);
	return status;
}

i2c_status_t reset_counter(i2c_instance_t *i2c_GMC_counter){
    uint8_t tx_size = 4;
	uint8_t tx_buffer[4];
	tx_buffer[0]=0x81; //1xxxxxx1 address pointer and test bit(why sending this byte after slave address?)
	tx_buffer[1]=0x00; //byte 1 for free register 0
	tx_buffer[2]=0x00; //byte 2 for free register 0
	tx_buffer[3]=0x02;  //byte 3 for free register 00000010 RST2=0, RST1=1, RST0=0 for resetting
	I2C_write(i2c_GMC_counter, counter_addr, tx_buffer, tx_size, I2C_RELEASE_BUS);
	status = I2C_wait_complete(i2c_GMC_counter, I2C_NO_TIMEOUT);
	return status;
}

i2c_status_t get_count(i2c_instance_t *i2c_GMC_counter, uint8_t* count){
	I2C_read(i2c_GMC_counter, counter_addr, count, rx_size, I2C_RELEASE_BUS);
	status = I2C_wait_complete(i2c_GMC_counter, I2C_NO_TIMEOUT);
	return status;
}

i2c_status_t set_free_res(i2c_instance_t *i2c_GMC_counter, uint8_t byte1, uint8_t byte2, uint8_t byte3){
    uint8_t tx_size = 4;
    uint8_t tx_buffer[4];
    tx_buffer[0] = 0x81;  //1xxxxxx1 address pointer and test bit(why sending this byte after slave address?)
    tx_buffer[1] = byte1; //byte 1 for free register 0
    tx_buffer[2] = byte2; //byte 2 for free register 0
    tx_buffer[3] = byte3; //not to reset avoid this :- byte 3 for free register 00000010 RST2=0, RST1=1, RST0=0 for resetting

    I2C_write(i2c_GMC_counter, counter_addr, tx_buffer, tx_size, I2C_RELEASE_BUS);
    status = I2C_wait_complete(i2c_GMC_counter, I2C_NO_TIMEOUT);
    return status;
}

i2c_status_t get_free_res(i2c_instance_t *i2c_GMC_counter, uint8_t* free_res){
    uint8_t tx_size = 1;
    uint8_t tx_buffer[1];
    tx_buffer[0] = 0x01;  //1xxxxxx1 address pointer and test bit(why sending this byte after slave address?)

    I2C_write(i2c_GMC_counter, counter_addr, tx_buffer, tx_size, I2C_HOLD_BUS);
    status = I2C_wait_complete(i2c_GMC_counter, I2C_NO_TIMEOUT);


    I2C_read(i2c_GMC_counter, counter_addr, free_res, rx_size, I2C_RELEASE_BUS);
    status = I2C_wait_complete(i2c_GMC_counter, I2C_NO_TIMEOUT);
    return status;
}

i2c_status_t get_gmc_voltages(i2c_instance_t *i2c_GMC_ADC, uint16_t* ADC_voltages){
	uint8_t channel;
	for (channel=0; channel<8; channel++){
		ADC_voltages[channel] = get_ADC_value(&counter_i2c, GMC_ADC_address, channel, &flag);
		gmc_voltage_flags = gmc_voltage_flags << 1 | flag;
	}
}
