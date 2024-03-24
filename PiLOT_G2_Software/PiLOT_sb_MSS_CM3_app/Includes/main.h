/*
 * main.h
 *
 *  Created on: 23-Mar-2024
 *      Author: S-SPACE
 */

#ifndef INCLUDES_MAIN_H_
#define INCLUDES_MAIN_H_

#include "CoreTimer/core_timer.h"
#include "sys_config_mss_clocks.h"
#include "P1.h"

#define CoreTimer_C0_0	0x50006000
#define CoreTimer_C1_0	0x50007000
#define CoreTimer_C2_0	0x50008000
#define CoreTimer_C3_0	0x50009000
#define CoreTimer_C4_0	0x5000A000


#define HK_PKT_PERIOD 	MSS_SYS_M3_CLK_FREQ/1024 * 1

#define HK_PACKET_SIZE  132
//uint16_t blck_pkt_2[256];
//uint16_t blck_pkt_3[256];
//uint16_t blck_pkt_4[256];

#endif /* INCLUDES_MAIN_H_ */
