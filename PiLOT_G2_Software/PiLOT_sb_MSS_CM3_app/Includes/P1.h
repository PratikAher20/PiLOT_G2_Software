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
#include "packet_definitions.h"
#include "IMU.h"
#include "ccsds.h"
#include "ADC.h"

#define P1_ADC_ADDR		0x40

void p1_init();
void get_hk();

#endif /* P1_H_ */
