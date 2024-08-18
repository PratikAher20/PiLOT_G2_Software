/**
 * @file IMU.h
 * @author Srinidhi G., Pratik A.
 * @brief Functionality to read the IMU: LSM9Ds1 data values.
 * @version 0.1
 * @date 2024-08-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef OBC_IMU_H_
#define OBC_IMU_H_

/**
 * @brief I2C Address for IMU
 * 
 */
#define IMU_ADDR 0x6a
/**
 * @brief I2C Address for ADC
 * 
 */
#define ADC_ADDR	0x23
#define IMU_WHO_AM_I_REG 0x0F
/**
 * @brief I2C instance for IMU.
 * 
 */
i2c_instance_t g_core_i2c3;
/**
 * @brief Redeclaring the I2C instance for generalized use ahead.
 * 
 */
#define IMU_CORE_I2C &g_core_i2c3
/**
 * @brief Function to get the acceleration values from the IMU
 * 
 * @param a_x : Pointer to Variable to store X-axis acceleration
 * @param a_y : Pointer to Variable to store Y-axis acceleration
 * @param a_z : Pointer to Variable to store Z-axis acceleration
 * @return uint8_t Log Values
 */
uint8_t get_IMU_acc(uint16_t *a_x,uint16_t *a_y,uint16_t *a_z);

/**
 * @brief FUnction to get the angular velocity values from the IMU
 * 
 * @param roll_rate : Pointer to variable to store ROLL-axis w
 * @param pitch_rate : Pointer to variable to store PITCH-axis w
 * @param yaw_rate : Pointer to variable to store YAW-axis w
 * @return uint8_t : Log Values
 */
uint8_t get_IMU_gyro(uint16_t *roll_rate, uint16_t *pitch_rate,uint16_t *yaw_rate);

/**
 * @brief Function to get Temperature values from the IMU.
 * 
 * @param temp : POinter to variable to store the temperature values.
 * @return uint8_t : Log Values
 */
uint8_t get_IMU_temp(uint16_t *temp);

#endif /* OBC_IMU_H_ */
