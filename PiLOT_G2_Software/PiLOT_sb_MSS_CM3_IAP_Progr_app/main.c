
#include "mss_sys_services.h"
#include "mss_spi.h"
#include "mss_gpio.h"

void delay ( volatile unsigned int n)
{
	while(n!=0)
	{
		n--;
	}
}


int main(){

	uint8_t prog_status, auth_status;
	uint8_t ERR_LOG = 0;

	MSS_GPIO_init();
	MSS_GPIO_config(MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE);
	uint8_t i=100;
//	while(1){
//		MSS_GPIO_set_output(MSS_GPIO_2, 1);
//		uint8_t j = 0;
//		for(;j<100;j++){
//
//		}
//		MSS_GPIO_set_output(MSS_GPIO_2, 0);
//		j = 0;
//		for(;j<100;j++){
//
//				}
////		i--;
//	}

	while(1){
		MSS_SYS_init(MSS_SYS_NO_EVENT_HANDLER);
		MSS_SPI_init( &g_mss_spi0 );
		MSS_SPI_configure_master_mode(&g_mss_spi0, MSS_SPI_SLAVE_0, MSS_SPI_MODE0, 8u, 8);

		MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );

		g_mss_spi0.hw_reg->CONTROL |= (0x04000000);
		delay(80000);

		auth_status = MSS_SYS_initiate_iap(MSS_SYS_PROG_AUTHENTICATE, 0x400000);

		delay(80000);

		if(auth_status){
			ERR_LOG = ERR_LOG | 0x01;
		}
		else{
			prog_status = MSS_SYS_initiate_iap(MSS_SYS_PROG_PROGRAM, 0x400000);
		}
	}

	return 0;

}
