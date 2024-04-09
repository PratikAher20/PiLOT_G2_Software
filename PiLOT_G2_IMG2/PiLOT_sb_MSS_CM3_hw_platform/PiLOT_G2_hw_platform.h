#ifndef PiLOT_G2_HW_PLATFORM_H_
#define PiLOT_G2_HW_PLATFORM_H_
/*****************************************************************************
*
*Created by Microsemi SmartDesign  Sun Mar 24 07:25:32 2024
*
*Memory map specification for peripherals in PiLOT_G2
*/

/*-----------------------------------------------------------------------------
* CM3 subsystem memory map
* Initiator(s) for this subsystem: CM3 
*---------------------------------------------------------------------------*/
#define COREI2C_0_0                     0x50000000U
#define COREI2C_1_0                     0x50001000U
#define COREI2C_2_0                     0x50002000U
#define COREI2C_3_0                     0x50003000U
#define CORESPI_C0_0                    0x50004000U
#define RS_485_CONTROLLER_0             0x50005000U
#define CORETIMER_C0_0                  0x50006000U
#define CORETIMER_C1_0                  0x50007000U
#define CORETIMER_C2_0                  0x50008000U
#define CORETIMER_C3_0                  0x50009000U
#define CORETIMER_C4_0                  0x5000A000U


#endif /* PiLOT_G2_HW_PLATFORM_H_*/
