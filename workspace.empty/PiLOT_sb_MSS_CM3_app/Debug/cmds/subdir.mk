################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cmds/cmd.c 

OBJS += \
./cmds/cmd.o 

C_DEPS += \
./cmds/cmd.d 


# Each subdirectory must supply rules for building sources it contributes
cmds/%.o: ../cmds/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -I"E:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_app\Includes" -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreSPI -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\CMSIS -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\CMSIS\startup_gcc -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreI2C -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreUARTapb -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_gpio -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_hpdma -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_i2c -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_nvm -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_spi -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_sys_services -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_timer -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_uart -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers_config -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\drivers_config\sys_config -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3 -IE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


