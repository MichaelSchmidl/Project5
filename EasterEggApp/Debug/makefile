################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include code/subdir.mk
-include RTE/Device/RSL10/subdir.mk
-include RTE/Device/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
EasterEggApp.hex \

SECONDARY_SIZE += \
EasterEggApp.siz \


# All Target
all: EasterEggApp.elf secondary-outputs

# Tool invocations
EasterEggApp.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -T "C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10/sections.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"EasterEggApp.map" --specs=nano.specs -v -o "EasterEggApp.elf" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

EasterEggApp.hex: EasterEggApp.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "EasterEggApp.elf"  "EasterEggApp.hex"
	@echo 'Finished building: $@'
	@echo ' '

EasterEggApp.siz: EasterEggApp.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "EasterEggApp.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) EasterEggApp.elf
	-@echo ' '

post-build:
	-@echo 'FOTA generation'
	-arm-none-eabi-objcopy -O binary "EasterEggApp.elf" "EasterEggApp.bin" && "C:/projekte/work/Project5/EasterEggApp/RTE/Device/RSL10/mkfotaimg.py" -o "EasterEggApp.fota" "C:/projekte/work/Project5/EasterEggApp/RTE/Device/RSL10/fota.bin" "EasterEggApp.bin"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents post-build

-include ../makefile.targets
