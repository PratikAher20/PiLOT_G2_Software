#include "adf7030.h"

ADF_SPI_INSTANCE_t *adf_spi;
extern cmd_t cmd_list[NUM_CMDS];

void set_adf_spi_instance(ADF_SPI_INSTANCE_t *instance) {
    adf_spi = instance;
}
uint8_t radio_memory_configuration[ ] = {
    #include "Settings_adf7030_CUB_rx.txt"
//    //The below 0x00 is included because an "expected expression" error occurs if its deleted. Need to find out why?
//    0x00
};

uint8_t callibrations_config[] = {
    #include "OfflineCalibrations.cfg"
};

//variable to store the length of radio_memory_configuration array
static uint32_t config_length_header;

//Pointer to store the beginning of the SPI commands in radio_memory_configuration
uint8_t *radio_memory_configuration_start_spi_command;

uint8_t adf_init(){
	uint32_t before = 0xFFFFFFFF,current,limit;
	uint8_t buf=0xFF, count=0;
	uint8_t flag = 1,rx_data =0;
	uint8_t enable_intr[4];
	enable_intr[0] = 0x00;
	enable_intr[1] = 0xFF;
	enable_intr[2] = 0x10;
	enable_intr[3] = 0x17;
	//Set limit as a multiple of 100us
	// limit = (data[0] - 48) * 100;
	// limit = limit * MSS_SYS_M3_CLK_FREQ / 1000000;
	//Timer to check if MISO is reacting on time. Typical delay bw #CS Low and MISO high is 92us from datasheet. Here
//	TMR_init(&timer,CORETIMER_0_0,TMR_ONE_SHOT_MODE,PRESCALER_DIV_2,before);

	//Set the adf spi as g_core_spi0

	SPI_init(&g_core_spi0, CORESPI_C0_0, 8);
	SPI_configure_master_mode(&g_core_spi0);

	MSS_GPIO_config(MSS_GPIO_5, MSS_GPIO_OUTPUT_MODE);

	set_adf_spi_instance(&g_core_spi0);

	//Doing ADF_RESET

	MSS_GPIO_set_output(ADF_RST,0);
	uint16_t i = 0;
	for(i=0;i<1000;i++){

	}
	MSS_GPIO_set_output(ADF_RST,1);

	//Start timer
//	TMR_start(&timer);

	//Bring #CS Low by selecting the slave
//	ADF_SPI_SLAVE_SELECT(adf_spi, ADF_SPI_SLAVE);
	i = 0;

	//Now we have to wait until MISO goes high, i.e some non zero data is obtained from the ADF
	do {

//		ADF_SPI_BLOCK_READ(adf_spi, &rx_data ,1, &rx_data, 1);
		//Older version
		//MSS_GPIO_set_output(MSS_GPIO_3, 0);
		//New Version
		ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
		for(i=0;i<350;i++){

		}
		ADF_SPI_BLOCK_READ(adf_spi, &buf ,1, &rx_data, 1);

//		for(i=0;i<350;i++){
//
//		}

		if(rx_data) {
			flag = 0;
			//Older version
//			MSS_GPIO_set_output(MSS_GPIO_3, 1);
			//New version
			ADF_SPI_SLAVE_CLEAR(adf_spi,ADF_SPI_SLAVE);
			for(i=0;i<1000;i++){

			}
			break;
		}
		// current = TMR_current_value(&timer);
		// if((before - current) > limit) {
		// 	flag = 1;
		// 	break;
		count++;
		// }
	}while(count<10);


	ADF_SPI_SLAVE_SELECT(adf_spi, ADF_SPI_SLAVE);
	ADF_SPI_BLOCK_READ(adf_spi, &buf ,1, &rx_data, 1);
	uint8_t check_val = 0,nop = ADF_NOP;
	uint8_t tries = 0;
	//Send NOP command(0xFF) until adf is ready to receive command and also adf is in idle state
	do {
		ADF_SPI_BLOCK_READ(adf_spi,&nop,1,&check_val,1);
		if(((check_val & CMD_READY) != 0) && ((check_val & 0x04) != 0)) {
			break;
		}
	}while(tries++ < 100);
	if(tries >= 100) {
//		return ERR_CMD_FAILED | ERR_IN_IDLE_FAILED;//This oring doesnt matter due to values of error. Need to come up with diffferent values
	}

	//Pull #CS high again
	//Old version
//	ADF_SPI_SLAVE_SELECT(adf_spi, 0);
	//New version
	ADF_SPI_SLAVE_CLEAR(adf_spi,ADF_SPI_SLAVE);
//	TMR_stop(&timer);

	uint8_t ret_val;
	tries = 0;
	//Call adf_config to configure the ADF
	while(tries++ < 100) {
	   ret_val = adf_get_state();
	   if(ret_val == PHY_OFF) {
		   break;
	   }
   }

	if(tries >= 100) {
//		return ERR_FAIL_TO_SET_PHY_OFF;
	}

	count = config_adf7030();

	if(count == 0){
		count = cmd_ready_set();
	} else {
		return ERR_CONFIG_FILE_FAILED;
	}

	if(count == 0){
		count = adf_in_idle();
	} else {
		return ERR_CMD_FAILED;
	}

	if(count == 0){
		adf_write_to_memory(WMODE_1, GENERIC_PKT_FRAME_CFG1, enable_intr, 4);
	} else {
//		return ERR_IN_IDLE_FAILED;
	}

}

uint8_t apply_file(uint8_t *file, uint16_t size) {
//    uint32_t size = sizeof(file);
	//uint32_t size = 728;
    uint32_t array_position = 0;
    uint8_t *pSeqData;
    ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
    do 
    { 
      // Calculate the number of bytes to write
      uint32_t length =  (*(file + array_position ) << 16) | 
                         (*(file + array_position + 1) << 8) |
                         (*(file + array_position + 2));
      
      if(length > 0xFFFF)
      {
         return ERR_LENGTH_OVERFLOW;
      } 
      
      // Write the SPI data pointed to location (MEMORY_FILE + array_position) with specified length (length)
      pSeqData = (file + array_position + 3);
      
      // Transfer the Configuration sequence
      ADF_SPI_BLOCK_WRITE(adf_spi,pSeqData,1,(pSeqData+1),(length-4));

      // Update the array position to point to the next block
      array_position += length;
    
    }while(array_position < size); // Continue operation until full data file has been written
    //ADF_SPI_SLAVE_SELECT(adf_spi,0);
    //using new drivers
    ADF_SPI_SLAVE_CLEAR(adf_spi,ADF_SPI_SLAVE);

    return 0;

}

uint8_t config_adf7030() {
//    uint32_t size = sizeof(radio_memory_configuration);
    //Not using PDMA
#if ADF_USE_PDMA == 0

    // uint32_t array_position = 0, i;
    
    // ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
    // do 
    // { 
    //   // Calculate the number of bytes to write
    //   uint32_t length =  (*(radio_memory_configuration + array_position ) << 16) | 
    //                      (*(radio_memory_configuration + array_position + 1) << 8) |
    //                      (*(radio_memory_configuration + array_position + 2));
      
    //   if(length > 0xFFFF)
    //   {
    //      return ERR_LENGTH_OVERFLOW;
    //   } 
      
    //   // Write the SPI data pointed to location (MEMORY_FILE + array_position) with specified length (length)
    //   uint8_t * pSeqData = (radio_memory_configuration + array_position + 3);
      
    //   // Transfer the Configuration sequence
    //   MSS_GPIO_set_output(MSS_GPIO_3, 0);
    //   ADF_SPI_BLOCK_WRITE(adf_spi,pSeqData,1,(pSeqData+1),(length-4));
    //   MSS_GPIO_set_output(MSS_GPIO_3, 1);
      
    //   for(i=0; i<1000; i++){

    //   }

    //   // Update the array position to point to the next block
    //   array_position += length;
    
    // }while(array_position < size); // Continue operation until full data file has been written

    uint8_t ret_val;
    uint8_t en_calib_array[] = {EN_CALIB >> 24,(EN_CALIB >> 16) & 0xFF, (EN_CALIB >> 8) & 0xFF, EN_CALIB & 0xFF};
    uint8_t en_calib_ar[] = {EN_CALIB & 0xFF, (EN_CALIB >> 8) & 0xFF, (EN_CALIB >> 16) & 0xFF, EN_CALIB >> 24 };
    uint8_t dis_calib_array[] = {DIS_CALIB >> 24,(DIS_CALIB >> 16) & 0xFF, (DIS_CALIB >> 8) & 0xFF, DIS_CALIB & 0xFF};
    uint8_t dis_calib_ar[] = {DIS_CALIB & 0xFF, (DIS_CALIB >> 8) & 0xFF, (DIS_CALIB >> 16) & 0xFF, DIS_CALIB >> 24};
    uint8_t read_reg[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t tries = 0;

    //Apply the configuration file
    ret_val = apply_file(radio_memory_configuration, SIZEOFCONFIG);
    if(ret_val) {
        return ERR_CONFIG_FILE_FAILED;
    }

//    Apply the calibration file
//    ret_val = apply_file(callibrations_config, SIZEOFCALIB);
//    if(ret_val) {
//        return ERR_CALIB_FILE_FAILED;
//    }

    //Enable calibration
//    adf_write_to_memory(WMODE_1,SM_DATA_CALIBRATION,en_calib_ar,4);

//    Issue CMD_CONFIG_DEV command
//    return 0;
    ret_val = adf_send_cmd(CMD_CFG_DEV);

	if(ret_val != 0) {
		return 1;
	}

//    if(ret_val) {
//        adf_write_to_memory(WMODE_1,SM_DATA_CALIBRATION,dis_calib_ar,4);
//        return (ret_val | 0x80);
//    }
////
//    while(1) {
//	   ret_val = adf_get_state();
//	   if(ret_val == PHY_OFF) {
//		   break;
//	   }
//   }

//    adf_in_idle();
    //Issue CMD_PHY_ON
    ret_val = adf_send_cmd(CMD_PHY_ON);
    if(ret_val) {
//        adf_write_to_memory(WMODE_1,SM_DATA_CALIBRATION,dis_calib_ar,4);
        return (ret_val | 0xC0);
    }
    while(tries++ < 100) {
    	ret_val = adf_get_state();
	   if(ret_val == PHY_ON) {
		   break;
	   }
   }

	if(tries >= 100) {
		return 1;
	}
    //Issue CMD_DO_CAL
//    ret_val = adf_get_state();
//    ret_val = adf_send_cmd(CMD_DO_CAL);
//    if(ret_val) {
////        adf_write_to_memory(WMODE_1,SM_DATA_CALIBRATION,dis_calib_ar,4);
//        return (ret_val | 0xE0);
//    }

    //Wait until adf returns to PHY_ON
//    while(1) {
//        ret_val = adf_get_state();
//        if(ret_val == PHY_ON) {
//            break;
//        }
//    }

    //Check for successful calibration
//    adf_read_from_memory(RMODE_1,PROFILE_RADIO_CAL_CFG1,read_reg,4);
//    if((read_reg[2] & 0x20) == 0) {
////        adf_write_to_memory(WMODE_1,SM_DATA_CALIBRATION,dis_calib_ar,4);
//        return ERR_CALIB_FAILED;
//    }

    //Disable calibration
//    adf_write_to_memory(WMODE_1,SM_DATA_CALIBRATION,dis_calib_ar,4);

    return 0;

#else

#endif
}


uint8_t adf_write_to_memory(uint8_t mode,uint32_t addr,uint8_t *data,uint32_t size) {
    //Currently implementing mode1 for testing. Need to implement other modes
    if((mode & WMODE_1) == WMODE_1){
        uint8_t cmd_data[] = {mode,(addr >> 24),((addr >> 16) & 0xFF),((addr >> 8) & 0xFF),(addr & 0xFF )};
        ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
        ADF_SPI_BLOCK_WRITE(adf_spi,cmd_data,5,data,size);
        ADF_SPI_SLAVE_CLEAR(adf_spi, ADF_SPI_SLAVE);
    }

    return 0;
    

}

uint8_t* adf_read_from_memory(uint8_t mode,uint32_t addr,uint8_t *data,uint32_t size) {
    if((mode & RMODE_1) == RMODE_1){
        uint8_t cmd_data[] = {mode,(addr >> 24),((addr >> 16) & 0xFF),((addr >> 8) & 0xFF),(addr & 0xFF )};
        ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
        ADF_SPI_BLOCK_READ(adf_spi,cmd_data,5,data,size+RMODE1_OFFSET);
        ADF_SPI_SLAVE_CLEAR(adf_spi, ADF_SPI_SLAVE);
    }
    return (data+RMODE1_OFFSET);
}

uint8_t adf_send_cmd(uint8_t command) {
    ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
    uint8_t check_val = 0,nop = ADF_NOP;
    uint8_t tries = 0;
    //Send NOP command(0xFF) until adf is ready to receive command
   do {
       ADF_SPI_BLOCK_READ(adf_spi,&nop,1,&check_val,1);
       if((check_val & CMD_READY) != 0) {
           break;
       }
   }while(tries++ < 100);
   if(tries >= 100) {
       return ERR_CMD_FAILED;
   }

    //Send the command
    ADF_SPI_BLOCK_WRITE(adf_spi,&command, 1, &check_val, 1);
    //Using old drivers
    //ADF_SPI_SLAVE_SELECT(adf_spi,0);
    //Using new drivers
    ADF_SPI_SLAVE_CLEAR(adf_spi,ADF_SPI_SLAVE);

    return 0;

}

uint8_t adf_send_cmd_without_ready(uint8_t command) {
    ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
    uint8_t check_val = 0,nop = ADF_NOP;
    uint8_t tries = 0;
    //Send NOP command(0xFF) until adf is ready to receive command
//   do {
//       ADF_SPI_BLOCK_READ(adf_spi,&nop,1,&check_val,1);
//       if((check_val & CMD_READY) != 0) {
//           break;
//       }
//   }while(tries++ < 100);
//   if(tries >= 100) {
//       return ERR_CMD_FAILED;
//   }

    //Send the command
    ADF_SPI_BLOCK_WRITE(adf_spi,&command, 1, &check_val, 1);
    //Using old drivers
    //ADF_SPI_SLAVE_SELECT(adf_spi,0);
    //Using new drivers
    ADF_SPI_SLAVE_CLEAR(adf_spi,ADF_SPI_SLAVE);

    return 0;

}

uint8_t cmd_ready_set() {
    uint8_t check_val = 0,nop = ADF_NOP;
    uint8_t tries = 0;
    //Send NOP command(0xFF) until adf is ready to receive command
    do {
        ADF_SPI_BLOCK_READ(adf_spi,&nop,1,&check_val,1);
        if((check_val & CMD_READY) != 0) {
            break;
        }
    }while(tries++ < 100);
    if(tries >= 100) {
        return 1;
    }
    return 0;
}

uint8_t adf_in_idle() {
    uint8_t check_val = 0,nop = ADF_NOP;
    uint8_t tries = 0;
    //Send NOP command(0xFF) until adf is ready to receive command
    do {
        ADF_SPI_BLOCK_READ(adf_spi,&nop,1,&check_val,1);
        if((check_val & 0x04) != 0) {
            break;
        }
    }while(tries++ < 100);
    if(tries >= 100) {
        return 1;
    }
    return 0;
}

void adf_spi_trans_read( spi_instance_t * this_spi,
    uint8_t * cmd_buffer,
    size_t cmd_byte_size,
    uint8_t * rd_buffer,
    size_t rd_byte_size){

	uint16_t i;
	uint8_t r_buf[6];
	r_buf[0] = 0x00;
	//Using old drivers
//	MSS_GPIO_set_output(MSS_GPIO_3, 0);
//	SPI_block_read(this_spi, cmd_buffer, cmd_byte_size, rd_buffer, rd_byte_size);
//	MSS_GPIO_set_output(MSS_GPIO_3, 1);
	//Using new drivers
//	SPI_set_slave_select(this_spi,ADF_SPI_SLAVE);


		SPI_transfer_block(this_spi,cmd_buffer,cmd_byte_size,r_buf,rd_byte_size);

//	SPI_transfer_block(this_spi,0,0,rd_buffer,rd_byte_size);
//	SPI_clear_slave_select(this_spi,ADF_SPI_SLAVE);
	for(i=0;i<1000;i++){
		if(i<rd_byte_size){
			rd_buffer[i] = r_buf[i];
		}
	}
}

void adf_spi_trans_write( spi_instance_t * this_spi,
    uint8_t * cmd_buffer,
    size_t cmd_byte_size,
    uint8_t * wr_buffer,
    size_t wr_byte_size){

	uint8_t data[300];
	uint16_t i = 0;

	for(;i<cmd_byte_size;i++){
		data[i] = cmd_buffer[i];
	}
	for(;i<cmd_byte_size + wr_byte_size;i++){
		data[i] = wr_buffer[i-cmd_byte_size];
	}
	//Using old drivers
//	MSS_GPIO_set_output(MSS_GPIO_3, 0);
//	SPI_block_write(this_spi, cmd_buffer, cmd_byte_size, wr_buffer, wr_byte_size);
//	MSS_GPIO_set_output(MSS_GPIO_3, 1);
	//Using new drivers
//	SPI_set_slave_select(this_spi,ADF_SPI_SLAVE);
	SPI_transfer_block(this_spi,data,cmd_byte_size + wr_byte_size,0,0);
//	SPI_transfer_block(this_spi,wr_buffer,wr_byte_size,0,0);
//	SPI_clear_slave_select(this_spi,ADF_SPI_SLAVE);
	for(i=0;i<1000;i++){

	}
}

uint8_t adf_get_state() {
    uint8_t misc_fw[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    uint8_t curr_mode = 0;
	uint8_t tries = 0;
    while((!(misc_fw[0] == 0xe4 || misc_fw[0] == 0xA4)) && tries++ < 100){
    	adf_read_from_memory(RMODE_1,MISC_FW,misc_fw,4);
    	if( misc_fw[0] == 0xe2 || misc_fw[0] == 0xA2){
    		break;
    	}
    }
	//PHY SLEEP is zero. Need to discuss what to return when tries goes out of limits
    curr_mode = misc_fw[4] & 0x3F;
    return curr_mode;
}

uint8_t adf_config_gpio(){


	uint8_t data1[4] = {0x1B, 0x1A, 0x19, 0x18};		//Configuring GPIO's as output for now. TODO generalize it to work as INTR also.
//	uint8_t	data2[4] = {0x1C, 0x1D, 0x1E, 0x1F};
	uint8_t rx_buffer[6];
	rx_buffer[0] = 0x00;
	uint8_t tries = 0;

	adf_write_to_memory(WMODE_1, GPIO_CONFIG_ADDR1, data1, sizeof(data1));	// Power On LNA connected to GPIO_0 in older design and GPIO_1 in newer version.
																			// Power On PA connected to GPIO_1 in older design and GPIO_2 in newer version
	while((rx_buffer[0] == 0x00) && tries++ < 100){
		adf_read_from_memory(RMODE_1, GPIO_CONFIG_ADDR1, rx_buffer, 6);
	}
	if(tries >= 100) {
		return 1;
	}

	rx_buffer[0] = 0x00;
	tries = 0;
	while((rx_buffer[0] == 0x00) && tries++ < 100){
		adf_read_from_memory(RMODE_1, TX_CONFIG1_REG, rx_buffer, 6);
	}
	if(tries >= 100) {
		return 1;
	}


	uint8_t data2[4];


	data2[0] = rx_buffer[2] | 0x01;
	data2[1] = rx_buffer[3] | 0x07;
	data2[2] = rx_buffer[4];
	data2[3] = rx_buffer[5];

	adf_write_to_memory(WMODE_1, TX_CONFIG1_REG, data2, sizeof(data2));

	while((rx_buffer[0] == 0x00) && tries++ < 100){
		adf_read_from_memory(RMODE_1, TX_CONFIG1_REG, rx_buffer, 6);
	}
	tries = 0;
	if(tries >= 100) {
		return 1;
	}
	return 0;

}

void read_cmd(uint8_t * cmd_buff){

	cmd_buff[0] = 0x00;

	while(cmd_buff[0] == 0x00){
		adf_read_from_memory(RMODE_1, RX_BUFFER, cmd_buff, 4);
	}
}

void get_rssi_data(uint16_t* rssi){

	uint8_t rx_buf[6];
	rx_buf[0] = 0x00;

	while(rx_buf[0] == 0x00){
		adf_read_from_memory(RMODE_1, RSSI_ADDR, rx_buf, 4);
	}

	*rssi = (uint16_t)((rx_buf[2] & 0x07) << 8) + rx_buf[3];
	*rssi = ~(*rssi) + 1;
	*rssi = *rssi & 0x0FFF;
	if(*rssi > 2048){
		*rssi = *rssi - 2048;
	}
	*rssi = *rssi/4;

}

void get_temp_data(uint8_t *temp){

	uint8_t rx_buf[6];
	uint8_t feed;
	rx_buf[0] = 0;
	feed = adf_send_cmd(CMD_MON);
	while(rx_buf[0] == 0x00){
		adf_read_from_memory(RMODE_1, TEMP_ADDR, rx_buf, 4);
	}

	temp[0] = rx_buf[5];
	temp[1] = rx_buf[4] & 0x0F;

}

void get_rssi_cca_data(uint16_t* rssi){
	uint8_t rx_buf[6];
	//uint16_t rssi;
	rx_buf[0] = 0x00;
	uint8_t state;

	adf_send_cmd(CMD_PHY_CCA);

	state = adf_get_state();

	while(state != 2){
		state = adf_get_state();
	}

	while(rx_buf[0] == 0x00){
		adf_read_from_memory(RMODE_1, PROFILE_CCA_READBACK, rx_buf, 4);
	}

	*rssi = (uint16_t)((rx_buf[4] & 0x07) << 8) + rx_buf[5];
	*rssi = ~(*rssi) + 1;
	*rssi = *rssi & 0x0FFF;
	if(*rssi > 2048){
		*rssi = *rssi - 2048;
	}
	*rssi = *rssi/4;

}

uint8_t send_nop(){

	ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
	uint8_t nop = ADF_NOP;
	uint8_t check_val;
	uint8_t tries = 0;
	uint8_t rem = 1, i=0;
	uint8_t op_data[2] = "0\0";
	uint8_t rdata;
	do {
	   ADF_SPI_BLOCK_READ(adf_spi,&nop,1, &check_val,1);
	   if((check_val & CMD_READY) != 0) {
		   break;
	   }
   }while(tries++ < 100);
   if(tries >= 100) {
	   return ERR_CMD_FAILED;
   }

   ADF_SPI_SLAVE_CLEAR(adf_spi,ADF_SPI_SLAVE);

   rdata = check_val;
   while(rdata){
   		rem = rdata%16;
   		if(rem<10) {
   			op_data[i] = rem + '0';
   		} else {
   			op_data[i] = rem + 'A' - 10;
   		}
   		i--;
   		rdata/=16;
   	}
   	op_data[1] = '\0';

   	echo_str("\n\r\0");
   	echo_str("Data: \0");
	echo_str(op_data);
	echo_str("\n\r\0");

   return 0;
}


uint8_t chk_status(){

	ADF_SPI_SLAVE_SELECT(adf_spi,ADF_SPI_SLAVE);
	uint8_t nop = ADF_NOP;
	uint8_t check_val;
	uint8_t tries = 0;
	uint8_t rem = 1, i=0;
	uint8_t op_data[2] = "0\0";
	uint8_t rdata;
	do {
	   ADF_SPI_BLOCK_READ(adf_spi,&nop,1, &check_val,1);
	   if((check_val & CMD_READY) != 0) {
		   break;
	   }
   }while(tries++ < 100);
   if(tries >= 100) {
	   return ERR_CMD_FAILED;
   }

   ADF_SPI_SLAVE_CLEAR(adf_spi,ADF_SPI_SLAVE);

   rdata = check_val;
//   while(rdata){
//   		rem = rdata%16;
//   		if(rem<10) {
//   			op_data[i] = rem + '0';
//   		} else {
//   			op_data[i] = rem + 'A' - 10;
//   		}
//   		i--;
//   		rdata/=16;
//   	}
//   	op_data[1] = '\0';

//   	echo_str("\n\r\0");
//   	echo_str("Data: \0");
//	echo_str(op_data);
//	echo_str("\n\r\0");

   return 0;
}

uint8_t rx_pkt(uint8_t * cmd, uint16_t* rssi, uint8_t* cmd_rx_flg){

	uint8_t rx_buf[6];
	uint8_t tries = 0;
	uint8_t i = 0;
	rx_buf[5] = 0x00;
	uint8_t clr_tx_buf[4];
	clr_tx_buf[0] = 0xFF;
	clr_tx_buf[1] = 0xFF;
	clr_tx_buf[2] = 0xFF;
	clr_tx_buf[3] = 0xFF;

	uint8_t clr_tx[4];
	clr_tx[0] = 0x00;
	clr_tx[1] = 0x00;
	clr_tx[2] = 0x00;
	clr_tx[3] = 0xDF;

//	while(!(rx_buf[5] == 0x01 || rx_buf[5] == 0x03 || rx_buf[5] == 0xDF)){
//		adf_read_from_memory(RMODE_1, IRQ_CTRL_STATUS0, rx_buf, 4);
//	}
////
//	while(!(rx_buf[5] == 0x03 || rx_buf[5]) == 0xDF){
//		adf_read_from_memory(RMODE_1, IRQ_CTRL_STATUS0, rx_buf, 4);
//	}

	adf_read_from_memory(RMODE_1, IRQ_CTRL_STATUS0, rx_buf, 4);

	do{
		timer_dis();
		adf_read_from_memory(RMODE_1, IRQ_CTRL_STATUS0, rx_buf, 4);
		timer_ena();
		if(rx_buf[5] == 0xDF){
			break;
		}
	}while(tries++ < 100);

//	while(!(rx_buf[5] == 0xDF || rx_buf[5] == 0xD7)){
//		adf_read_from_memory(RMODE_1, IRQ_CTRL_STATUS0, rx_buf, 4);
//	}		//Give number of tries

//	chk_status();
	//If tries<100 read from rx_buffer! or else do not
	if(tries < 100){

		while(rx_buf[5] == 0xDF){
			adf_read_from_memory(RMODE_1, IRQ_CTRL_STATUS0, rx_buf, 4);
			adf_write_to_memory(WMODE_1, IRQ_CTRL_STATUS0, clr_tx, 4);
		}

		*cmd_rx_flg = 1;


		adf_read_from_memory(RMODE_1, RX_BUFFER, rx_buf, 4);

		cmd[0] = rx_buf[5];
		cmd[1] = rx_buf[4];
		cmd[2] = rx_buf[3];
		cmd[3] = rx_buf[2];

//		adf_read_from_memory(RMODE_1, RX_BUFFER + 4, rx_buf, cmd_list[cmd[0] - 1].length - 3);	TODO Rectify This..Gives HardFualt Handler
//		cmd[4] = rx_buf[5];

		if(cmd_list[cmd[0] - 1].length > 4 && cmd_list[cmd[0] - 1].length <= 8){
			adf_read_from_memory(RMODE_1, RX_BUFFER + 4, rx_buf, 4);
			cmd[4] = rx_buf[5];
			cmd[5] = rx_buf[4];
			cmd[6] = rx_buf[3];
			cmd[7] = rx_buf[2];
		}

		else if(cmd_list[cmd[0] - 1].length > 8 && cmd_list[cmd[0] - 1].length <= 16){
			adf_read_from_memory(RMODE_1, RX_BUFFER + 4, rx_buf, 4);
			cmd[4] = rx_buf[5];
			cmd[5] = rx_buf[4];
			cmd[6] = rx_buf[3];
			cmd[7] = rx_buf[2];

			adf_read_from_memory(RMODE_1, RX_BUFFER + 8, rx_buf, 4);
			cmd[8] = rx_buf[5];
			cmd[9] = rx_buf[4];
			cmd[10] = rx_buf[3];
			cmd[11] = rx_buf[2];

			adf_read_from_memory(RMODE_1, RX_BUFFER + 12, rx_buf, 4);
			cmd[12] = rx_buf[5];
			cmd[13] = rx_buf[4];
			cmd[14] = rx_buf[3];
			cmd[15] = rx_buf[2];
		}


		adf_write_to_memory(WMODE_1, RX_BUFFER, clr_tx_buf, 4);
		get_rssi_data(rssi);
	}


	return 0;
}

uint8_t tx_pkt(uint8_t* tx_buffer, uint8_t* size){
//	uint8_t tx_buffer[4];
//	tx_buffer[0] = 0xaa;
//	tx_buffer[1] = 0xbb;
//	tx_buffer[2] = 0xcc;
//	tx_buffer[3] = 0xdd;
	uint8_t rx_buf[6];
	uint8_t set_size[4];
	uint8_t clr_tx[4];
	uint8_t i=0;
	uint8_t tx_buf[*size];

	for(;i<*size;i++){
		tx_buf[i] = tx_buffer[*size - (i+1)];
	}

	clr_tx[0] = 0xff;
	clr_tx[1] = 0xff;
	clr_tx[2] = 0xff;
	clr_tx[3] = 0xff;

	adf_read_from_memory(RMODE_1, GENERIC_PKT_FRAME_CFG1, rx_buf, 4);

	set_size[0] = rx_buf[2];
	set_size[1] = rx_buf[3];
	set_size[2] = rx_buf[4];
	set_size[3] = *size;

	adf_write_to_memory(WMODE_1, GENERIC_PKT_FRAME_CFG1, set_size, 4);

	adf_write_to_memory(WMODE_1, TX_BUFFER, tx_buf, 4);

	chk_status();

	adf_send_cmd(CMD_PHY_TX);

	chk_status();

	if(adf_get_state() == PHY_ON){
		adf_write_to_memory(WMODE_1, TX_BUFFER, clr_tx, 4);
	}


	return 0;
}

uint8_t get_preamble_pkt(){
	uint8_t pre[6];
	uint8_t preamble;

	adf_read_from_memory(RMODE_1, PREAMBLE_READ_REG, pre, 4);
	preamble = pre[3];

	return preamble;
}

uint32_t get_sync_word(){
	uint8_t sw[6];
	uint32_t sync_word;

	adf_read_from_memory(RMODE_1, SYNC_WORD_READ_REG, sw, 4);
	sync_word = (sw[2] << 24) | (sw[3] << 16) | (sw[4] << 8) | (sw[5]);

	return sync_word;
}

uint32_t get_freq(){
	uint8_t fre[6];
	uint32_t frequency;

	adf_read_from_memory(RMODE_1, PROFILE_CH_FREQ, fre, 4);
	frequency = (fre[2] << 24) | (fre[3] << 16) | (fre[4] << 8) | (fre[5]);

	return frequency;
}

