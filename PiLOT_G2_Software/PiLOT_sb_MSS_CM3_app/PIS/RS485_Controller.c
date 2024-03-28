/*
 * RS485_Controller.c
 *
 *  Created on: 23-Mar-2024
 *      Author: S-SPACE
 */

#include "RS485_Controller.h"


void GPIO1_IRQHandler( void ){
    uint16_t a, i = 0;
    a  = 1;
    uint16_t buf[1];
    uint16_t r_addr, w_addr;
    buf[0] = 0xFF;
//
		r_addr = HAL_get_16bit_reg(RS_485_Controller_0, READ_RADDR);
		w_addr = HAL_get_16bit_reg(RS_485_Controller_0, READ_WADDR);

//		MSS_GPIO_set_output(MSS_GPIO_8, 0);

//		for(;i<3;i++){
////
//			HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) buf[0]);
//		}
		r_addr = HAL_get_16bit_reg(RS_485_Controller_0, READ_RADDR);
		w_addr = HAL_get_16bit_reg(RS_485_Controller_0, READ_WADDR);
//        if(i == 255){

        	NVIC_ClearPendingIRQ(GPIO1_IRQn);
			return ;
//        }
//    }

    //Start storing in SD_CARD
    //Clear the interrupt after reading a 256 block packet
}


uint16_t init_RS485_Controller(){
    MSS_GPIO_init();
    MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(MSS_GPIO_8, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_set_output(MSS_GPIO_0, 1);
    MSS_GPIO_config(MSS_GPIO_1, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
    MSS_GPIO_enable_irq(MSS_GPIO_1);
    NVIC_EnableIRQ(GPIO1_IRQn);
    NVIC_SetPriority(GPIO1_IRQn, 255);
    uint16_t buf[1];
    uint16_t waddr, i;
    buf[0] = 0;
    i = 0;

    HAL_set_8bit_reg(RS_485_Controller_0, WRITE_SLAVE_ADDR, (uint_fast8_t) SLAVE_ADDR);

    HAL_set_8bit_reg(RS_485_Controller_0, WRITE_CLKS_PER_BIT, (uint_fast8_t) CLKS_PER_BIT);

    waddr = HAL_get_16bit_reg(RS_485_Controller_0, READ_WADDR);

//    for(;i<1025;i++){
//
//        HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) buf[0]);
//    }

    return waddr;
}

