/*
 * P1.c
 *
 *  Created on: 24-Mar-2024
 *      Author: S-SPACE
 */

#include "P1.h"

hk_pkt_t* hk_pkt;
thermistor_pkt_t* thermistor_pkt;
uint8_t data[512];
uint16_t hk_seq_num =0;
uint16_t temp_seq_num =0;
uint8_t RTM[16];
uint8_t latest_codeword = 0;
uint16_t blck_pkt[4][256];
uint8_t send_pkt_flg = 0;
uint8_t active_blck = 0;
uint8_t wri_ptr = 0;
uint8_t store_in_sd_card = 0;
extern uint8_t cmd_rx_count;
extern uint8_t cmd_rs485_succ_count;
extern uint8_t cmd_rs485_fail_count;
extern partition_t hk_partition;
extern partition_t comms_partition;
extern partition_t thermistor_partition;
extern uint8_t sd_dump;
extern uint8_t IMG_ID;
extern uint8_t reset_counts[1];
extern rx_cmd_t* rx_cmd_pkt;
extern uint8_t Time_Vector[32];
extern uint8_t CHK_CMD;
extern uint64_t current_time_upper,current_time_lower;
uint8_t cmd_cntr = 0;


//uint16_t data_test[256] = {0};

void timer_intr_dis(){
	NVIC_DisableIRQ(FabricIrq4_IRQn);
}

void timer_intr_en(){
	NVIC_EnableIRQ(FabricIrq4_IRQn);
}

void store_pkt(){
	uint16_t i = 0;


		for(;i<256;i++){

			HAL_set_16bit_reg(RS_485_Controller_0, WRITE_SRAM, (uint_fast16_t) blck_pkt[active_blck - 1][i]);

			if(i == 255){
				return;
			}
		}


}


void p1_init(){
	I2C_init(VC_SENSOR_I2C, COREI2C_0_0, VC1, I2C_PCLK_DIV_256);	//VC_Sensor
	I2C_init(IMU_CORE_I2C, COREI2C_3_0, IMU_ADDR, I2C_PCLK_DIV_256);	//IMU_Sensor
	I2C_init(TEMP_ADC_CORE_I2C, COREI2C_1_0, ADC_ADDR, I2C_PCLK_DIV_256);	//Temp_ADC_Sensor
}


void vGetPktStruct(pkt_name_t pktname, void* pktdata, uint8_t pktsize){

	uint8_t i =0;
	uint8_t* pkt_data;
	pkt_data = (uint8_t* )pktdata;

//	xQueueSend(Data_PKT_Queue, &pkt, 0);
	if(pktsize + wri_ptr >= 255){
//		send_pkt_flg = 1;
		active_blck += 1;
		store_pkt();
//		MSS_GPIO_set_output(MSS_GPIO_8, 1);
		wri_ptr = 0;
		if(active_blck == 4){
			active_blck = 0;
//			send_pkt_flg = 2;
		}
	}

	// Storing the packets

	for(;i<pktsize;i++){
		blck_pkt[active_blck][wri_ptr + i] = pkt_data[i] | (pkt_data[i] << 8);
	}
	wri_ptr += pktsize;

}

//void get_comms(){
//	uint16_t i = 1;
//
//	data_test[0]++;
//	for(;i<25;i++){
//		data_test[i] = i;
//		if(i == 24){
//			break;
//		}
//	}
//
//	i = 0;
//
//	vGetPktStruct(pld, (void*) data_test, sizeof(data_test));
//}


uint16_t make_FLetcher(uint8_t *data,uint16_t len) {
	uint8_t sumA = 0,sumB = 0,temp = 0;
	uint8_t i = 0;
	for(i = 0;i<len;i++) {
		sumA = (sumA + data[i]) % 255;
		sumB = (sumB + sumA) % 255;
	}
	temp = 255 - ((sumA + sumB) % 255);
	sumB = 255 - ((sumA + temp) % 255);

	return ((sumB << 8) | temp);

}

uint16_t get_hk(){
	hk_pkt = (hk_pkt_t* )data;
	uint16_t ax, ay, az;
	uint16_t roll_rate, pitch_rate, yaw_rate;
	uint16_t CDH_VC[2];
	uint16_t PIS_VC[2];
	uint16_t imu_temp;
	uint16_t result=0;
	uint8_t flag;
	uint8_t i = 0 ;
	uint8_t msg[18] = "\n\rGot HK Readings\0";
	uint16_t hk_status;

	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);

	result = (get_IMU_acc(&ax, &ay, &az) == 0 ? 0 : 1);
	result |= ((get_IMU_gyro(&roll_rate, &pitch_rate, &yaw_rate) == 0 ? 0 : 1) << 1);
	result |= ((get_IMU_temp(&imu_temp) == 0 ? : 1) << 2);
//	CDH_VC[0] = read_bus_voltage( VC1,  2, &flag);
//	PIS_VC[0] = read_bus_voltage(VC1, 3, &flag);
//	CDH_VC[1] = read_shunt_voltage(VC1, 2, &flag);
//	PIS_VC[1] = read_shunt_voltage(VC1, 3, &flag);

	hk_pkt->IMG_ID = IMG_ID;
	hk_pkt->CLK_RATE = MSS_SYS_M3_CLK_FREQ / 1000;
	hk_pkt->Command_Loss_Timer = MSS_WD_current_value();
	hk_pkt->Reset_Counts = reset_counts[0];
	hk_pkt->PREV_CMD_RX = rx_cmd_pkt->cmd_id;
	hk_pkt->Cmd_ADF_counts = cmd_rx_count;

	for(;i<16;i++){
		hk_pkt->RTM[i] = RTM[i];
	}

	hk_pkt->Cmd_RS485_Succ_counts = cmd_rs485_succ_count;
	hk_pkt->Cmd_RS485_Fail_counts = cmd_rs485_fail_count;
	hk_pkt->Acc[0] = ((ax));
	hk_pkt->Acc[1] = ((ay));
	hk_pkt->Acc[2] = ((az));
	hk_pkt->Angular_Rate[0] = roll_rate;
	hk_pkt->Angular_Rate[1] = pitch_rate;
	hk_pkt->Angular_Rate[2] = yaw_rate;
	hk_pkt->imu_temp = imu_temp;

//	hk_pkt->Sensor_Board_VC[0] = read_bus_voltage(VC1, 1, &flag);
//	hk_pkt->CDH_VC[0] = read_bus_voltage( VC1,  2, &flag);
//	hk_pkt->PIS_VC[0] = read_bus_voltage( VC1,  3, &flag);
	hk_pkt->Voltages[0] = read_bus_voltage(VC1, 2, &flag);
	result |= flag << 3;
	hk_pkt->Voltages[1] = read_bus_voltage(VC1, 3, &flag);
	result |= flag << 4;
//	hk_pkt->Sensor_Board_VC[1] = read_shunt_voltage(VC1, 1, &flag);
//	hk_pkt->CDH_VC[1] = read_shunt_voltage( VC1,  2, &flag);
//	hk_pkt->PIS_VC[1] = read_shunt_voltage( VC1,  3, &flag);

	hk_pkt->HK_Write_Pointer = hk_partition.write_pointer;
	hk_pkt->HK_Read_Pointer = hk_partition.read_pointer;
	hk_pkt->COMMS_Write_Pointer = comms_partition.write_pointer;
	hk_pkt->COMMS_Read_Pointer = comms_partition.read_pointer;
	hk_pkt->Thermistor_Write_Pointer = thermistor_partition.write_pointer;
	hk_pkt->Thermistor_Read_Pointer = thermistor_partition.read_pointer;

	hk_pkt->Currents[0] = read_shunt_voltage( VC1,  2, &flag);
	result |= flag << 5;
	hk_pkt->Currents[1] = read_shunt_voltage( VC1,  3, &flag);
	result |= flag << 6;

	hk_pkt->latest_codeword_rx = latest_codeword;

	get_time_vector(Time_Vector);
	for(;i<32;i++){
		hk_pkt->GTime_SVector[i] = Time_Vector[i];
	}

	hk_pkt->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p1(tlm_pkt_type, HK_API_ID))));
	hk_pkt->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p2((hk_seq_num++)))));
	hk_pkt->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p3(HK_PKT_LENGTH))));
	hk_pkt->ccsds_s2 = current_time_lower;
	hk_pkt->ccsds_s1 = current_time_upper;

	if(cmd_cntr == 20){
		cmd_cntr = 1;
		CHK_CMD = 0;
	}
	else{
		cmd_cntr++ ;
	}

	if(store_in_sd_card){
		sd_dump = 1;
		hk_pkt->sd_dump = sd_dump;
		hk_pkt->Fletcher_Code = make_FLetcher(data, sizeof(hk_pkt_t) - 2);
		result |= ((store_data(&hk_partition, data) == 0 ? 0 : 1) << 7);
		store_data(&hk_partition, data);
	}
	else{
		sd_dump = 0;
		hk_pkt->sd_dump = sd_dump;
		hk_pkt->Fletcher_Code = make_FLetcher(data, sizeof(hk_pkt_t) - 2);
//		vGetPktStruct(hk, (void*) hk_pkt, sizeof(hk_pkt_t));
		MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(hk_pkt_t));
	}

	return result;


//	MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(hk_pkt_t));
//	MSS_UART_polled_tx_string(&g_mss_uart0, msg);
}


uint16_t get_temp(){
	uint8_t i = 0;
	uint8_t flag;
	uint8_t sd_dump_thermistor = 0;
	uint16_t res = 0;
	thermistor_pkt = (thermistor_pkt_t*) data;

	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);

	thermistor_pkt->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p1(tlm_pkt_type, THERMISTOR_API_ID))));
	thermistor_pkt->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p2((temp_seq_num++)))));
	thermistor_pkt->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p3(THERMISTOR_PKT_LENGTH))));
	thermistor_pkt->ccsds_s2 = current_time_lower;
	thermistor_pkt->ccsds_s1 = current_time_upper;

	for(;i<8;i++){
		thermistor_pkt->Temperature_Values[i] = get_ADC_value(TEMP_ADC_CORE_I2C, ADC_ADDR, i, &flag);
		res |= (flag << i);
	}

	get_time_vector(Time_Vector);
	for(;i<32;i++){
		thermistor_pkt->GTime_SVector[i] = Time_Vector[i];
	}


	if(store_in_sd_card){
		sd_dump_thermistor = 1;
		thermistor_pkt->sd_dump = sd_dump_thermistor;
		thermistor_pkt->Fletcher_Code = make_FLetcher(data, sizeof(thermistor_pkt_t) - 2);
		store_data(&thermistor_partition, data);
	}
	else{
		sd_dump_thermistor = 0;
		thermistor_pkt->sd_dump = sd_dump_thermistor;
		thermistor_pkt->Fletcher_Code = make_FLetcher(data, sizeof(thermistor_pkt_t) - 2);
		vGetPktStruct(thermistor, (void*) thermistor_pkt, sizeof(thermistor_pkt_t));
//		MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(thermistor_pkt_t));
	}

	return res;
}

void get_sd_data(){
	read_data(&hk_partition, data);
	MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(hk_pkt_t));
}
