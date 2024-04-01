/*
 * main.c
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
 */

#include "P1.h"
#include "cmd.h"
//#include "RS485_Controller.h"

timer_instance_t hk_timer;
timer_instance_t comms_timer;


uint16_t rssi_cca;
uint16_t rssi;
uint8_t ERR_LOG = 0;
uint8_t cmd_rx_count = 0;
uint8_t cmd_succ_count = 0;
uint8_t cmd_reject_count = 0;
rx_cmd_t* rx_cmd_pkt;

void HK_ISR(){

	get_hk();
	TMR_clear_int(&hk_timer);
}

void COMMS_ISR(){

	get_comms();
	TMR_clear_int(&comms_timer);
}

void timer_intr_set(){
	TMR_init(&hk_timer, CORETIMER_C0_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, HK_PKT_PERIOD);
	TMR_enable_int(&hk_timer);
	NVIC_EnableIRQ( FabricIrq4_IRQn);
	NVIC_SetPriority(FabricIrq4_IRQn, 254);

	TMR_init(&comms_timer, CORETIMER_C1_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, COMMS_PKT_PERIOD);
	TMR_enable_int(&comms_timer);
	NVIC_EnableIRQ( FabricIrq5_IRQn);
	NVIC_SetPriority(FabricIrq5_IRQn, 254);

	TMR_start(&hk_timer);
	TMR_start(&comms_timer);
}

void timer_dis(){
//	NVIC_DisableIRQ(FabricIrq4_IRQn);
	NVIC_DisableIRQ(FabricIrq5_IRQn);
}

void timer_ena(){
//	NVIC_EnableIRQ(FabricIrq4_IRQn);
	NVIC_EnableIRQ(FabricIrq5_IRQn);
}

void get_cmd(uint8_t* cmd){

	rx_cmd_pkt = (rx_cmd_t*) cmd;

	if(cmd_valid(rx_cmd_pkt)){
		cmd_engine(rx_cmd_pkt);
		cmd_succ_count++;
	}
	else{
		cmd_reject_count++;
	}
}

void init_cmd_engine(){
	add_cmd(0, 2, cmd_noop);
	add_cmd(1, 3, set_pkt_rate);
	add_cmd(2, 3, exe_iap);
}

int main(){

    //adf_init
    //gmc_init
    //sd_init
    //pslv_interface_init
    //interface_init
	p1_init();
	uint16_t ax, ay, az;
	uint16_t roll_rate, pitch_rate, yaw_rate;
	uint16_t imu_temp;
	uint8_t result = 0, flag;
	uint32_t tmr_value;
	uint16_t i = 1;
	uint16_t cont, waddr, raddr;
	uint16_t buf[256];
	buf[0] = 0;
	uint16_t data[256];
	uint16_t rssi;
	uint8_t mode;
	uint32_t freq;

	uint8_t cmd;
	uint8_t cmd_rx_flag = 0;

	MSS_SYS_init(MSS_SYS_NO_EVENT_HANDLER);
	MSS_SPI_init( &g_mss_spi0 );
	MSS_SPI_configure_master_mode
		(
			&g_mss_spi0,
			MSS_SPI_SLAVE_0,
			MSS_SPI_MODE0,
			8u,
			8
		);
	MSS_GPIO_init();
	MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_set_output(MSS_GPIO_0, 1);

	init_cmd_engine();

	cont = HAL_get_16bit_reg(RS_485_Controller_0, READ_CONST);


	waddr = init_RS485_Controller();

	adf_init();

	mode = adf_get_state();

	 MSS_UART_init(&g_mss_uart0,
	                   MSS_UART_9600_BAUD,
	                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);


	NVIC_ClearPendingIRQ(FabricIrq4_IRQn);
	NVIC_ClearPendingIRQ(FabricIrq5_IRQn);

	timer_intr_set();

	uint32_t timer_count = 0xFFFFFFFF;
	uint32_t ICI = 0xFFFFFFFF - (MSS_SYS_M3_CLK_FREQ* (10));
	uint32_t curr_value = 0x0;

//	MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
//    MSS_TIM1_load_immediate(timer_count);
//    MSS_TIM1_start();


//    while(1){
//    	adf_send_cmd(CMD_PHY_RX);
//    	rx_pkt(&cmd, &rssi, &cmd_rx_flag);
//    }


	while(1){
//		result = get_IMU_acc(&ax, &ay, &az);
//		result = get_IMU_gyro(&roll_rate, &pitch_rate, &yaw_rate);
//		result = get_IMU_temp(&imu_temp);
//		HK_ISR();

//Just Keep Checking For Commands
//		tmr_value = TMR_current_value(&t1);
		adf_send_cmd(CMD_PHY_RX);

		curr_value = MSS_TIM1_get_current_value();

		while(curr_value > ICI){

			rx_pkt(&cmd, &rssi, &cmd_rx_flag);
			if(cmd_rx_flag == 1){
				cmd_rx_count++;
				cmd_rx_flag = 0;
				get_cmd(&cmd);
				break;
			}

			curr_value = MSS_TIM1_get_current_value();
		}

		adf_send_cmd(CMD_PHY_ON);
		adf_send_cmd(CMD_PHY_CCA);

		get_rssi_cca_data(&rssi_cca);

		MSS_TIM1_load_immediate(timer_count);
		//Get _in Rx
		//timer_start
		//rx_pkt -> check_for_100_tries --- continue until timer expires
		//read_rssi_cca
		//reload_timer
	}

    uint8_t message[12] = "Hello World";

//    if(hk_time_expire){
//        if(wr_ptr + HK_PACKET_SIZE <= 255){
//            //Store HK_PKT in Active_blck
//        }
//    }




    data[0] = 0;
    for(;i<256;i++){
        data[i] = i;
    }



    uint32_t gpio_delay = 0xFFFFFFCD;
    uint16_t data_temp = 0xABCD;


//  MSS_GPIO_set_output(MSS_GPIO_1, 1);
//    waddr = init_RS485_Controller();
////
//    cont = HAL_get_16bit_reg(RS_485_Controller_0, READ_CONST);
//
//    MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
//    MSS_TIM1_load_immediate(timer_count);
//    MSS_TIM1_start();
//
//    MSS_TIM64_init(MSS_TIMER_PERIODIC_MODE);

//    while(1){
////    	store_pkt();
//    	tmr_value = TMR_current_value(&t1);
//    }


//    while(1){
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

void FabricIrq5_IRQHandler(void)
{
    COMMS_ISR();
}
