/*
 * IMU.c
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
 */

#include <stdint.h>
#include <stdio.h>

#include <core_i2c.h>
#include "IMU.h"

uint8_t get_IMU_acc(uint16_t *a_x,uint16_t *a_y,uint16_t *a_z) {
    uint8_t write_CTRL_REG6_XL[2] = {0x20,0x60};
    uint8_t read_ACC_out_X_L[] = {0x28};
    uint8_t read_ACC_out_Y_L[] = {0x2A};
    uint8_t read_ACC_out_Z_L[] = {0x2C};
    uint8_t read_ACC_out_X_H[] = {0x29};
    uint8_t read_ACC_out_Y_H[] = {0x2B};
    uint8_t read_ACC_out_Z_H[] = {0x2D};
    uint8_t rx_buffer[1],rx_buffer_2[1];
    uint8_t result = 0,status;


    I2C_write_read(IMU_CORE_I2C,IMU_ADDR,write_CTRL_REG6_XL,2,rx_buffer,
                    1,I2C_RELEASE_BUS);
    status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
    result+=status;

    I2C_write_read(IMU_CORE_I2C,IMU_ADDR,read_ACC_out_Z_L,1,rx_buffer,
                            1,I2C_RELEASE_BUS);

    status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
    result+=status;

    I2C_write_read(IMU_CORE_I2C,IMU_ADDR,read_ACC_out_Z_H,1,rx_buffer_2,
                            1,I2C_RELEASE_BUS);

    status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
    result+=status;

    *a_z = ((rx_buffer_2[0] << 8) | rx_buffer[0]);
    if((*a_z) > 32768) {
        *a_z = 65535-*a_z;
    }

    I2C_write_read(IMU_CORE_I2C,IMU_ADDR,read_ACC_out_Y_L,1,rx_buffer,
                            1,I2C_RELEASE_BUS);

    status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
    result+=status;

    I2C_write_read(IMU_CORE_I2C,IMU_ADDR,read_ACC_out_Y_H,1,rx_buffer_2,
                            1,I2C_RELEASE_BUS);

    status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
    result+=status;

    *a_y = ((rx_buffer_2[0] << 8) | rx_buffer[0]);
    if((*a_y) > 32768) {
        *a_y = 65535-*a_y;
    }

    I2C_write_read(IMU_CORE_I2C,IMU_ADDR,read_ACC_out_X_L,1,rx_buffer,
                            1,I2C_RELEASE_BUS);

    status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
    result+=status;

    I2C_write_read(IMU_CORE_I2C,IMU_ADDR,read_ACC_out_X_H,1,rx_buffer_2,
                            1,I2C_RELEASE_BUS);

    status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
    result+=status;

    *a_x = ((rx_buffer_2[0] << 8) | rx_buffer[0]);
    if((*a_x) > 32768) {
        *a_x = 65535-*a_x;
    }

    return status;

}

uint8_t get_IMU_gyro(uint16_t *roll_rate, uint16_t *pitch_rate,uint16_t *yaw_rate) {

    uint8_t write_CTRL_REG1_G[2] = {0x10,0x6A};
        uint8_t read_ACC_out_X_L[] = {0x18};
        uint8_t read_ACC_out_Y_L[] = {0x1A};
        uint8_t read_ACC_out_Z_L[] = {0x1C};
        uint8_t read_ACC_out_X_H[] = {0x19};
        uint8_t read_ACC_out_Y_H[] = {0x1B};
        uint8_t read_ACC_out_Z_H[] = {0x1D};
        uint8_t IMU_slave_addr = 0x6a;
        uint8_t rx_buffer[1],rx_buffer_2[1];
        uint8_t result = 0,status;


        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,write_CTRL_REG1_G,2,rx_buffer,
                        1,I2C_RELEASE_BUS);
        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
        result+=status;

        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,read_ACC_out_Z_L,1,rx_buffer,
                                1,I2C_RELEASE_BUS);

        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
        result+=status;

        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,read_ACC_out_Z_H,1,rx_buffer_2,
                                1,I2C_RELEASE_BUS);

        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
        result+=status;

        *roll_rate = ((rx_buffer_2[0] << 8) | rx_buffer[0]);

        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,read_ACC_out_Y_L,1,rx_buffer,
                                1,I2C_RELEASE_BUS);

        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
        result+=status;

        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,read_ACC_out_Y_H,1,rx_buffer_2,
                                1,I2C_RELEASE_BUS);

        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
        result+=status;

        *pitch_rate = ((rx_buffer_2[0] << 8) | rx_buffer[0]);

        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,read_ACC_out_X_L,1,rx_buffer,
                                1,I2C_RELEASE_BUS);

        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
        result+=status;

        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,read_ACC_out_X_H,1,rx_buffer_2,
                                1,I2C_RELEASE_BUS);

        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);
        result+=status;

        *yaw_rate = ((rx_buffer_2[0] << 8) | rx_buffer[0]);

        return status;
}

uint8_t get_IMU_temp(uint16_t *temp) {

        uint8_t read_temp_L[] = {0x15};
        uint8_t read_temp_H[] = {0x16};
        uint8_t IMU_slave_addr = 0x6a;
        uint8_t rx_buffer[1],rx_buffer_2[1];
        i2c_status_t status;

        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,read_temp_L,1,rx_buffer,
                                1,I2C_RELEASE_BUS);

        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);

        I2C_write_read(IMU_CORE_I2C,IMU_slave_addr,read_temp_H,1,rx_buffer_2,
                                1,I2C_RELEASE_BUS);

        status = I2C_wait_complete(IMU_CORE_I2C,I2C_NO_TIMEOUT);

        *temp = (rx_buffer[0]) | (rx_buffer_2[0] << 8);

        return 0;

}

