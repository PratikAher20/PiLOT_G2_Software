/**
 * @file cmd.c
 * @author Pratik A.
 * @brief Implementation of functions defined in cmd.h
 * @version 1.0
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "cmd.h"

cmd_t* cmd;

// Function checking the CheckSUM
//void chk_sum();

cmd_t cmd_list[NUM_CMDS];

extern uint16_t command_cnt;
extern uint16_t command_reject_cnt;
extern timer_instance_t hk_timer;
extern timer_instance_t comms_timer;
extern timer_instance_t gmc_timer;
extern timer_instance_t temp_timer;
extern timer_instance_t sd_timer;
extern uint8_t ERR_LOG;
extern uint8_t latest_codeword;
extern uint32_t cmd_adf_read_addr;
extern uint8_t cmd_adf_read_No_double_words;
extern uint32_t cmd_adf_data[8];
extern uint8_t RTM[16];
extern uint8_t IMG_ID;

uint32_t REPRO_CODE_WORD_ADDR = 0x60033000;
uint8_t* code_word = (uint8_t*) 0x60033000;

uint8_t cmd_valid(rx_cmd_t* rx_cmd, uint8_t src){
	//Logic to check validity of the command ID within limits along with the params limits.
	//Check the Checksum of the cmds from TTPU and then proceed to check the cmd id.
	if(rx_cmd->cmd_id - 1 >= 0x00 && rx_cmd->cmd_id - 1 <= NUM_CMDS){
		return 1;  //Will be checking the validated of the checksum.
	}
	else{
		return 0;
	}

}


void add_cmd(uint8_t id, uint16_t length, void (*ex_func)(uint8_t id, rx_cmd_t* rcv_cmd)){

	cmd_list[id].id = id;
	cmd_list[id].length = length;
	cmd_list[id].ex_func = ex_func;

}

void cmd_engine(rx_cmd_t* rx_cmd){

	   //the actual command apid's is one greater than this.
//	add_cmd(2, 2, cmd_sc_reset);

	cmd_list[rx_cmd->cmd_id - 1].ex_func(rx_cmd);

}

// Commands Definition

void cmd_noop(rx_cmd_t* rcv_cmd){

	uint8_t a;

}

void set_pkt_rate(rx_cmd_t* rcv_cmd){

	uint32_t new_time_period = MSS_SYS_M3_CLK_FREQ/10240 * (rcv_cmd->parameters[1]);

	if(rcv_cmd->parameters[1] != 0){
		if(rcv_cmd->parameters[0] == hk){
			TMR_init(&hk_timer, CORETIMER_C0_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, new_time_period);
			TMR_enable_int(&hk_timer);
			TMR_start(&hk_timer);
		}
		else if(rcv_cmd->parameters[0] == comms){
			TMR_init(&comms_timer, CORETIMER_C1_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, new_time_period);
			TMR_enable_int(&comms_timer);
			TMR_start(&comms_timer);
		}
		else if(rcv_cmd->parameters[0] == thermistor){
			TMR_init(&temp_timer, CORETIMER_C2_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, new_time_period);
			TMR_enable_int(&temp_timer);
			TMR_start(&temp_timer);
		}
		else if(rcv_cmd->parameters[0] == sd){
			TMR_init(&sd_timer, CORETIMER_C3_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, new_time_period);
			TMR_enable_int(&sd_timer);
			TMR_start(&sd_timer);
		}
		else if(rcv_cmd->parameters[0] == gmc){
			TMR_init(&gmc_timer, CORETIMER_C4_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_1024, new_time_period);
			TMR_enable_int(&gmc_timer);
			TMR_start(&gmc_timer);
		}
	}
	else{
		//Here, instead of disabling the NVIC interrupts, we can stop the timer, so that the packetisation can again be restarted with another command
		if(rcv_cmd->parameters[0] == hk){
			TMR_stop(&hk_timer);
//			NVIC_DisableIRQ(FabricIrq4_IRQn);
		}
		else if(rcv_cmd->parameters[0] == comms){
			TMR_stop(&comms_timer);
//			NVIC_DisableIRQ(FabricIrq5_IRQn);
		}
		else if(rcv_cmd->parameters[0] == thermistor){
			TMR_stop(&temp_timer);
//			NVIC_DisableIRQ(FabricIrq6_IRQn);
		}
		else if(rcv_cmd->parameters[0] == sd){
			TMR_stop(&sd_timer);
//			NVIC_DisableIRQ(FabricIrq7_IRQn);
		}
		else if(rcv_cmd->parameters[0] == gmc){
			TMR_stop(&gmc_timer);
//			NVIC_DisableIRQ(FabricIrq8_IRQn);
		}
	}

}

void delay ( volatile unsigned int n)
{
	while(n!=0)
	{
		n--;
	}
}


void copy_esram_image()
{
    unsigned int ii=0;
    unsigned long *exeDestAddr, *exeSrcAddr;

    exeDestAddr = (unsigned long *)0x20008000;
    exeSrcAddr = (unsigned long *)0x6000D000;
    /* 60 K B = 61440/4 ptr increments by 4bytes*/
    for (ii=0; ii<2764; ii++ )
    {
    	*exeDestAddr++ = *exeSrcAddr++;
    }

}

void __attribute__((optimize("O0"))) remap_user_code_eSRAM_0(void)
{
//	void __attribute__((optimize("O0"))) remap_user_code_eSRAM_0(void)
//	{
//	 int * address = (int *)0x20000004; //pointer to reset handler of application
//	 __set_MSP(*(int*)0x20000000);   //set the stack pointer to that of the application
//	 SYSREG->ESRAM_CR |= 0x1;
//	 ((void (*)())(*address))();    // pointer recast as function pointer and the dereferenced/called
//	 while(1){ };            //This instruction never executed
//	}



	int * address = (int *)0x20008004;
	__set_MSP(*(int*)0x20008000);
	SYSREG->ESRAM_CR |= 0x3;
	((void (*)())(*address))();
	while(1){ };
}



void exe_iap(rx_cmd_t* rcv_cmd){


	uint8_t prog_status, auth_status;

//	if(rcv_cmd->parameters[0] == 0x07 || rcv_cmd->parameters[0] == 0x14 || rcv_cmd->parameters[0] == 0x21){
//		NVM_write(REPRO_CODE_WORD_ADDR, &(rcv_cmd->parameters[0]), 1, NVM_DO_NOT_LOCK_PAGE);
//		latest_codeword = rcv_cmd->parameters[0];
//		REPRO_CODE_WORD_ADDR += 0x01;
//	}



//
//	if(code_word[0] == 0x07 && code_word[1] == 0x14 && code_word[2] == 0x21){
//		if(IMG_ID == 0){
//			MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
//
//			g_mss_spi0.hw_reg->CONTROL |= (0x04000000);
//			delay(80000);
//
//			auth_status = MSS_SYS_initiate_iap(MSS_SYS_PROG_AUTHENTICATE, 0x400000);
//
//			delay(80000);
//
//			if(auth_status){
//				ERR_LOG = ERR_LOG | 0x01;
//			}
//			else{
//				prog_status = MSS_SYS_initiate_iap(MSS_SYS_PROG_PROGRAM, 0x400000);
//			}
//		}
//		else{
//			MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
//
//			g_mss_spi0.hw_reg->CONTROL |= (0x04000000);
//			delay(80000);
//
//			auth_status = MSS_SYS_initiate_iap(MSS_SYS_PROG_AUTHENTICATE, 0x001000);
//
//			delay(80000);
//
//			if(auth_status){
//				ERR_LOG = ERR_LOG | 0x01;
//			}
//			else{
//				prog_status = MSS_SYS_initiate_iap(MSS_SYS_PROG_PROGRAM, 0x001000);
//			}
//		}

	SYSREG->WDOG_CR = 0x00000000;


	copy_esram_image();

	remap_user_code_eSRAM_0();

//	}

}

void read_adf_reg(rx_cmd_t* rcv_cmd){

	uint8_t data_read[6];
	uint8_t j;

	cmd_adf_read_addr = (rcv_cmd->parameters[1] << 24) | (rcv_cmd->parameters[2] << 16) | (rcv_cmd->parameters[3] << 8) | rcv_cmd->parameters[4];
	cmd_adf_read_No_double_words = rcv_cmd->parameters[0];

	adf_read_from_memory(RMODE_1, cmd_adf_read_addr, data_read, 6);

	cmd_adf_data[0] = (data_read[2] << 24) | (data_read[3] << 16) | (data_read[4] << 8) | (data_read[5]);

	cmd_adf_read_addr += 4;

	adf_read_from_memory(RMODE_1, cmd_adf_read_addr, data_read, 6);

	cmd_adf_data[1] = (data_read[2] << 24) | (data_read[3] << 16) | (data_read[4] << 8) | (data_read[5]);


}

void exe_rtm(rx_cmd_t* rcv_cmd){
	uint8_t i = 0;

	for(;i<16;i++){
		RTM[i] = rcv_cmd->parameters[i];
	}

}

void ctrl_gmc(rx_cmd_t* rcv_cmd){

	if(rcv_cmd->parameters[0] == 0){
		MSS_GPIO_set_output(MSS_GPIO_6, 0);
		NVIC_DisableIRQ(FabricIrq8_IRQn);
	}
	else{
		MSS_GPIO_set_output(MSS_GPIO_6, 1);
		NVIC_EnableIRQ(FabricIrq8_IRQn);
	}
}

void ctrl_comms(rx_cmd_t* rcv_cmd){

	if(rcv_cmd->parameters[0] == 0){
		MSS_GPIO_set_output(MSS_GPIO_2, 0);
		NVIC_DisableIRQ(FabricIrq5_IRQn);
	}
	else{
		MSS_GPIO_set_output(MSS_GPIO_2, 1);
		NVIC_EnableIRQ(FabricIrq5_IRQn);
	}
}

//void cmd_sc_reset(rx_cmd_t* rcv_cmd){
//
//	command_cnt = 0;
//	// Power Cycle other Components;
//}

