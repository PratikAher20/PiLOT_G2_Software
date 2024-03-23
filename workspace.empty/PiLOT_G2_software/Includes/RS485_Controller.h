/*
 * RS485_Controller.h
 *
 *  Created on: 23-Mar-2024
 *      Author: S-SPACE
 */

#ifndef INCLUDES_RS485_CONTROLLER_H_
#define INCLUDES_RS485_CONTROLLER_H_

#include "mss_gpio.h"
#include "hw_reg_access.h"
#include "hal.h"
#include "sys_config_mss_clocks.h"

#define WRITE_SRAM_REG_OFFSET   0x00u
#define WRITE_SRAM_ENABLE_REG_OFFSET 0x04u
#define WRITE_SLAVE_ADDR_REG_OFFSET 0x14u
#define WRITE_CLKS_PER_BIT_REG_OFFSET  0x18u
#define READ_CONST_REG_OFFSET 0x08u
#define READ_RADDR_REG_OFFSET 0x0Cu
#define READ_WADDR_REG_OFFSET 0x10u

#define RS_485_Controller_0 0x50005000u
#define SLAVE_ADDR      0xAu
#define RS485_BAUD_RATE 2000000
#define CLKS_PER_BIT    MSS_SYS_M3_CLK_FREQ / RS485_BAUD_RATE

uint16_t init_RS485_Controller();

#endif /* INCLUDES_RS485_CONTROLLER_H_ */
