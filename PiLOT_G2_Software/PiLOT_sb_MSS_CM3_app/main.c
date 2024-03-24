/*
 * main.c
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
 */

#include "main.h"

uint16_t blck_pkt_1[256];
uint16_t blck_pkt_2[256];
uint16_t blck_pkt_3[256];
uint16_t blck_pkt_4[256];

uint16_t* active_blck;
uint8_t wr_ptr = 0;

int main(){

    //adf_init
    //gmc_init
    //sd_init
    //pslv_interface_init
    //interface_init

    active_blck = blck_pkt_1;
    uint8_t message[12] = "Hello World";

//    if(hk_time_expire){
//        if(wr_ptr + HK_PACKET_SIZE <= 255){
//            //Store HK_PKT in Active_blck
//        }
//    }


     MSS_UART_init(&g_mss_uart0,
                   MSS_UART_9600_BAUD,
                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

    uint16_t i = 1;
    uint16_t cont, waddr, raddr;
    uint16_t buf[256];
    buf[0] = 0;
    uint16_t data[256];

    data[0] = 0;
    for(;i<256;i++){
        data[i] = i;
    }


    uint32_t timer_count = 0xFFFFFFFF;
    uint32_t ICI = 0xFFFFFFFF - (MSS_SYS_M3_CLK_FREQ* (0.237567));
    uint32_t gpio_delay = 0xFFFFFFCD;
    uint32_t curr_value = 0x0;
    uint16_t data_temp = 0xABCD;


//  MSS_GPIO_set_output(MSS_GPIO_1, 1);
    waddr = init_RS485_Controller();

    cont = HAL_get_16bit_reg(RS_485_Controller_0, READ_CONST);

    MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
    MSS_TIM1_load_immediate(timer_count);
    MSS_TIM1_start();

    MSS_TIM64_init(MSS_TIMER_PERIODIC_MODE);



    while(1){

        cont = HAL_get_16bit_reg(RS_485_Controller_0, READ_CONST);

        waddr = HAL_get_16bit_reg(RS_485_Controller_0, READ_WADDR);

        raddr = HAL_get_16bit_reg(RS_485_Controller_0, READ_RADDR);

        i = 0;

        curr_value = MSS_TIM1_get_current_value();
        if(curr_value < ICI){
            data[0] = data[0] + 1;
            for(;i<256;i++){

                HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) data[i]);

            }
            MSS_TIM1_load_immediate(timer_count);
        }
        else{
            curr_value = MSS_TIM1_get_current_value();
        }
    }

    return 0;



}


