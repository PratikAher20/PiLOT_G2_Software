/*
 * P2.c
 *
 *  Created on: 26-Mar-2024
 *      Author: S-SPACE
 */

#include "P2.h"

extern uint16_t rssi;
extern uint16_t rssi_cca;
extern uint8_t cmd_rx_count;
extern uint8_t cmd_succ_count;
extern uint8_t data[512];
extern uint8_t cmd_reject_count;
extern partition_t comms_partition;
extern partition_t gmc_partition;
extern uint8_t store_in_sd_card;
extern uint8_t IMG_ID;
extern uint64_t current_time_upper,current_time_lower;

uint8_t sd_dump_comms = 0;
uint8_t sd_dump_gmc = 0;
uint16_t data_test[25] = {0};
comms_pkt_t* comms_pkt;
gmc_pkt_t* gmc_pkt;
uint16_t comms_seq_num = 0;
uint16_t gmc_seq_num = 0;
uint32_t cmd_adf_read_addr = 0;
uint8_t cmd_adf_read_No_double_words = 0;
uint32_t cmd_adf_data[8];

uint8_t count_val[3];
uint8_t free_res[3];
uint32_t gmc_radiation_count;
uint32_t gmc_free_res_value;

uint8_t status;
uint16_t ADC_voltages[8];


uint8_t get_gmc(){
	uint16_t i = 0;
	gmc_pkt = (gmc_pkt_t*) data;
	status = get_count(&counter_i2c, count_val);
	status = status << 1;
	gmc_radiation_count = count_val[0];
	gmc_radiation_count = gmc_radiation_count << 8;
	gmc_radiation_count = gmc_radiation_count | count_val[1];
	gmc_radiation_count = gmc_radiation_count << 8;
	gmc_radiation_count = gmc_radiation_count | count_val[2];



	status = get_free_res(&counter_i2c, free_res);
	status = status << 1;
	gmc_free_res_value = count_val[0];
	gmc_free_res_value = gmc_free_res_value << 8;
	gmc_free_res_value = gmc_free_res_value | count_val[1];
	gmc_free_res_value = gmc_free_res_value << 8;
	gmc_free_res_value = gmc_free_res_value | count_val[2];

	get_gmc_voltages(&counter_i2c, ADC_voltages);

	gmc_pkt->IMG_ID = IMG_ID;
	gmc_pkt->Radiation_Counts = gmc_radiation_count;
	gmc_pkt->Counter_Free_Register = gmc_free_res_value;
	for (i=0; i<8; i++){
	gmc_pkt->GMC_Voltage_ADC[i] = ADC_voltages[i];
	}

//	get_time_vector(Time_Vector);

	i = 0;
//	for(;i<32;i++){
//		gmc_pkt->GMC_GTime_SVector[i] = Time_Vector[i];
//	}

	gmc_pkt->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p1(tlm_pkt_type, GMC_API_ID))));
	gmc_pkt->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p2((gmc_seq_num++)))));
	gmc_pkt->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p3(GMC_PKT_LENGTH))));
	gmc_pkt->ccsds_s1 = 0;

	gmc_pkt->ccsds_s2 = 0;

	if(store_in_sd_card){
			sd_dump_gmc = 1;
			gmc_pkt->GMC_sd_dump = sd_dump_gmc;
			gmc_pkt->Fletcher_Code = make_FLetcher(data, sizeof(gmc_pkt_t) - 2);
			store_data(&gmc_partition, data);
	//		store_in_sd_card = 0;
		}
		else{
			sd_dump_gmc = 0;
			gmc_pkt->GMC_sd_dump = sd_dump_gmc;
			gmc_pkt->Fletcher_Code = make_FLetcher(data, sizeof(gmc_pkt_t) - 2);
	//		vGetPktStruct(comms, (void*) comms_pkt, sizeof(comms_pkt_t));
			MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(gmc_pkt_t));
		}


	return status;
}

uint16_t get_comms(){
	uint16_t i = 0;
	comms_pkt = (comms_pkt_t*) data;
//	data_test[0]++;

	MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);

	for(;i<8;i++){
		cmd_adf_data[i] = 0;
	}
	i = 0;
	get_rssi_data(&rssi);
	comms_pkt->IMG_ID = IMG_ID;
	comms_pkt->comms_adf_rssi = rssi;
	comms_pkt->comms_adf_cmd_rx = cmd_rx_count;
	comms_pkt->comms_adf_cmd_succ = cmd_succ_count;
	comms_pkt->comms_adf_cmd_reject = cmd_reject_count;
//	adf_send_cmd(CMD_PHY_RX);
	comms_pkt->comms_adf_rssi_cca = rssi_cca;
	comms_pkt->comms_adf_preamble_patt = get_preamble_pkt();
	comms_pkt->comms_adf_sync_word = get_sync_word();
	comms_pkt->comms_adf_freq = get_freq();
	comms_pkt->comms_adf_read_reg_addr = cmd_adf_read_addr;
	comms_pkt->comms_adf_read_No_double_words = cmd_adf_read_No_double_words;
	comms_pkt->comms_adf_state = adf_get_state();

	for(;i<cmd_adf_read_No_double_words;i++){
		comms_pkt->comms_adf_data[i] =	cmd_adf_data[i];
	}



//	comms_pkt->comms_adf_state = adf_get_state();

	comms_pkt->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p1(tlm_pkt_type, COMMS_API_ID))));
	comms_pkt->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p2((comms_seq_num++)))));
	comms_pkt->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p3(COMMS_PKT_LENGTH))));
	comms_pkt->ccsds_s2 = current_time_lower;
	comms_pkt->ccsds_s1 = current_time_upper;

	if(store_in_sd_card){
		sd_dump_comms = 1;
		comms_pkt->comms_sd_dump = sd_dump_comms;
		comms_pkt->Fletcher_Code = make_FLetcher(data, sizeof(comms_pkt_t) - 2);
		store_data(&comms_partition, data);
		store_in_sd_card = 0;
	}
	else{
		sd_dump_comms = 0;
		comms_pkt->comms_sd_dump = sd_dump_comms;
		comms_pkt->Fletcher_Code = make_FLetcher(data, sizeof(comms_pkt_t) - 2);
//		vGetPktStruct(comms, (void*) comms_pkt, sizeof(comms_pkt_t));
		MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(comms_pkt_t));
	}

//	MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(comms_pkt_t));
//	vGetPktStruct(comms, (void*) data_test, sizeof(data_test));
}

