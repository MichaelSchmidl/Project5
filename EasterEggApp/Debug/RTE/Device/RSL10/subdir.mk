################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/flashlib/code/rsl10_flash.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_romvect.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_asrc.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_audio.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_clocks.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_crc.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_dma.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_flash.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_power.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_power_modes.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_rffe.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_timers.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_uart.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_version.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/cmsis/source/sbrk.c \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/cmsis/source/start.c \
../RTE/Device/RSL10/system_rsl10.c 

S_UPPER_SRCS += \
../RTE/Device/RSL10/startup_rsl10.S 

OBJS += \
./RTE/Device/RSL10/rsl10_flash.o \
./RTE/Device/RSL10/rsl10_romvect.o \
./RTE/Device/RSL10/rsl10_sys_asrc.o \
./RTE/Device/RSL10/rsl10_sys_audio.o \
./RTE/Device/RSL10/rsl10_sys_clocks.o \
./RTE/Device/RSL10/rsl10_sys_crc.o \
./RTE/Device/RSL10/rsl10_sys_dma.o \
./RTE/Device/RSL10/rsl10_sys_flash.o \
./RTE/Device/RSL10/rsl10_sys_power.o \
./RTE/Device/RSL10/rsl10_sys_power_modes.o \
./RTE/Device/RSL10/rsl10_sys_rffe.o \
./RTE/Device/RSL10/rsl10_sys_timers.o \
./RTE/Device/RSL10/rsl10_sys_uart.o \
./RTE/Device/RSL10/rsl10_sys_version.o \
./RTE/Device/RSL10/sbrk.o \
./RTE/Device/RSL10/start.o \
./RTE/Device/RSL10/startup_rsl10.o \
./RTE/Device/RSL10/system_rsl10.o 

S_UPPER_DEPS += \
./RTE/Device/RSL10/startup_rsl10.d 

C_DEPS += \
./RTE/Device/RSL10/rsl10_flash.d \
./RTE/Device/RSL10/rsl10_romvect.d \
./RTE/Device/RSL10/rsl10_sys_asrc.d \
./RTE/Device/RSL10/rsl10_sys_audio.d \
./RTE/Device/RSL10/rsl10_sys_clocks.d \
./RTE/Device/RSL10/rsl10_sys_crc.d \
./RTE/Device/RSL10/rsl10_sys_dma.d \
./RTE/Device/RSL10/rsl10_sys_flash.d \
./RTE/Device/RSL10/rsl10_sys_power.d \
./RTE/Device/RSL10/rsl10_sys_power_modes.d \
./RTE/Device/RSL10/rsl10_sys_rffe.d \
./RTE/Device/RSL10/rsl10_sys_timers.d \
./RTE/Device/RSL10/rsl10_sys_uart.d \
./RTE/Device/RSL10/rsl10_sys_version.d \
./RTE/Device/RSL10/sbrk.d \
./RTE/Device/RSL10/start.d \
./RTE/Device/RSL10/system_rsl10.d 


# Each subdirectory must supply rules for building sources it contributes
RTE/Device/RSL10/rsl10_flash.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/flashlib/code/rsl10_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_romvect.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_romvect.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_asrc.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_asrc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_audio.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_audio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_clocks.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_clocks.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_crc.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_crc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_dma.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_flash.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_power.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_power.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_power_modes.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_power_modes.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_rffe.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_rffe.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_timers.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_timers.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_uart.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/rsl10_sys_version.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/syslib/code/rsl10_sys_version.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/sbrk.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/cmsis/source/sbrk.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/start.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/cmsis/source/start.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/%.o: ../RTE/Device/RSL10/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -x assembler-with-cpp -D_RTE_ -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RTE/Device/RSL10/%.o: ../RTE/Device/RSL10/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


