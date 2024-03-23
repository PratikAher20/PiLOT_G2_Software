/*
 * IMU.h
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
 */

#ifndef OBC_IMU_H_
#define OBC_IMU_H_

#define IMU_ADDR 0x6a
#define IMU_WHO_AM_I_REG 0x0F

i2c_instance_t g_core_i2c5;

uint8_t get_IMU_acc(uint16_t *a_x,uint16_t *a_y,uint16_t *a_z);

uint8_t get_IMU_gyro(uint16_t *roll_rate, uint16_t *pitch_rate,uint16_t *yaw_rate);

uint8_t get_IMU_temp(uint16_t *temp);

#endif /* OBC_IMU_H_ */
