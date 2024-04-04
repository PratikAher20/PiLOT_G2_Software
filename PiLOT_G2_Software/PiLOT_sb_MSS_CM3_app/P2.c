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
extern uint8_t store_in_sd_card;
extern uint8_t IMG_ID;

uint8_t sd_dump_comms = 0;
uint16_t data_test[25] = {0};
comms_pkt_t* comms_pkt;
uint16_t comms_seq_num = 0;
uint32_t cmd_adf_read_addr = 0;
uint8_t cmd_adf_read_No_double_words = 0;
uint32_t cmd_adf_data[8];

void get_gmc(){

}

void get_comms(){
	uint16_t i = 0;
	comms_pkt = (comms_pkt_t*) data;
//	data_test[0]++;
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
	comms_pkt->ccsds_s1 = 0;

	comms_pkt->ccsds_s2 = 0;

	if(store_in_sd_card){
		sd_dump_comms = 1;
		comms_pkt->comms_sd_dump = sd_dump_comms;
		store_data(&comms_partition, data);
		store_in_sd_card = 0;
	}
	else{
		sd_dump_comms = 0;
		comms_pkt->comms_sd_dump = sd_dump_comms;
//		vGetPktStruct(comms, (void*) comms_pkt, sizeof(comms_pkt_t));
		MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(comms_pkt_t));
	}

//	MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(comms_pkt_t));
//	vGetPktStruct(comms, (void*) data_test, sizeof(data_test));
}
