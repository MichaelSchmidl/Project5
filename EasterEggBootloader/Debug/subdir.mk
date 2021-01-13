################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drv_targ.c \
../drv_uart.c \
../sys_boot.c \
../sys_upd.c 

OBJS += \
./drv_targ.o \
./drv_uart.o \
./sys_boot.o \
./sys_upd.o 

C_DEPS += \
./drv_targ.d \
./drv_uart.d \
./sys_boot.d \
./sys_upd.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall  -g3 -DRSL10_CID=101 -D_DEBUG -D_RTE_ -I"C:\projekte\work\Project5\EasterEggBootloader" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:\projekte\work\Project5\EasterEggBootloader/RTE" -I"C:\projekte\work\Project5\EasterEggBootloader/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:\projekte\work\Project5\EasterEggBootloader/RTE" -isystem"C:\projekte\work\Project5\EasterEggBootloader/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sys_boot.o: ../sys_boot.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall  -g3 -DRSL10_CID=101 -D_DEBUG -D_RTE_ -I"C:\projekte\work\Project5\EasterEggBootloader" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:\projekte\work\Project5\EasterEggBootloader/RTE" -I"C:\projekte\work\Project5\EasterEggBootloader/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:\projekte\work\Project5\EasterEggBootloader/RTE" -isystem"C:\projekte\work\Project5\EasterEggBootloader/RTE/Device/RSL10" -std=gnu11 -fno-lto -Wmissing-prototypes -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"sys_boot.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


