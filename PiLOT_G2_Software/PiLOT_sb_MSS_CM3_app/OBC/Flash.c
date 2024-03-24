// flashm.c

#include "mss_spi.h"

#include "winbondflash.h"
#include "CoreSPI/core_spi.h"
//#include "SPI.h"  // Include the SPI driver library or header file

uint8_t rx_buf[16];

uint8_t data_read[5];

// Commands for W25Q64FV Flash Memory
//uint8_t CMD0[] = {0x06};        //
//uint8_t CMD1[] = {0x99};        //

uint8_t CMD_RDB[4] = {0x90, 0x00, 0x00, 0x00};
uint8_t cmd = GLOBAL_UNBLOCK;
//Write Enable
//uint8_t CMD1[] = {0x04};        // Write Disable
//uint8_t CMD2[] = {0x01, 0x00};  // Read (1-1-4)
//uint8_t CMD3[] = {0x03, 0x00, 0x00, 0x00};  // Read (1-1-4)
//uint8_t CMD4[] = {0x0B, 0x00, 0x00, 0x00};  // Fast Read (1-1-4)
//uint8_t CMD5[] = {0x0C, 0x00, 0x00, 0x00};  // Fast Read (1-1-1)
//uint8_t CMD6[] = {0x02, 0x00, 0x00, 0x00};  // Page Program (1-1-4)
//uint8_t CMD7[] = {0x20, 0x00, 0x00, 0x00};  // Subsector Erase (4KB) (1-1-4)
//uint8_t CMD8[] = {0x52, 0x00, 0x00, 0x00};  // Sector Erase (32KB) (1-1-4)
//uint8_t CMD9[] = {0xD8, 0x00, 0x00, 0x00};  // Block Erase (64KB) (1-1-4)
//uint8_t CMD10[] = {0x60};       // Chip Erase (1-1-4)
//uint8_t CMD11[] = {0x9F};       // Read JEDEC ID
//uint8_t CMD12[] = {0x01};       // Read Electronic ID
//uint8_t CMD13[] = {0x05};       // Read Unique ID
//uint8_t CMD14[] = {0x81, 0x00}; // Read SFDP Register
//uint8_t CMD15[] = {0x50};       // Clear SR Fail Flags

// ... Add more commands as needed for your specific application.

// Function to send a single command to the flash memory and receive the response

//void delay(){
//	uint8_t i;
//	for(i=0;i<100;i++){
//
//	}
//}


uint8_t read_status_reg(uint8_t sr_cmd){

	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	MSS_SPI_transfer_block(&g_mss_spi0, &sr_cmd, 1, rx_buf, 1); //Checking status register
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

	return 0;

}

void chk_busy(){
	uint8_t cmd[] = {0x05};
	uint32_t i = 0;
	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

//	while(1){
//	MSS_SPI_transfer_block(&g_mss_spi0, cmd, sizeof(cmd), rx_buf, sizeof(rx_buf));
//		SPI_block_read(&g_mss_spi0, cmd, sizeof(cmd), rx_buf, sizeof(rx_buf));
//	}

	MSS_SPI_transfer_block(&g_mss_spi0, cmd, sizeof(cmd), rx_buf, sizeof(rx_buf));
	while(1){
		if(!(rx_buf[0] & 0x01)){
			break;
		}
		MSS_SPI_transfer_block(&g_mss_spi0, cmd, sizeof(cmd), rx_buf, sizeof(rx_buf));
	}

	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

	return;
}


void write_cmd_data
(
    mss_spi_instance_t * this_spi,
    const uint8_t * cmd_buffer,
    uint16_t cmd_byte_size,
    uint8_t * data_buffer,
    uint16_t data_byte_size
){
	uint8_t tx_buffer[516];
	uint16_t transfer_size;
	uint16_t idx = 0;

	transfer_size = cmd_byte_size + data_byte_size;

	for(idx = 0; idx < cmd_byte_size; ++idx)
	{
		tx_buffer[idx] = cmd_buffer[idx];
	}

	for(idx = 0; idx < data_byte_size; ++idx)
	{
		tx_buffer[cmd_byte_size + idx] = data_buffer[idx];
	}

	MSS_SPI_transfer_block( &g_mss_spi0, tx_buffer, transfer_size, 0, 0 );
}



void FLASH_program
(
    uint32_t address,
    uint8_t * write_buffer,
    size_t size_in_bytes
)
{
    uint8_t cmd_buffer[4];

    uint32_t in_buffer_idx;
    uint32_t nb_bytes_to_write;
    uint32_t target_addr;

    MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );

    /* Send Write Enable command */
    cmd_buffer[0] = WRITE_ENABLE_CMD;
    wait_ready();
    MSS_SPI_transfer_block( &g_mss_spi0, cmd_buffer, 1, 0, 0 );

    /* Unprotect sector */
    cmd_buffer[0] = UNPROTECT_SECTOR_OPCODE;
    cmd_buffer[1] = (address >> 16) & 0xFF;
    cmd_buffer[2] = (address >> 8 ) & 0xFF;
    cmd_buffer[3] = address & 0xFF;
    wait_ready();
    MSS_SPI_transfer_block( &g_mss_spi0, cmd_buffer, sizeof(cmd_buffer), 0, 0 );

    /* Send Write Enable command */
    cmd_buffer[0] = WRITE_ENABLE_CMD;
    wait_ready();
    MSS_SPI_transfer_block( &g_mss_spi0, cmd_buffer, 1, 0, 0 );

    /**/
    in_buffer_idx = 0;
    nb_bytes_to_write = size_in_bytes;
    target_addr = address;

    while ( in_buffer_idx < size_in_bytes )
    {
        uint32_t size_left;
        nb_bytes_to_write = 0x100 - (target_addr & 0xFF);   /* adjust max possible size to page boundary. */
        size_left = size_in_bytes - in_buffer_idx;
        if ( size_left < nb_bytes_to_write )
        {
            nb_bytes_to_write = size_left;
        }

        wait_ready();

        /* Send Write Enable command */
        cmd_buffer[0] = WRITE_ENABLE_CMD;
        MSS_SPI_transfer_block( &g_mss_spi0, cmd_buffer, 1, 0, 0 );

        /* Program page */
        wait_ready();

        cmd_buffer[0] = PROGRAM_PAGE_CMD;
        cmd_buffer[1] = (target_addr >> 16) & 0xFF;
        cmd_buffer[2] = (target_addr >> 8 ) & 0xFF;
        cmd_buffer[3] = target_addr & 0xFF;

        write_cmd_data
          (
            &g_mss_spi0,
            cmd_buffer,
            sizeof(cmd_buffer),
            &write_buffer[in_buffer_idx],
            nb_bytes_to_write
          );

        target_addr += nb_bytes_to_write;
        in_buffer_idx += nb_bytes_to_write;
    }

    /* Send Write Disable command. */
    cmd_buffer[0] = WRITE_DISABLE_CMD;

    wait_ready();

    MSS_SPI_transfer_block( &g_mss_spi0, cmd_buffer, 1, 0, 0 );
    MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
}



void flash_erase(){
	uint8_t erase_cmd[] = {0x60};

	Flash_write_enable(WREN);
	Flash_unblock_blocks(cmd);

	Flash_write_enable(WREN);

	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
//	while(1){
//		SPI_block_write(&g_core_spi0, erase_cmd, 1, rx_buff, 1);
	wait_ready();
		MSS_SPI_transfer_block(&g_mss_spi0, erase_cmd, sizeof(erase_cmd), rx_buf, 0);
//	}
//	chk_busy();
		wait_ready();
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	  // After 20sec come out of infinite while loop

}

void flash_sec_erase(uint32_t sector){
	uint8_t sec_erase[] = {0x20, ((sector & 0xffffff) >> 16), ((sector & 0xffff) >> 8), sector & 0xff};


	Flash_write_enable(WREN);
	Flash_unblock_blocks(cmd);

	Flash_write_enable(WREN);

	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	wait_ready();
	MSS_SPI_transfer_block(&g_mss_spi0, sec_erase, sizeof(sec_erase), rx_buf, 0);
	wait_ready();
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
//	chk_busy();



}

void flash_64kb_block_erase(uint32_t blck){
	uint8_t blk_erase[] = {0xD8, ((blck & 0xffffff) >> 16), ((blck & 0xffff) >> 8), blck & 0xff};
	uint8_t i = 0;
	Flash_write_enable(WREN);
	Flash_unblock_blocks(cmd);

	Flash_write_enable(WREN);

	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	wait_ready();
	MSS_SPI_transfer_block(&g_mss_spi0, blk_erase, sizeof(blk_erase), rx_buf, 0);
	wait_ready();
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

//	chk_busy();

}


void Flash_write_enable(uint8_t task){

	if(task == WREN){
		task = FLASH_WREN;
		MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);   //Write enable
		//while(1){
		MSS_SPI_transfer_block(&g_mss_spi0, &task, sizeof(uint8_t), rx_buf, 0);
	//}

		MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	}
	else{
		task = FLASH_WRDS;
		MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);   //Write enable
		//while(1){
		MSS_SPI_transfer_block(&g_mss_spi0, &task, sizeof(uint8_t), rx_buf, 0);
	//}

		MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	}

	chk_busy();
}


void flash_write(uint32_t addr, uint8_t* data, uint32_t size){
	uint8_t wr_cmd = 0x02;
	uint8_t cmd[260] = {wr_cmd, ((addr & 0xffffff) >> 16), ((addr & 0xffff) >> 8), addr & 0xff};
	uint32_t i = 0;
	uint32_t j = 0;
	uint8_t test = 0;
	uint8_t test_count = 0;
	uint8_t num_pages = 16;

	if(size <= 256){

		if(size == 256){
			for(;i<size - 1;i++){
				cmd[4 + i] = data[i];
			}

			cmd[259] = 0;
		}
		else{
			for(;i<size;i++){
				cmd[4 + i] = data[i];
			}
		}
	}


	i = 0;

	uint8_t unlock_sect = 0x39;
	uint8_t unlock_cmd[] = {unlock_sect, ((addr & 0xffffff) >> 16), ((addr & 0xffff) >> 8), addr & 0xff};

	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
//	while(1){
	wait_ready();
	MSS_SPI_transfer_block(&g_mss_spi0, unlock_cmd, sizeof(unlock_cmd), rx_buf, 0);   //unlock the block
	wait_ready();
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

//	chk_busy();


	uint8_t read_block_stat_cmd[] = {0x3D, 0x000000};
	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	MSS_SPI_transfer_block(&g_mss_spi0, read_block_stat_cmd, sizeof(read_block_stat_cmd), rx_buf, 1);   //read_block_stat
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

	uint8_t read_block_stat_cmd1[] = {0x3D, 0x000001};
	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	MSS_SPI_transfer_block(&g_mss_spi0, read_block_stat_cmd1, sizeof(read_block_stat_cmd), rx_buf, 1);   //read_block_stat
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);


	Flash_write_enable(WREN);



	if(size <= 256){
		MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
		wait_ready();
		MSS_SPI_transfer_block(&g_mss_spi0, cmd, 4 + size, rx_buf, 0);  ///write command along with address
		wait_ready();
		MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	}
	else{


		for(;j<num_pages;j++){
			uint8_t unlock_cmd[] = {unlock_sect, ((addr & 0xffffff) >> 16), ((addr & 0xffff) >> 8), addr & 0xff};

			MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
			wait_ready();
			MSS_SPI_transfer_block(&g_mss_spi0, unlock_cmd, sizeof(unlock_cmd), rx_buf, 0);   //unlock the block
			wait_ready();
			MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

			Flash_write_enable(WREN);

			for(;i<256;i++){
				cmd[0] = wr_cmd;
				cmd[1] = (addr & 0xffffff) >> 16;
				cmd[2] = (addr & 0xffff) >> 8;
				cmd[3] = addr & 0xff;

//				if((i & 0xFF) == 0xFF){
//					cmd[4+i] = 0;
//				}
//				else{
					cmd[4+i] = data[i + (j*256)];
//				}
			}

			MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
			wait_ready();
			MSS_SPI_transfer_block(&g_mss_spi0, cmd, 4 + 256, rx_buf, 0);  ///cmd contains addr and the data. Making the pointer generalized.
			wait_ready();
			MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

			Flash_write_enable(WREN);

			wait_ready();
			flash_read(addr, rx_buf, 16);

//			i = 0;

			while(!(rx_buf[0] == cmd[0 + 4])){
				wait_ready();
				Flash_write_enable(WREN);
				MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
				wait_ready();
				MSS_SPI_transfer_block(&g_mss_spi0, cmd, 4 + 256, rx_buf, 0);  ///cmd contains addr and the data. Making the pointer generalized.
				wait_ready();
				MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

				Flash_write_enable(WREN);
				flash_read(addr, rx_buf, 16);
			}
			addr += 0x000100;
			i = 0;
		}

	}



//	chk_busy();

}

uint8_t flash_read(uint32_t addr, uint8_t* read_data, uint32_t size){
	uint8_t read_cmd = 0x03;
	uint8_t cmd[4] = {read_cmd, ((addr & 0xffffff) >> 16), ((addr & 0xffff) >> 8), addr & 0xff};



	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

//	chk_busy();
	wait_ready();

	MSS_SPI_transfer_block(&g_mss_spi0, cmd, sizeof(cmd), read_data, size);

//	chk_busy();
	wait_ready();

	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);


	return 0;

}

void flash_fast_read(uint32_t addr, uint8_t* read_Data, uint32_t size){
	uint8_t read_fast_cmd = 0x0B;
	uint8_t cmd[5] = {read_fast_cmd, ((addr & 0xffffff) >> 16), ((addr & 0xffff) >> 8), addr & 0xff, 0x00};

	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

//	chk_busy();
//	wait_ready();

	MSS_SPI_transfer_block(&g_mss_spi0, cmd, sizeof(cmd), read_Data, size);

//	chk_busy();
//	wait_ready();

	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

}

void Read_mgf_id(){
	uint8_t dummy[] = {0x90, 0x00, 0x00, 0x00};
	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	MSS_SPI_transfer_block(&g_mss_spi0, dummy , 4, rx_buf, 2); //Read mgf_id
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

	uint8_t a;
}

void Flash_unblock_blocks(uint8_t cmd){

	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
	wait_ready();
	MSS_SPI_transfer_block(&g_mss_spi0, &cmd , 1, rx_buf, 0); //Read mgf_id
	wait_ready();
	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
}


void wait_ready( void )
{
    uint8_t ready_bit;
    uint8_t command = READ_STATUS;

    do {
        MSS_SPI_transfer_block( &g_mss_spi0, &command, sizeof(command), &ready_bit, sizeof(ready_bit) );
        ready_bit = ready_bit & READY_BIT_MASK;
    } while( ready_bit == 1 );
}


void flash_init(){
//	uint8_t CMD0[] = {0x06};
//	uint8_t CMD1[] =  {0x99};
	uint8_t id = READ_MGF_ID;
	uint8_t sr_read1 = READ_STAT_REG1;
	uint8_t sr_read3 = READ_STAT_REG3;
//	uint32_t addr = 0x00000001;
//	uint8_t data[] = {0x07, 0x08};
	uint8_t cmd = GLOBAL_UNBLOCK;
//	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
//	MSS_SPI_transfer_block(&g_mss_spi0, CMD0, 1, rx_buf, 1);  //Reset Enable
//	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
//
//	MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
//	MSS_SPI_transfer_block(&g_mss_spi0, CMD1, 1, rx_buf, 1); //Reset
//	MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);

//	while(1){
	uint8_t device_id;
	uint8_t write_sr3[] = {0x11, 0x64};
//	while(1){
	Read_mgf_id();
//	}

//	}
//	while(1){
		Flash_write_enable(WREN);

		read_status_reg(sr_read1);

		read_status_reg(sr_read3);
//	}
//		flash_erase();
//		flash_sec_erase(addr);
//		flash_read(addr, data_read, 1);  //Should read 0xff
//		Flash_write_enable(WREN);
//		Flash_unblock_blocks(cmd);

//		MSS_SPI_set_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
//		MSS_SPI_transfer_block(&g_mss_spi0, write_sr3 , 2, rx_buf, 0); //Write Status reg3
//		MSS_SPI_clear_slave_select(&g_mss_spi0, MSS_SPI_SLAVE_0);
//
//		read_status_reg(sr_read3);

//		Flash_write_enable(WREN);
//		flash_write(addr, data, sizeof(data));
//		flash_read(addr, data_read, sizeof(data_read));

	return;

}


