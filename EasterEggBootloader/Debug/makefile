################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include RTE/Device/RSL10/subdir.mk
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
EasterErrBootloader.hex \

SECONDARY_SIZE += \
EasterErrBootloader.siz \


# All Target
all: EasterErrBootloader.elf secondary-outputs

# Tool invocations
EasterErrBootloader.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall  -g3 -T "C:\projekte\work\Project5\EasterEggBootloader/RTE/Device/RSL10/sections.ld" -nostartfiles -Xlinker --gc-sections -Xlinker --sort-section -Xlinker alignment -Wl,-Map,"EasterErrBootloader.map" --specs=nano.specs -Wl,--sort-section,alignment -o "EasterErrBootloader.elf" -Wl,--start-group $(OBJS) $(USER_OBJS) $(LIBS) -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

EasterErrBootloader.hex: EasterErrBootloader.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "EasterErrBootloader.elf"  "EasterErrBootloader.hex"
	@echo 'Finished building: $@'
	@echo ' '

EasterErrBootloader.siz: EasterErrBootloader.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "EasterErrBootloader.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) EasterErrBootloader.elf
	-@echo ' '

post-build:
	-@echo 'Invoking: Cross ARM GNU Create Updater Image'
	-arm-none-eabi-objcopy -O binary "EasterErrBootloader.elf" "EasterErrBootloader.bin"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents post-build

-include ../makefile.targets
