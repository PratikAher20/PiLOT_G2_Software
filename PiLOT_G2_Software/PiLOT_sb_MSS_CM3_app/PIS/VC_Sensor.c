/*
 * VC_Sensor.c
 *
 *  Created on: 11-Mar-2024
 *      Author: S-SPACE
 */

#include "VC_Sensor.h"

uint8_t count;
uint16_t init_command;
uint8_t config_reg;
i2c_status_t status;
uint8_t read[2];
uint16_t bvol;

uint8_t vc_write(i2c_instance_t* I2C, uint8_t addr, uint8_t *tx, uint8_t tx_size) {
     count = 0;
    while(count < 10) {
        I2C_write(I2C,addr,tx,tx_size,I2C_RELEASE_BUS);
        status = I2C_wait_complete(VC_SENSOR_I2C,I2C_NO_TIMEOUT);
        if(status == I2C_SUCCESS) {
            return 0;
        }
        count++;
    }
    return count;

}

uint8_t vc_read(i2c_instance_t* I2C, uint8_t addr, uint8_t *rx, uint8_t rx_size) {
    count = 0;
    while(count < 10) {
        I2C_read(I2C,addr,rx,rx_size,I2C_RELEASE_BUS);
        status = I2C_wait_complete(VC_SENSOR_I2C,I2C_NO_TIMEOUT);
        if(status == I2C_SUCCESS) {
            return 0;
        }
        count++;
    }
    return count;

}

uint8_t vc_read_reg(i2c_instance_t* I2C, uint8_t vc_addr, uint8_t reg_addr,uint8_t *rx) {
    if((vc_write(I2C, vc_addr,&reg_addr,sizeof(reg_addr))) >= 10) {
        return 1;
    }
    if((vc_read(I2C, vc_addr,(uint8_t*)rx,2)) >= 10) {
        return 1;
    }

    return 0;
}

uint8_t vc_write_reg(i2c_instance_t* I2C, uint8_t vc_addr, uint8_t reg_addr,uint8_t *tx,uint8_t tx_size) {
    if((vc_write(I2C, vc_addr,&reg_addr,sizeof(reg_addr))) >= 10) {
        return 1;
    }
    if((vc_write(I2C, vc_addr,(uint8_t*)tx,tx_size)) >= 10) {
        return 1;
    }

    return 0;
}

uint8_t vc_init(i2c_instance_t* I2C, uint8_t addr) {

    init_command = VC_INIT_CMD;
    config_reg = VC_CONFIG_REG;
    if(vc_write(I2C, addr,&config_reg,sizeof(config_reg)) >= 10) {
        return 1;
    }
    if(vc_write(I2C, addr,(uint8_t*)(&init_command),sizeof(init_command)) >= 10) {
        return 1;
    }

    return 0;
}

uint16_t read_bus_voltage(i2c_instance_t* I2C, uint8_t addr, uint8_t chx,uint8_t *flag) {
    read[0] = 0;
    read[1] = 0;
    bvol = 0;
    if(vc_read_reg(I2C, addr,VC_BUSV_CHx(chx),read) == 0) {
        bvol = read[0]<<8 | read[1];
        *flag = 0;

    } else {
        *flag = 1;
        bvol = 0;
    }
    return bvol;
}

uint16_t read_shunt_voltage(i2c_instance_t* I2C, uint8_t addr, uint8_t chx,uint8_t *flag) {
    read[0] = 0;
    read[1] = 0;
    bvol = 0;
    if(vc_read_reg(I2C, addr,VC_SHUNTV_CHx(chx),read) == 0) {
        bvol = read[0]<<8 | read[1];
        *flag = 0;
    } else {
        *flag = 1;
        bvol = 0;
    }
    return bvol;
}
