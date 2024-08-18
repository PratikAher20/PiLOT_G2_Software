/**
 * @file winbondflash.h
 * @author Pratik A.
 * @brief Implementation of Winbond Flash Functionality. (Implementation of the functionality can be found in https://github.com/PratikAher20/SF2_Reprogramming)
 * @version 1.0
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// flashm.h
#ifndef flash_H_

#define flash_H_

#define _CORE_SPI_0 0x40001000

#define FLASH_WREN 0x06
#define FLASH_WRDS 0x04
#define READ_STAT_REG1 0x05
#define READ_STAT_REG3 0x15
#define READ_MGF_ID 0x90
#define RESET_ENABLE 0x06
#define RESET 0x99
#define GLOBAL_UNBLOCK 0x98

#define READ_ARRAY_OPCODE   0x03
#define DONT_CARE       0x00u
#define UNPROTECT_SECTOR_OPCODE     0x39
#define WRITE_DISABLE_CMD   0x4
#define PROGRAM_PAGE_CMD    0x02
#define READ_STATUS         0x05
#define READY_BIT_MASK      0x01
#define ERASE_4K_BLOCK_OPCODE   0x20
#define WRITE_ENABLE_CMD        0x06
#define WRITE_STATUS1_OPCODE    0x01

#define blck_erase	0xD8

#include <stdint.h>

typedef enum{
	WREN = 0,
	WRDS = 1
}ena_dis;

/**
 * @brief Function to write in the Flash memory
 * 
 * @param addr : Address for the Flash Memory
 * @param data : Poniter to the Data to be written
 * @param size : Size of the data to be written
 */
void flash_write(uint32_t addr, uint8_t* data, uint32_t size);

/**
 * @brief Function to read from the Winbond Flash
 * 
 * @param addr : Address of the Flash to be read from
 * @param read_data : POinter to data to store the read data in it
 * @param size : Size of the data to be read from.
 * @return uint8_t 
 */
uint8_t flash_read(uint32_t addr, uint8_t* read_data, uint32_t size);

void flash_fast_read(uint32_t addr, uint8_t* read_Data, uint32_t size);

/**
 * @brief Function to enable the Flash for writing data into it.
 * 
 */
void Flash_write_enable(uint8_t);

/**
 * @brief Function to erase a particular sector from the flash
 * 
 * @param sector 
 */
void flash_sec_erase(uint32_t sector);

/**
 * @brief Function to erase 64K of Block
 * 
 * @param blck 
 */
void flash_64kb_block_erase(uint32_t blck);

/**
 * @brief Function to erase the flash completely
 * 
 */
void flash_erase();
void wait_ready( void );

void chk_busy();
/**
 * @brief Function to read the manufacturing ID from the Flash.
 * 
 */
void Read_mgf_id();

#endif
