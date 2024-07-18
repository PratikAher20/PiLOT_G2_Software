/*
 * main.c
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
 */

#include "P1.h"
#include "cmd.h"
//#include "RS485_Controller.h"

//Golden Image
//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1//IMG-1

extern uint8_t data[512];
init_packet_t* init_pkt;
timer_instance_t hk_timer;
timer_instance_t comms_timer;
timer_instance_t gmc_timer;
timer_instance_t temp_timer;
timer_instance_t sd_timer;

partition_t hk_partition;
partition_t comms_partition;
partition_t gmc_partition;
partition_t thermistor_partition;

//Create partition for log packets
partition_t log_partiton;
extern uint8_t store_in_sd_card;
extern uint32_t REPRO_CODE_WORD_ADDR;
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
uint8_t CHK_CMD = 0;
uint8_t stat1 = 0;
uint8_t stat2 = 0;
uint8_t adf_status = 0;

//Declare variables to hold the current 64 bit timer counts
uint64_t current_time_upper,current_time_lower;

//Declare data buffer to store log data
uint8_t log_data[512];

//Declare log packet pointer
log_packet_t *log_packet_ptr;

//Declare log counter
uint8_t log_counter;

//Declare logs sequence number variable
uint16_t logs_seq_no;

//Declare variable for status for each packet
uint16_t hk_status,gmc_status,comms_status,therm_status;

//Function to form Log packet
void form_log_packet() {
	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);
	log_packet_ptr->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
	log_packet_ptr->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
	log_packet_ptr->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
	log_packet_ptr->ccsds_s2 = current_time_lower;
	log_packet_ptr->ccsds_s1 = current_time_upper;
	log_packet_ptr->Fletcher_Code = make_FLetcher(log_data,LOGS_PKT_LENGTH-2);
	//Need to replace the below l
	// add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data,&logs_miss,LOGS_TASK_ID);
	if(store_in_sd_card){
		store_data(&log_partiton, log_data);
	}
	else{
//		vGetPktStruct(logs, (void*) log_packet_ptr, sizeof(log_packet_t));
		MSS_UART_polled_tx(&g_mss_uart0, log_data, sizeof(log_packet_t));
	}
	log_counter = 0;
	logs_seq_no++;
}
void HK_ISR(){
	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);

	hk_status = get_hk();
	if(log_counter >= 10) {
		form_log_packet();
	}
	log_packet_ptr->logs[log_counter].task_id = HK_TASK_ID;
	log_packet_ptr->logs[log_counter].time_H = current_time_upper;
	log_packet_ptr->logs[log_counter].time_L = current_time_lower;
	log_packet_ptr->logs[log_counter].task_status = hk_status;
	log_counter++;
	TMR_clear_int(&hk_timer);
}

void GMC_ISR(){
	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);
	gmc_status = get_gmc();
	if(log_counter >= 10) {
		form_log_packet();
	}
	log_packet_ptr->logs[log_counter].task_id = GMC_TASK_ID;
	log_packet_ptr->logs[log_counter].time_H = current_time_upper;
	log_packet_ptr->logs[log_counter].time_L = current_time_lower;
	log_packet_ptr->logs[log_counter].task_status = gmc_status;
	log_counter++;
	TMR_clear_int(&gmc_timer);
}

void COMMS_ISR(){
	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);
	comms_status = get_comms();
	if(log_counter >= 10) {
		form_log_packet();
	}
	log_packet_ptr->logs[log_counter].task_id = COMMS_TASK_ID;
	log_packet_ptr->logs[log_counter].time_H = current_time_upper;
	log_packet_ptr->logs[log_counter].time_L = current_time_lower;
	log_packet_ptr->logs[log_counter].task_status = comms_status;
	log_counter++;
	TMR_clear_int(&comms_timer);
}

void THER_ISR(){
	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);
	therm_status =  get_temp();
	if(log_counter >= 10) {
		form_log_packet();
	}
	log_packet_ptr->logs[log_counter].task_id = THERMISTOR_TASK_ID;
	log_packet_ptr->logs[log_counter].time_H = current_time_upper;
	log_packet_ptr->logs[log_counter].time_L = current_time_lower;
	log_packet_ptr->logs[log_counter].task_status = therm_status;
	log_counter++;
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
	TMR_start(&comms_timer);
	TMR_start(&temp_timer);
	TMR_start(&sd_timer);
	TMR_start(&gmc_timer);
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


void get_init(){
	init_pkt = (init_packet_t* )data;
	init_pkt->Status_1 = stat1;
	init_pkt->Adf_init_status = adf_status;
	init_pkt->status_2 = stat2;
	init_pkt->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p1(tlm_pkt_type, INIT_API_ID))));
	init_pkt->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p2((0)))));
	init_pkt->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p3(INIT_PKT_LENGTH))));
	init_pkt->ccsds_s1 = 0;
	init_pkt->ccsds_s2 = 0;

	uint8_t i = 0;

	get_time_vector(Time_Vector);
	for(;i<32;i++){
		init_pkt->GTime_SVector[i] = Time_Vector[i];
	}
	init_pkt->Fletcher_Code = make_FLetcher(data, sizeof(init_packet_t) - 2);


//	vGetPktStruct(init, (void*) init_pkt, sizeof(init_packet_t));
	MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(init_packet_t));


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
	MSS_GPIO_init();
	MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(MSS_GPIO_6, MSS_GPIO_OUTPUT_MODE);

	MSS_GPIO_set_output(MSS_GPIO_0, 1);	//Control_Interface ON
	MSS_GPIO_set_output(MSS_GPIO_2, 1);	//COmms_ON
	MSS_GPIO_set_output(MSS_GPIO_6, 1);	//GMC_on

	MSS_SYS_init(MSS_SYS_NO_EVENT_HANDLER);
	MSS_SPI_init( &g_mss_spi0 );
	MSS_SPI_configure_master_mode(&g_mss_spi0, MSS_SPI_SLAVE_0, MSS_SPI_MODE0, 8u, 8);

	MSS_SPI_init(&g_mss_spi1);
	MSS_SPI_configure_master_mode(&g_mss_spi1, MSS_SPI_SLAVE_0, MSS_SPI_MODE0, 512, 8);

	initialise_partition(&hk_partition, HK_BLOCK_INIT, HK_BLOCK_END);
	initialise_partition(&comms_partition, COMMS_BLOCK_INIT, COMMS_BLOCK_END);
	initialise_partition(&thermistor_partition, THERMISTOR_BLOCK_INIT, THERMISTOR_BLOCK_END);
	initialise_partition(&gmc_partition, GMC_BLOCK_INIT, GMC_BLOCK_END);
	initialise_partition(&log_partiton, LOGS_BLOCK_INIT, LOGS_BLOCK_END);

	uint8_t init_envm[3];
	init_envm[0] = 0x00;
	init_envm[1] = 0x00;
	init_envm[2] = 0x00;
	NVM_write(REPRO_CODE_WORD_ADDR, init_envm, 3, NVM_DO_NOT_LOCK_PAGE);

	//Assign log packet pointer to log data buffer
	log_packet_ptr = (log_packet_t*)log_data;

	//Initialise log counter to zero
	log_counter = 0;

	//Initailise the log sequence number
	logs_seq_no = 1;

	//Function to initialise 64 bit timer
	Tim64_init();

	uint8_t mode = 0;
	counter_init(&counter_i2c);

	stat1 |= ADC_Init(TEMP_ADC_CORE_I2C, ADC_ADDR);
	stat1 = (stat1 << 1);
	stat1 |= ADC_Init(&counter_i2c, ADC_ADDR);
	stat1 = (stat1 << 1);
	stat1 |= SD_Init();
	stat1 = (stat1 << 5);
	stat1 |= init_RS485_Controller();

	adf_status = adf_init();
	mode = adf_get_state();


	stat2 |= mode;
	stat2 = (stat2 << 1);
	stat2 |= vc_init(VC1);
	stat2 = (stat2 << 1);


	uint16_t curr_tpsram_read_addr;
	uint16_t rssi;
	uint8_t cmd[8];
	uint8_t cmd_rx_flag = 0;


	uint32_t wd_reset;

	init_cmd_engine();


	 MSS_UART_init(&g_mss_uart0,
	                   MSS_UART_9600_BAUD,
	                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);


	NVIC_ClearPendingIRQ(FabricIrq4_IRQn);
	NVIC_ClearPendingIRQ(FabricIrq5_IRQn);

	timer_intr_set();

	uint32_t curr_value = 0x0;

//	MSS_TIM1_init(MSS_TIMER_PERIODIC_MODE);
//    MSS_TIM1_load_immediate(timer_count);
//    MSS_TIM1_start();

	//Sending Init packet

	get_init();


	while(1){

		CHK_CMD = 1;

		adf_send_cmd(CMD_PHY_RX);
//
//		curr_value = MSS_TIM1_get_current_value();
//
		while(CHK_CMD){
//
			rx_pkt(cmd, &rssi, &cmd_rx_flag);
			if(cmd_rx_flag == 1){
				cmd_rx_count++;
				cmd_rx_flag = 0;
				get_cmd(cmd, 1);
				MSS_WD_reload();
				break;
			}
//
			wd_reset = MSS_WD_timeout_occured();
			  if(wd_reset)
			  {
				  reset_counts[0]++;
				  NVM_write(ENVM_RESET_COUNT_ADDR, reset_counts, sizeof(reset_counts), NVM_DO_NOT_LOCK_PAGE);
				  MSS_WD_clear_timeout_event();
			  }

			MSS_WD_reload();

//			curr_value = MSS_TIM1_get_current_value();
		}
//
		adf_send_cmd(CMD_PHY_ON);
		adf_send_cmd(CMD_PHY_CCA);

		timer_dis();
		get_rssi_cca_data(&rssi_cca);
		timer_ena();
//
//		curr_value = MSS_TIM1_get_current_value();
//
//		MSS_TIM1_load_immediate(timer_count);
//
//
		if(store_in_sd_card == 1){
			curr_tpsram_read_addr = HAL_get_16bit_reg(RS_485_Controller_0, READ_RADDR);
			if(curr_tpsram_read_addr > Read_TPSRAM_addr){
				store_in_sd_card = 0;
			}
		}
//		//Get _in Rx
//		//timer_start
//		//rx_pkt -> check_for_100_tries --- continue until timer expires
//		//read_rssi_cca
//		//reload_timer
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
