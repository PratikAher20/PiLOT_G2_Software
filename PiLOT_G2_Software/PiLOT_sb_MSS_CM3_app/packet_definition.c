/*
 * packet_definition.c
 *
 *  Created on: 24-Mar-2024
 *      Author: S-SPACE
 */

//#include "packet_definitions.h"

//void vGetPktStruct(pkt_name_t pktname, void* pktdata, uint8_t pktsize){
//
//	uint8_t i =0;
//	uint8_t* pkt_data;
//	pkt_data = (uint8_t* )pktdata;
//
////	xQueueSend(Data_PKT_Queue, &pkt, 0);
//	if(pktsize + wri_ptr >= 255){
//		send_pkt_flg = 1;
//		active_blck += 1;
//		wri_ptr = 0;
//		if(active_blck == 5){
//			active_blck = 1;
//			send_pkt_flg = 2;
//		}
//	}
//
//	// Storing the packets
//
//	for(;i<pktsize;i++){
//		blck_pkt[active_blck][wri_ptr + i] = pkt_data[i] | (pkt_data[i] << 8);
//	}
//	wri_ptr += pktsize;
//
//}
