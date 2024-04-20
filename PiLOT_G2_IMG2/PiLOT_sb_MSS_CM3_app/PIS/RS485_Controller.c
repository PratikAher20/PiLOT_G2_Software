/*
 * RS485_Controller.c
 *
 *  Created on: 23-Mar-2024
 *      Author: S-SPACE
 */

#include "RS485_Controller.h"

extern rx_cmd_t* rx_cmd_pkt;
extern uint8_t cmd_rs485_succ_count;
extern uint8_t cmd_rs485_fail_count;
extern uint8_t store_in_sd_card;
extern timer_instance_t sd_timer;

void GPIO1_IRQHandler( void ){
    uint16_t a, i = 0;
    a  = 1;
    uint16_t buf[1];
    uint16_t r_addr, w_addr;
    buf[0] = 0xFF;
//
		r_addr = HAL_get_16bit_reg(RS_485_Controller_0, READ_RADDR);
		w_addr = HAL_get_16bit_reg(RS_485_Controller_0, READ_WADDR);

		//Start storing the packets in sd card

		store_in_sd_card = 1;


		MSS_GPIO_clear_irq(MSS_GPIO_1);

		return ;


    //Start storing in SD_CARD
    //Clear the interrupt after reading a 256 block packet
}

void GPIO3_IRQHandler(void){

//	uint8_t cmd[32];
//	for(;i<32;i++){
//		cmd[i] = HAL_get_8bit_reg(RS_485_Controller_0, READ_SRAM_CMD);
//	}
	uint8_t i = 0;
	uint8_t cmd[32];
	for(;i<32;i++){
		cmd[i] = HAL_get_8bit_reg(APB_READ_CMD_0, READ_SRAM_CMD);
	}

	for(;i<32;i++){
		cmd[i] = HAL_get_8bit_reg(APB_READ_CMD_0, READ_SRAM_CMD);
	}

	get_cmd(cmd, 0);

//	rx_cmd_pkt = (rx_cmd_t*) cmd;
//
//	if(cmd_valid(rx_cmd_pkt)){
//		cmd_engine(rx_cmd_pkt);
//		cmd_rs485_succ_count++;
//	}
//	else{
//		cmd_rs485_fail_count++;
//	}


	MSS_GPIO_clear_irq( MSS_GPIO_3);

}

void get_time_vector(uint8_t* time_vect){

	uint8_t i = 0;

	for(;i<32;i++){
		time_vect[i] = HAL_get_8bit_reg(APB_READ_TLM_0, READ_RADDR);
	 }
	 i=0;
	 for(;i<32;i++){
		 time_vect[i] = HAL_get_8bit_reg(APB_READ_TLM_0, READ_RADDR);
	 }
}


uint8_t init_RS485_Controller(){

    MSS_GPIO_config(MSS_GPIO_1, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);		//SRAM Full INTR
    MSS_GPIO_enable_irq(MSS_GPIO_1);
    NVIC_EnableIRQ(GPIO1_IRQn);
    NVIC_SetPriority(GPIO1_IRQn, 255);

    MSS_GPIO_config(MSS_GPIO_3, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_NEGATIVE);		//RS485_cmd_INTR
	MSS_GPIO_enable_irq(MSS_GPIO_3);
	NVIC_EnableIRQ(GPIO3_IRQn);
	NVIC_SetPriority(GPIO3_IRQn, 253);
    uint16_t buf[1];
    uint16_t waddr, i, raddr;
    buf[0] = 0;
    i = 0;
    uint8_t cont;
    uint8_t status;

    HAL_set_8bit_reg(RS_485_Controller_0, WRITE_SLAVE_ADDR, (uint_fast8_t) SLAVE_ADDR);

    HAL_set_8bit_reg(RS_485_Controller_0, WRITE_CLKS_PER_BIT, (uint_fast8_t) CLKS_PER_BIT);

    HAL_set_8bit_reg(APB_READ_CMD_0, WRITE_PAY_ID, (uint_fast8_t) PAY_ID);

    cont = HAL_get_8bit_reg(RS_485_Controller_0, READ_CONST);
    if(cont != 0xab){
    	status |= 0x01;
    	status  = status << 1;
    }
    status  = status << 1;
    cont = HAL_get_8bit_reg(APB_READ_CMD_0, READ_CONST);
    if(cont != 0xab){
		status |= 0x01;
		status  = status << 1;
	}
	status  = status << 1;
    cont = HAL_get_8bit_reg(APB_READ_TLM_0, READ_CONST);
    if(cont != 0xab){
		status |= 0x01;
		status  = status << 1;
	}
	status  = status << 1;

	raddr = HAL_get_16bit_reg(RS_485_CONTROLLER_0, READ_RADDR);

	if(raddr != 0){
		status |= 0x01;
		status  = status << 1;
	}
	status  = status << 1;

	waddr = HAL_get_16bit_reg(RS_485_Controller_0, READ_WADDR);
	if(waddr != 2){
		status |= 0x01;
		status  = status << 1;
	}
	status  = status << 1;


    buf[0] = HAL_get_8bit_reg(APB_READ_CMD_0, READ_RADDR);
	buf[0] = HAL_get_8bit_reg(APB_READ_TLM_0, READ_RADDR);

//    for(;i<1025;i++){
//
//        HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) buf[0]);
//    }

    return status;
}

