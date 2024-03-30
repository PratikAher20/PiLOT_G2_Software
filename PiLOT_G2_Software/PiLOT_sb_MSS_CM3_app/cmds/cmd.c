
#include "cmd.h"

cmd_t* cmd;

// Function checking the CheckSUM
//void chk_sum();

cmd_t cmd_list[NUM_CMDS];

extern uint16_t command_cnt;
extern uint16_t command_reject_cnt;
extern timer_instance_t hk_timer;
extern timer_instance_t comms_timer;

uint8_t cmd_valid(rx_cmd_t* rx_cmd){
	return 1;  //Will be checking the validated of the checksum.
}


void add_cmd(uint8_t id, uint16_t length, void (*ex_func)(uint8_t id, rx_cmd_t* rcv_cmd)){

	cmd_list[id].id = id;
	cmd_list[id].length = length;
	cmd_list[id].ex_func = ex_func;

}

void cmd_engine(rx_cmd_t* rx_cmd){

	add_cmd(0, 2, cmd_noop);
	add_cmd(1, 3, set_pkt_rate);   //the actual command apid's is one greater than this.
//	add_cmd(2, 2, cmd_sc_reset);

	cmd_list[rx_cmd->cmd_id - 1].ex_func(rx_cmd);

}

// Commands Definition

void cmd_noop(rx_cmd_t* rcv_cmd){

	uint8_t a;

}

void set_pkt_rate(rx_cmd_t* rcv_cmd){

	uint32_t new_time_period = MSS_SYS_M3_CLK_FREQ/1024 * rcv_cmd->parameters[1];

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
	}
	else{
		if(rcv_cmd->parameters[0] == hk){
			NVIC_DisableIRQ(FabricIrq4_IRQn);
		}
		else if(rcv_cmd->parameters[0] == comms){
			NVIC_DisableIRQ(FabricIrq5_IRQn);
		}
	}

}

//void cmd_sc_reset(rx_cmd_t* rcv_cmd){
//
//	command_cnt = 0;
//	// Power Cycle other Components;
//}

