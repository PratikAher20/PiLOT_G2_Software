################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device_drivers/adf7030.c 

OBJS += \
./device_drivers/adf7030.o 

C_DEPS += \
./device_drivers/adf7030.d 


# Each subdirectory must supply rules for building sources it contributes
device_drivers/%.o: ../device_drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreSPI -I"C:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\AHAN_FSW\AHAN_sb_MSS_CM3_hw_platform\drivers\CoreSPI" -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform -I"C:\D_Drive\Srinidhi\BTech\SSpace\CDH\AHAN\FSW\AHAN_FSW\AHAN_sb_MSS_CM3_hw_platform\drivers" -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\CMSIS -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\CMSIS\startup_gcc -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreGPIO -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreI2C -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreUARTapb -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_gpio -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_hpdma -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_i2c -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_nvm -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_spi -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_sys_services -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_timer -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_uart -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers_config -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\drivers_config\sys_config -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\hal -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3 -IC:\Users\S-SPACE\Desktop\Srinidhi\AHAN_Git\AHAN\FSW\testing\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


