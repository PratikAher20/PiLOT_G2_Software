/*
 * main.c
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
 */

#include "P1.h"
#include "cmd.h"
//#include "RS485_Controller.h"

//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2//IMG-2

timer_instance_t hk_timer;
timer_instance_t comms_timer;
timer_instance_t gmc_timer;
timer_instance_t temp_timer;
timer_instance_t sd_timer;

partition_t hk_partition;
partition_t comms_partition;
partition_t gmc_partition;
partition_t thermistor_partition;
extern uint8_t store_in_sd_card;
uint8_t IMG_ID = 0;
uint16_t rssi_cca;
uint16_t rssi;
uint8_t ERR_LOG = 0;
uint8_t cmd_rx_count = 0;
uint8_t cmd_succ_count = 0;
uint8_t cmd_reject_count = 0;
uint8_t cmd_rs485_succ_count = 0;
uint8_t cmd_rs485_fail_count = 0;
uint8_t sd_dump = 0;
uint8_t reset_counts[1] = {0};
rx_cmd_t* rx_cmd_pkt;
uint8_t Time_Vector[32];
uint16_t Read_TPSRAM_addr;


//Declare variables to hold the current 64 bit timer counts
uint64_t current_time_upper,current_time_lower;

//Declare data buffer to store log data
uint8_t log_data[512];

//Declare log packet pointer
log_packet_t *log_packet_ptr;

//Declare log counter
uint8_t log_counter;

void HK_ISR(){
	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);

	get_hk();
//	if(log_counter >= 10) {
////		form_log_packet();
//	}
	//Need to add mutex here
//	log_packet_ptr->logs[log_counter].task_id = HK_TASK_ID;
//	log_packet_ptr->logs[log_counter].time_H = current_time_upper;
//	log_packet_ptr->logs[log_counter].time_L = current_time_lower;
//	log_packet_ptr->logs[log_counter].task_status = hk_status;
//	log_counter++;
	//Need to remove mutex here
	TMR_clear_int(&hk_timer);
}

void GMC_ISR(){
	uint8_t gmc_status;
	gmc_status = get_gmc();
	TMR_clear_int(&gmc_timer);
}

void COMMS_ISR(){
	uint16_t comms_status = get_comms();
	TMR_clear_int(&comms_timer);
}

void THER_ISR(){
	get_temp();
	TMR_clear_int(&temp_timer);
}

void SD_ISR(){
	get_sd_data();
	if(hk_partition.read_pointer < hk_partition.write_pointer && store_in_sd_card == 0){
		get_sd_data();
	}

	TMR_clear_int(&sd_timer);
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


	TMR_init(&temp_timer, CORETIMER_C2_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, TEMP_PKT_PERIOD);
	TMR_enable_int(&temp_timer);
	NVIC_EnableIRQ( FabricIrq6_IRQn);
	NVIC_SetPriority(FabricIrq6_IRQn, 254);

	TMR_init(&sd_timer, CORETIMER_C3_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, SD_PKT_PERIOD);
	TMR_enable_int(&sd_timer);
	NVIC_EnableIRQ( FabricIrq7_IRQn);
	NVIC_SetPriority(FabricIrq7_IRQn, 254);

	TMR_init(&gmc_timer, CORETIMER_C4_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, GMC_PKT_PERIOD);
	TMR_enable_int(&gmc_timer);
	NVIC_EnableIRQ( FabricIrq8_IRQn);
	NVIC_SetPriority(FabricIrq8_IRQn, 254);

	TMR_start(&hk_timer);
//	TMR_start(&comms_timer);
//	TMR_start(&temp_timer);
//	TMR_start(&sd_timer);
//	TMR_start(&gmc_timer);
}

void timer_dis(){
//	NVIC_DisableIRQ(FabricIrq4_IRQn);
	NVIC_DisableIRQ(FabricIrq5_IRQn);
}

void timer_ena(){
//	NVIC_EnableIRQ(FabricIrq4_IRQn);
	NVIC_EnableIRQ(FabricIrq5_IRQn);
}

void get_cmd(uint8_t* cmd, uint8_t src){

	rx_cmd_pkt = (rx_cmd_t*) cmd;

	if(cmd_valid(rx_cmd_pkt, src)){
		cmd_engine(rx_cmd_pkt);
		if(src == 0){
			cmd_rs485_succ_count++;
		}
		else{
			cmd_succ_count++;
		}


	}
	else{
		if(src == 0){
			cmd_rs485_fail_count++;
		}
		else{
			cmd_reject_count++;
		}

	}
}

void init_cmd_engine(){
	add_cmd(0, 2, cmd_noop);
	add_cmd(1, 3, set_pkt_rate);
	add_cmd(2, 3, exe_iap);
	add_cmd(3, 6, read_adf_reg);
	add_cmd(4, 16, exe_rtm);
}

//Function to initialise 64 bit timer
void Tim64_init() {
	MSS_TIM64_init(MSS_TIMER_ONE_SHOT_MODE);
	MSS_TIM64_load_immediate(0xFFFFFFFF,0xFFFFFFFF);
	MSS_TIM64_start();
}

int main(){

    //adf_init
    //gmc_init
    //sd_init
    //pslv_interface_init
    //interface_init
	MSS_WD_init();
	MSS_WD_reload();
	p1_init();

	initialise_partition(&hk_partition, HK_BLOCK_INIT, HK_BLOCK_END);
	initialise_partition(&comms_partition, COMMS_BLOCK_INIT, COMMS_BLOCK_END);
	initialise_partition(&thermistor_partition, THERMISTOR_BLOCK_INIT, THERMISTOR_BLOCK_END);
	initialise_partition(&gmc_partition, GMC_BLOCK_INIT, GMC_BLOCK_END);
	//Assign log packet pointer to log data buffer
	log_packet_ptr = (log_packet_t*)log_data;

	//Initialise log counter to zero
	log_counter = 0;

	//Function to initialise 64 bit timer
//	Tim64_init();

	ADC_Init(TEMP_ADC_CORE_I2C, ADC_ADDR);

	counter_init(&counter_i2c);
	ADC_Init(&counter_i2c, ADC_ADDR);

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
	uint16_t curr_tpsram_read_addr;
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
	MSS_GPIO_config(MSS_GPIO_10, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_set_output(MSS_GPIO_0, 1);
	uint32_t sd_addr = 10;
	uint8_t sd_data_write[512];
	uint8_t data_read[512];
	uint8_t sd_flg;
	uint32_t wd_reset;

	sd_data_write[0] = 0x21;
	sd_data_write[1] = 0x20;
	sd_data_write[3] = 0x19;
	sd_data_write[4] = 0x18;

	MSS_SPI_init(&g_mss_spi1);
	MSS_SPI_configure_master_mode(&g_mss_spi1, MSS_SPI_SLAVE_0, MSS_SPI_MODE0, 512, 8);

	sd_flg = SD_Init();

	vc_init(VC_SENSOR_I2C);
////
//	sd_flg = SD_Write(sd_addr, sd_data_write);
//
//	sd_flg = SD_Read(sd_addr, data_read);
//
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
	uint32_t CMD_CHK_TIMER = 0xFFFFFFFF - (MSS_SYS_M3_CLK_FREQ* (10));
	uint32_t curr_value = 0x0;

	MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
    MSS_TIM1_load_immediate(timer_count);
    MSS_TIM1_start();


	while(1){

		adf_send_cmd(CMD_PHY_CCA);

		get_rssi_cca_data(&rssi_cca);

		if(store_in_sd_card == 1){
			curr_tpsram_read_addr = HAL_get_16bit_reg(RS_485_Controller_0, READ_RADDR);
			if(curr_tpsram_read_addr > Read_TPSRAM_addr){
				store_in_sd_card = 0;
			}
		}
	}

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
    I2C_isr(&counter_i2c);
}

void FabricIrq3_IRQHandler(void)
{
    I2C_isr(&g_core_i2c3);
}

void FabricIrq4_IRQHandler(void)
{
    HK_ISR();

}

void FabricIrq5_IRQHandler(void)
{
    COMMS_ISR();
}

void FabricIrq6_IRQHandler(void)
{
    THER_ISR();
}

void FabricIrq7_IRQHandler(void)
{
    SD_ISR();
}

void FabricIrq8_IRQHandler(void)
{
    GMC_ISR();
}
