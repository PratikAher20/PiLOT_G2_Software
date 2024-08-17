################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include cmds/subdir.mk
-include PIS/subdir.mk
-include OBC/subdir.mk
-include GMC/subdir.mk
-include Comms/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
MEMORYMAP += \
memory-map.xml \

IHEXFILES += \
PiLOT_sb_MSS_CM3_app.hex \

SRECFILES += \
PiLOT_sb_MSS_CM3_app.srec \

LISTINGS += \
PiLOT_sb_MSS_CM3_app.lst \


# All Target
all: PiLOT_sb_MSS_CM3_app secondary-outputs

# Tool invocations
PiLOT_sb_MSS_CM3_app: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU C Linker'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -L"E:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\Debug" -TE:\PiLOT_G2_Software\PiLOT_G2_Software\PiLOT_G2_IMG2\PiLOT_sb_MSS_CM3_hw_platform\CMSIS\startup_gcc\production-smartfusion2-execute-in-place.ld -Wl,-Map=$(EXECUTABLE).map -Xlinker -gc-sections -o"PiLOT_sb_MSS_CM3_app" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

memory-map.xml: $(EXECUTABLE)
	@echo 'Invoking: Memory map generator'
	actel-map -o "memory-map.xml" $(EXECUTABLE) $(BUILDCMD)
	@echo 'Finished building: $@'
	@echo ' '

PiLOT_sb_MSS_CM3_app.hex: $(EXECUTABLE)
	@echo 'Invoking: GNU Intel Hex File Generator'
	arm-none-eabi-objcopy -O ihex $(EXECUTABLE) "PiLOT_sb_MSS_CM3_app.hex"
	@echo 'Finished building: $@'
	@echo ' '

PiLOT_sb_MSS_CM3_app.srec: $(EXECUTABLE)
	@echo 'Invoking: GNU S-Record Generator'
	arm-none-eabi-objcopy -O srec $(EXECUTABLE) "PiLOT_sb_MSS_CM3_app.srec"
	@echo 'Finished building: $@'
	@echo ' '

PiLOT_sb_MSS_CM3_app.lst: $(EXECUTABLE)
	@echo 'Invoking: GNU Listing Generator'
	arm-none-eabi-objdump -h -S $(EXECUTABLE) > "PiLOT_sb_MSS_CM3_app.lst"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean::
	-$(RM) $(MEMORYMAP)$(OBJS)$(C_DEPS)$(SRECFILES)$(IHEXFILES)$(LISTINGS)$(EXECUTABLE) PiLOT_sb_MSS_CM3_app
	-@echo ' '

secondary-outputs: $(MEMORYMAP) $(IHEXFILES) $(SRECFILES) $(LISTINGS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
