################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/mss_hpdma/mss_hpdma.c 

OBJS += \
./drivers/mss_hpdma/mss_hpdma.o 

C_DEPS += \
./drivers/mss_hpdma/mss_hpdma.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/mss_hpdma/%.o: ../drivers/mss_hpdma/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\CMSIS -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\CMSIS\startup_gcc -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreI2C -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreUARTapb -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_gpio -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_hpdma -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_i2c -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_nvm -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_spi -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_sys_services -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_timer -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_uart -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers_config -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers_config\sys_config -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3 -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


