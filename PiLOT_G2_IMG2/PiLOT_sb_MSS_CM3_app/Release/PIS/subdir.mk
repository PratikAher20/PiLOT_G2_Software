################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PIS/RS485_Controller.c \
../PIS/VC_Sensor.c 

OBJS += \
./PIS/RS485_Controller.o \
./PIS/VC_Sensor.o 

C_DEPS += \
./PIS/RS485_Controller.d \
./PIS/VC_Sensor.d 


# Each subdirectory must supply rules for building sources it contributes
PIS/%.o: ../PIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -DNDEBUG -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\CMSIS -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\CMSIS\startup_gcc -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreI2C -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreUARTapb -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_gpio -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_hpdma -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_i2c -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_nvm -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_spi -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_sys_services -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_timer -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_uart -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers_config -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\drivers_config\sys_config -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\hal -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3 -IC:\Users\S-SPACE\Desktop\Pratik\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


