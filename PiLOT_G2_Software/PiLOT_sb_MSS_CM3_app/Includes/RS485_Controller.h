/**
 * @file RS485_Controller.h
 * @author Pratik A.
 * @brief : Functionality for interfacing with DPU and TTPU (PSLV Packages)
 * @version : 1.0
 * @date 2024-08-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef INCLUDES_RS485_CONTROLLER_H_
#define INCLUDES_RS485_CONTROLLER_H_

#include "mss_gpio.h"
#include "hw_reg_access.h"
#include "hal.h"
#include "sys_config_mss_clocks.h"
#include "cmd.h"
//#include "P1.h"

/**
 * @brief Register Address to write into TPSRAM
 * 
 */
#define WRITE_SRAM_REG_OFFSET   0x00u
/**
 * @brief Register Address to enable the Control Interface. (For this software the Enabling is done using GPIO 0.)
 * 
 */
#define WRITE_SRAM_ENABLE_REG_OFFSET 0x04u
/**
 * @brief Register Address to set the slave address for data transmission
 * 
 */
#define WRITE_SLAVE_ADDR_REG_OFFSET 0x14u
/**
 * @brief Register Address to set the CLKS_PER_BIT
 * 
 */
#define WRITE_CLKS_PER_BIT_REG_OFFSET  0x18u
/**
 * @brief Register address to set the NUmber of param bytes to receive in cmd pkt. (Not use in this SW)
 * 
 */
#define WRITE_NUM_BYTES_REG_OFFSET	0x20u
/**
 * @brief Register Address to set the payload ID to receive the commands from the TTPU
 * 
 */
#define WRITE_PAY_ID_REG_OFFSET		0x10u
/**
 * @brief Register Address to read the constant value to verify the FPGA Operation.
 * 
 */
#define READ_CONST_REG_OFFSET 0x08u
/**
 * @brief Register Address to read the READ_ADDRESS of the TPSRAM
 * 
 */
#define READ_RADDR_REG_OFFSET 0x0Cu
/**
 * @brief Register Address to reas the WRITE_ADDRESS of the TPSRAM
 * 
 */
#define READ_WADDR_REG_OFFSET 0x10u
/**
 * @brief Register Address to read the command parameters from the TPSRAM.
 * 
 */
#define READ_SRAM_CMD_REG_OFFSET	0x0Cu

/**
 * @brief APB address to read from Data Transfer APB block.
 * 
 */
#define RS_485_Controller_0 0x50005000u
/**
 * @brief APB address to read from Commanding APB block.
 * 
 */
#define APB_READ_CMD_0		0x5000B000u
/**
 * @brief APB address to read from GPS_TIME and STATE_Vector APB block.
 * 
 */
#define APB_READ_TLM_0		0x5000C000u
/**
 * @brief Slave address used for data transfer.
 * 
 */
#define SLAVE_ADDR      0x26u
/**
 * @brief Baud Rate for RS485 line.
 * 
 */
#define RS485_BAUD_RATE 2000000
/**
 * @brief Number of command parameters to rx from the rxed cmd.
 * 
 */
#define NUM_BYTES	32
/**
 * @brief Payload ID to receive the command from TTPY.
 * 
 */
#define PAY_ID	0x07
/**
 * @brief Clock per bits value to provide to the FPGA.
 * 
 */
#define CLKS_PER_BIT    MSS_SYS_M3_CLK_FREQ / RS485_BAUD_RATE
/**
 * @brief Number of TTPU IRQ's should be given to start the Repro command.
 * 
 */
#define NUMBER_OF_REPRO_CMDS	5

/**
 * @brief Function to initiliaze the Control Interface FPGA block.
 * 
 * @return uint8_t : Log Values.
 */
uint8_t init_RS485_Controller();
/**
 * @brief Function to read the GPS_TIME and State_Vector 
 * 
 */
void get_time_vecto();

#endif /* INCLUDES_RS485_CONTROLLER_H_ */
