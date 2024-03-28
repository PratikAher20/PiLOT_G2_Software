
#include "cmd.h"

cmd_t* cmd;

// Function checking the CheckSUM
//void chk_sum();

cmd_t cmd_list[NUM_CMDS];

extern uint16_t command_cnt;
extern uint16_t command_reject_cnt;

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
//	add_cmd(1, 3, set_pkt_rate);   //the actual command apid's is one greater than this.
//	add_cmd(2, 2, cmd_sc_reset);

	cmd_list[rx_cmd->cmd_id - 1].ex_func(rx_cmd);

}

// Commands Definition

void cmd_noop(rx_cmd_t* rcv_cmd){

	uint8_t a;

}

//void set_pkt_rate(rx_cmd_t* rcv_cmd){
//
//	if(rcv_cmd->parameters[1] != 0){
//		xTimerChangePeriod(pkt_timer[rcv_cmd->parameters[0]], xMsToTicks(rcv_cmd->parameters[1] * 1000), 0);
//	}
//	else{
//		xTimerStop(pkt_timer[rcv_cmd->parameters[0]], 0);
//	}
//	// Write a function to take in APID and increase the downlink rate of that pkt.
////	if(pkt_rate->pkt_apid == HK_API_ID){
////		xTimerChangePeriod() Change the downlink rate of HK packet
////	}
//
//}

//void cmd_sc_reset(rx_cmd_t* rcv_cmd){
//
//	command_cnt = 0;
//	// Power Cycle other Components;
//}

