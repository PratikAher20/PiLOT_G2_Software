
#ifndef __cmd_
#define __cmd_

#include "stdint.h"

#include "P1.h"
#include "mss_sys_services.h"

#define MAX_CMD_PARAM 32
#define NUM_CMDS 5


typedef struct rx_cmd{

	uint8_t cmd_id;

	uint8_t parameters[MAX_CMD_PARAM];

}__attribute__((packed))rx_cmd_t;


typedef struct cmd{

	uint8_t id;
	uint16_t length;
	void (*ex_func)(rx_cmd_t* rcv_cmd);

}__attribute__((packed))cmd_t;



uint8_t cmd_valid(rx_cmd_t* rx_cmd, uint8_t src);
void add_cmd(uint8_t id, uint16_t length, void (*ex_func)(uint8_t id, rx_cmd_t* rcv_cmd));
void cmd_engine(rx_cmd_t* rx_cmd);

void cmd_noop(rx_cmd_t* rcv_cmd);
void set_pkt_rate(rx_cmd_t* rcv_cmd);
void exe_iap(rx_cmd_t* rcv_cmd);
void read_adf_reg(rx_cmd_t* rcv_cmd);
void cmd_sc_reset(rx_cmd_t* rcv_cmd);
void exe_rtm(rx_cmd_t* rcv_cmd);

typedef struct pkt_rate{

	uint8_t pkt_apid;
	uint8_t pkt_rate;

}__attribute__((packed))pkt_rate_t;


#endif
