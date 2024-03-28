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

uint16_t data_test[25] = {0};
comms_pkt_t* comms_pkt;
uint16_t comms_seq_num = 0;

void get_gmc(){

}

void get_comms(){
	uint16_t i = 1;
	comms_pkt = (comms_pkt_t*) data;
//	data_test[0]++;
//	for(;i<25;i++){
//		data_test[i] = i;
//		if(i == 24){
//			break;
//		}
//	}

	get_rssi_data(&rssi);
	comms_pkt->comms_adf_rssi = rssi;
	comms_pkt->comms_adf_cmd_rx = cmd_rx_count;
	comms_pkt->comms_adf_cmd_succ = cmd_succ_count;
	comms_pkt->comms_adf_cmd_reject = cmd_reject_count;
	comms_pkt->comms_adf_rssi_cca = rssi_cca;
	comms_pkt->comms_adf_preamble_patt = get_preamble_pkt();
	comms_pkt->comms_adf_sync_word = get_sync_word();
	comms_pkt->comms_adf_freq = get_freq();
//	comms_pkt->comms_adf_state = adf_get_state();

	comms_seq_num++;
	comms_pkt->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p1(tlm_pkt_type, COMMS_API_ID))));
	comms_pkt->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p2((comms_seq_num++)))));
	comms_pkt->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p3(COMMS_PKT_LENGTH))));
	comms_pkt->ccsds_s1 = 0;

	comms_pkt->ccsds_s2 = 0;

	MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(comms_pkt_t));
//	vGetPktStruct(comms, (void*) data_test, sizeof(data_test));
}
