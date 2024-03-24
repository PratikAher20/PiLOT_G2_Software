/*
 * P1.c
 *
 *  Created on: 24-Mar-2024
 *      Author: S-SPACE
 */

#include "P1.h"

hk_pkt_t* hk_pkt;
uint8_t data[512];
uint16_t hk_seq_num =0;

void p1_init(){
	I2C_init(VC_SENSOR_I2C, COREI2C_0_0, VC1, I2C_PCLK_DIV_256);	//VC_Sensor
	I2C_init(IMU_CORE_I2C, COREI2C_3_0, IMU_ADDR, I2C_PCLK_DIV_256);	//IMU_Sensor
}

void get_hk(){
	hk_pkt = (hk_pkt_t* )data;
	uint16_t ax, ay, az;
	uint16_t roll_rate, pitch_rate, yaw_rate;
	uint16_t CDH_VC[2];
	uint16_t PIS_VC[2];
	uint16_t imu_temp;
	uint8_t result=0, flag;
	uint8_t msg[18] = "\n\rGot HK Readings\0";
	result = get_IMU_acc(&ax, &ay, &az);
	result = get_IMU_gyro(&roll_rate, &pitch_rate, &yaw_rate);
	result = get_IMU_temp(&imu_temp);
//	CDH_VC[0] = read_bus_voltage( VC1,  2, &flag);
//	PIS_VC[0] = read_bus_voltage(VC1, 3, &flag);
//	CDH_VC[1] = read_shunt_voltage(VC1, 2, &flag);
//	PIS_VC[1] = read_shunt_voltage(VC1, 3, &flag);

	hk_pkt->Acc[0] = ((ax));
	hk_pkt->Acc[1] = ((ay));
	hk_pkt->Acc[2] = ((az));
	hk_pkt->Angular_Rate[0] = roll_rate;
	hk_pkt->Angular_Rate[1] = pitch_rate;
	hk_pkt->Angular_Rate[2] = yaw_rate;
	hk_pkt->imu_temp = imu_temp;

//	hk_pkt->Sensor_Board_VC[0] = read_bus_voltage(VC1, 1, &flag);
	hk_pkt->CDH_VC[0] = read_bus_voltage( VC1,  2, &flag);
	hk_pkt->PIS_VC[0] = read_bus_voltage( VC1,  3, &flag);
//	hk_pkt->Sensor_Board_VC[1] = read_shunt_voltage(VC1, 1, &flag);
	hk_pkt->CDH_VC[1] = read_shunt_voltage( VC1,  2, &flag);
	hk_pkt->PIS_VC[1] = read_shunt_voltage( VC1,  3, &flag);

	hk_pkt->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p1(tlm_pkt_type, HK_API_ID))));
	hk_pkt->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p2((hk_seq_num++)))));
	hk_pkt->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(((ccsds_p3(HK_PKT_LENGTH))));
	hk_pkt->ccsds_s1 = 0;
	hk_pkt->ccsds_s2 = 0;

	vGetPktStruct(hk, (void*) hk_pkt, sizeof(hk_pkt_t));
//	MSS_UART_polled_tx(&g_mss_uart0, data, sizeof(hk_pkt_t));
//	MSS_UART_polled_tx_string(&g_mss_uart0, msg);
}
