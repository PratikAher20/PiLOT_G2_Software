/*
 * RS485_Controller.c
 *
 *  Created on: 23-Mar-2024
 *      Author: S-SPACE
 */

#include "RS485_Controller.h"

void GPIO2_IRQHandler( void ){
    uint16_t a, i = 0;
    a  = 1;
    uint16_t buf[1];
    buf[0] = 0;
    for(;i<1024;i++){

        HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) buf[0]);
    }
    NVIC_ClearPendingIRQ(GPIO2_IRQn);
    return ;
    //Start storing in SD_CARD
    //Clear the interrupt after reading a 256 block packet
}


uint16_t init_RS485_Controller(){
    MSS_GPIO_init();
    MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_set_output(MSS_GPIO_0, 1);
    MSS_GPIO_config(MSS_GPIO_2, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
    MSS_GPIO_enable_irq(MSS_GPIO_2);
    NVIC_EnableIRQ(GPIO2_IRQn);

    uint16_t buf[1];
    uint16_t waddr, i;
    buf[0] = 0;
    i = 0;

    HAL_set_8bit_reg(RS_485_Controller_0, WRITE_SLAVE_ADDR, (uint_fast8_t) SLAVE_ADDR);

    HAL_set_8bit_reg(RS_485_Controller_0, WRITE_CLKS_PER_BIT, (uint_fast8_t) CLKS_PER_BIT);

    waddr = HAL_get_16bit_reg(RS_485_Controller_0, READ_WADDR);

    for(;i<1024;i++){

        HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) buf[0]);
    }

    return waddr;
}


