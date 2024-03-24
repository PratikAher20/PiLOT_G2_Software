/*
 * main.c
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
 */

#include "main.h"

timer_instance_t t1;

void HK_ISR(){

	get_hk();
	TMR_clear_int(&t1);
}


int main(){

    //adf_init
    //gmc_init
    //sd_init
    //pslv_interface_init
    //interface_init
	p1_init();
//	uint16_t ax, ay, az;
//	uint16_t roll_rate, pitch_rate, yaw_rate;
//	uint16_t imu_temp;
	uint8_t result = 0, flag;
	uint32_t tmr_value;

	 MSS_UART_init(&g_mss_uart0,
	                   MSS_UART_9600_BAUD,
	                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);

	NVIC_ClearPendingIRQ(FabricIrq4_IRQn);
	TMR_init(&t1, CoreTimer_C0_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, HK_PKT_PERIOD);
	TMR_enable_int(&t1);
	NVIC_EnableIRQ( FabricIrq4_IRQn);
	NVIC_SetPriority(FabricIrq4_IRQn, 255);
	TMR_start(&t1);

	while(1){
//		result = get_IMU_acc(&ax, &ay, &az);
//		result = get_IMU_gyro(&roll_rate, &pitch_rate, &yaw_rate);
//		result = get_IMU_temp(&imu_temp);
//		HK_ISR();


		tmr_value = TMR_current_value(&t1);

	}

    uint8_t message[12] = "Hello World";

//    if(hk_time_expire){
//        if(wr_ptr + HK_PACKET_SIZE <= 255){
//            //Store HK_PKT in Active_blck
//        }
//    }


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
//    waddr = init_RS485_Controller();
//
//    cont = HAL_get_16bit_reg(RS_485_Controller_0, READ_CONST);

//    MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
//    MSS_TIM1_load_immediate(timer_count);
//    MSS_TIM1_start();
//
//    MSS_TIM64_init(MSS_TIMER_PERIODIC_MODE);



//    while(1){
//
//
//
//        cont = HAL_get_16bit_reg(RS_485_Controller_0, READ_CONST);
//
//        waddr = HAL_get_16bit_reg(RS_485_Controller_0, READ_WADDR);
//
//        raddr = HAL_get_16bit_reg(RS_485_Controller_0, READ_RADDR);
//
//        i = 0;
//
//        curr_value = MSS_TIM1_get_current_value();
//        if(curr_value < ICI){
//            data[0] = data[0] + 1;
//            for(;i<256;i++){
//
//                HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) data[i]);
//
//            }
//            MSS_TIM1_load_immediate(timer_count);
//        }
//        else{
//            curr_value = MSS_TIM1_get_current_value();
//        }
//    }

    return 0;

}

void FabricIrq0_IRQHandler(void)
{
    I2C_isr(&g_core_i2c0);
}

void FabricIrq1_IRQHandler(void)
{
    I2C_isr(&g_core_i2c1);
}

void FabricIrq2_IRQHandler(void)
{
    I2C_isr(&g_core_i2c2);
}

void FabricIrq3_IRQHandler(void)
{
    I2C_isr(&g_core_i2c3);
}

//void FabricIrq4_IRQHandler(void)
//{
//    I2C_isr(&g_core_i2c4);
//}
//
//void FabricIrq5_IRQHandler(void)
//{
//    I2C_isr(&g_core_i2c5);
//}

void FabricIrq4_IRQHandler(void)
{
    HK_ISR();

}
