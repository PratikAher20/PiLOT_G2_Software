/**
 * @file RS485_Controller.c
 * @author Pratik A.
 * @brief Implemented Functions defined in RS485_Controller.h
 * @version 1.0
 * @date 2024-08-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "RS485_Controller.h"

extern rx_cmd_t* rx_cmd_pkt;
extern uint8_t cmd_rs485_succ_count;
extern uint8_t cmd_rs485_fail_count;
extern uint8_t store_in_sd_card;
extern timer_instance_t sd_timer;
extern uint8_t Time_Vector[32];

/**
 * @brief Number of TTPU IRQ's received.
 * 
 */
uint8_t no_of_TTPU_cmds = 0;

/**
 * @brief IRQ Handler invoked when the TPSRAM is full
 * 
 */
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

/**
 * @brief IRQ Handler invoked when the command is received from the TTPU.
 * TODO: Improve the Command Reception code to correctly receive all the parameters from the TTPU.
 * 
 */
void GPIO3_IRQHandler(void){

//	uint8_t cmd[32];
//	for(;i<32;i++){
//		cmd[i] = HAL_get_8bit_reg(RS_485_Controller_0, READ_SRAM_CMD);
//	}
	uint8_t i = 0;
	uint8_t cmd[32];
	no_of_TTPU_cmds++;

	if(no_of_TTPU_cmds == NUMBER_OF_REPRO_CMDS){
		cmd[0] = 0x03;
	}
	else{
		cmd[0] = 0x01;
	}
//	else if(no_of_TTPU_cmds == 2){
//
//	}
//	for(;i<32;i++){
//		cmd[i] = HAL_get_8bit_reg(APB_READ_CMD_0, READ_SRAM_CMD);
//	}
//
//	i = 0;
//
//	for(;i<32;i++){
//		cmd[i] = HAL_get_8bit_reg(APB_READ_CMD_0, READ_SRAM_CMD);
//	}
//
//	i = 0;
//
//	for(;i<32;i++){
//		cmd[i] = cmd[i+3];
//	}

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

void get_time_vector(){

	uint8_t i = 0;


	for(;i<32;i++){
		Time_Vector[i] = HAL_get_8bit_reg(APB_READ_TLM_0, READ_SRAM_CMD);
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


    buf[0] = HAL_get_8bit_reg(APB_READ_CMD_0, READ_SRAM_CMD);
	buf[0] = HAL_get_8bit_reg(APB_READ_TLM_0, READ_SRAM_CMD);

//    for(;i<1025;i++){
//
//        HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) buf[0]);
//    }

    return status;
}

