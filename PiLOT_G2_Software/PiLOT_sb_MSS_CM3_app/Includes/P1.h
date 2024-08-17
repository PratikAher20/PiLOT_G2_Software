/*
 * P1.h
 *
 *  Created on: 24-Mar-2024
 *      Author: S-SPACE
 */

#ifndef P1_H_
#define P1_H_

#include "VC_Sensor.h"
#include "RS485_Controller.h"
#include "SD.h"
#include "peripherals.h"
//#include "packet_definitions.h"
#include "IMU.h"
#include "ccsds.h"
#include "ADC.h"
#include "CoreTimer/core_timer.h"
#include "sys_config_mss_clocks.h"
#include "adf7030.h"
#include "PiLOT_G2_hw_platform.h"
#include "memory.h"
#include "P2.h"
#include "drivers/mss_watchdog/mss_watchdog.h"
#include "drivers/mss_nvm/mss_nvm.h"
#include "counter.h"

//#define CoreTimer_C0_0	0x50006000
//#define CoreTimer_C1_0	0x50007000
//#define CoreTimer_C2_0	0x50008000
//#define CoreTimer_C3_0	0x50009000
//#define CoreTimer_C4_0	0x5000A000


#define HK_PKT_PERIOD 		MSS_SYS_M3_CLK_FREQ/1024 * 0.5
#define COMMS_PKT_PERIOD	MSS_SYS_M3_CLK_FREQ/1024 * 0.8
#define TEMP_PKT_PERIOD		MSS_SYS_M3_CLK_FREQ/1024 * 1
#define SD_PKT_PERIOD		MSS_SYS_M3_CLK_FREQ/1024 * 10
#define GMC_PKT_PERIOD		MSS_SYS_M3_CLK_FREQ/1024 * 0.5

//ENVM Storage address for critical data
#define ENVM_RESET_COUNT_ADDR		0x60032000


#define P1_ADC_ADDR		0x40

void p1_init();
uint16_t get_hk();
uint16_t get_temp();
//void get_comms();


#define PILOT_REVERSE_BYTE_ORDER(var)	(((var) << 8) | ((var) >> 8))

#define HK_API_ID			1
#define HK_PKT_LENGTH		sizeof(hk_pkt_t)

#define GMC_API_ID			2
#define GMC_PKT_LENGTH		sizeof(gmc_pkt_t)

#define COMMS_API_ID			3
#define COMMS_PKT_LENGTH		sizeof(comms_pkt_t)

#define THERMISTOR_API_ID			4
#define THERMISTOR_PKT_LENGTH		sizeof(thermistor_pkt_t)

#define INIT_API_ID    	5
#define INIT_PKT_LENGTH  sizeof(init_packet_t)

#define LOGS_API_ID		6
#define LOGS_PKT_LENGTH	sizeof(log_packet_t)

#define TIME_API_ID			60
#define TIME_PKT_LENGTH		sizeof(timer_pkt)

//Task IDs
#define TIMER_TASK_ID		0
#define THERMISTOR_TASK_ID	1
#define HK_TASK_ID			2
#define SD_HK_TASK_ID		3
#define COMMS_TASK_ID		4
#define LOGS_TASK_ID		5
#define GMC_TASK_ID         6

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint32_t ccsds_s2;

    uint8_t Cmd_ADF_counts;
    uint8_t Cmd_RS485_Succ_counts;
    uint8_t Cmd_RS485_Fail_counts;
    uint8_t IMG_ID;
    uint16_t CLK_RATE;		// In KHz
    uint32_t Command_Loss_Timer;
    uint8_t PREV_CMD_RX;
    uint8_t latest_codeword_rx;
    uint8_t Reset_Counts;
    uint8_t RTM[16];
    uint16_t Acc[3];  // X,Y,Z Axis
    uint16_t Angular_Rate[3]; //Pitch, Roll, Yaw
    uint16_t imu_temp;
//    uint16_t CDH_VC[2];
//    uint16_t PIS_VC[2];
    uint16_t Voltages[5];
    uint16_t Currents[5];

    uint32_t HK_Read_Pointer;
    uint32_t HK_Write_Pointer;
    uint32_t Thermistor_Read_Pointer;
    uint32_t Thermistor_Write_Pointer;
//    uint32_t Logs_Read_Pointer;
//    uint32_t Logs_Write_Pointer;
//    uint32_t SD_Test_Read_Pointer;
//    uint32_t SD_Test_Write_Pointer;
    uint32_t COMMS_Read_Pointer;
    uint32_t COMMS_Write_Pointer;
//
//    uint16_t aris_miss;
//    uint16_t hk_miss;
//    uint16_t payload_miss;
    uint8_t sd_dump;
    uint8_t HKGTime_SVector[32];
//
    uint16_t Fletcher_Code;
}__attribute__((packed)) hk_pkt_t;

typedef struct{

	uint16_t ccsds_p1;
	uint16_t ccsds_p2;
	uint16_t ccsds_p3;

	uint32_t ccsds_s1;
	uint32_t ccsds_s2;
	uint16_t Temperature_Values[8];
	uint8_t sd_dump;
	uint8_t TempGTime_SVector[32];
	uint16_t Fletcher_Code;

}__attribute__((packed))thermistor_pkt_t;
/**
 * @brief Each log entry has the below four fields for every task that occurs
 *
 */
typedef struct {
	uint32_t time_L;//lower time value
	uint32_t time_H;//upper time value
	uint8_t task_id;//task id
	uint16_t task_status;//task status;
}__attribute__((packed)) log_entry_t;

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint32_t ccsds_s2;

    log_entry_t logs[10];

    uint16_t Fletcher_Code;
}__attribute__((packed)) log_packet_t;


typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint32_t ccsds_s2;

    uint8_t Image_ID;
    uint8_t Status_1;
    uint8_t Adf_init_status;
    uint8_t status_2;

    uint8_t GTime_SVector[32];
    uint16_t Fletcher_Code;
}__attribute__((packed)) init_packet_t;

typedef enum pkt_name{
	hk = 0,
	comms = 1,
	thermistor = 2,
	gmc = 3,
    logs = 4,
    sd = 5,
    init = 6
}pkt_name_t;

typedef struct pkt{

	pkt_name_t pkt_type;

	void * pkt_data;

	uint8_t pkt_size;

} __attribute__((packed)) pkt_t;


void vGetPktStruct(pkt_name_t pktname, void* pktdata, uint8_t pktsize);
void store_pkt();
void get_sd_data();
uint16_t make_FLetcher(uint8_t *data,uint16_t len);

#endif /* P1_H_ */
