################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/mss_gpio/mss_gpio.c 

OBJS += \
./drivers/mss_gpio/mss_gpio.o 

C_DEPS += \
./drivers/mss_gpio/mss_gpio.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/mss_gpio/%.o: ../drivers/mss_gpio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\CMSIS -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\CMSIS\startup_gcc -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreI2C -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreUARTapb -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_gpio -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_hpdma -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_i2c -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_nvm -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_spi -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_sys_services -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_timer -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_uart -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers_config -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers_config\sys_config -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3 -ID:\SSAPCE_Lab_Material\Final_Year_Project\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


