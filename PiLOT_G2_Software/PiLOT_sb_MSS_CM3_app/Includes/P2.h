/*
 * P2.h
 *
 *  Created on: 26-Mar-2024
 *      Author: S-SPACE
 */

#ifndef P2_H_
#define P2_H_

#include "adf7030.h"
#include "P1.h"

void get_gmc();
uint16_t get_comms();

typedef struct{

	uint16_t ccsds_p1;
	uint16_t ccsds_p2;
	uint16_t ccsds_p3;

	uint32_t ccsds_s1;
	uint32_t ccsds_s2;

//	uint16_t comms_adf_temp;
	uint8_t IMG_ID;
	uint8_t comms_adf_cmd_rx;
	uint8_t comms_adf_cmd_succ;
	uint8_t comms_adf_cmd_reject;
	uint16_t comms_adf_rssi_cca;
	uint16_t comms_adf_rssi;
	uint8_t comms_adf_preamble_patt;
	uint32_t comms_adf_sync_word;
	uint32_t comms_adf_freq;
	uint32_t comms_adf_read_reg_addr;
	uint8_t comms_adf_read_No_double_words;
	uint32_t comms_adf_data[8];
	uint8_t comms_adf_state;
	uint8_t comms_sd_dump;
	uint8_t comms_GTime_SVector[32];
//	uint8_t comms_adf_read_flag;

//	uint8_t comms_cmd_list;

}__attribute__((packed)) comms_pkt_t;

typedef struct{
	uint16_t ccsds_p1;
	uint16_t ccsds_p2;
	uint16_t ccsds_p3;

	uint32_t ccsds_s1;
	uint32_t ccsds_s2;

	uint8_t IMG_ID;
	uint32_t Radiation_Counts;
	uint32_t GMC_Raditation_Timestamp;
	uint32_t Counter_Free_Register;


}__attribute__((packed)) gmc_pkt_t;


#endif /* P2_H_ */
