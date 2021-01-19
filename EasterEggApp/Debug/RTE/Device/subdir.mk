################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/ble_abstraction_layer/ble/source/stubprf.c 

OBJS += \
./RTE/Device/stubprf.o 

C_DEPS += \
./RTE/Device/stubprf.d 


# Each subdirectory must supply rules for building sources it contributes
RTE/Device/stubprf.o: C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/ble_abstraction_layer/ble/source/stubprf.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DRSL10_CID=101 -DCFG_BLE=1 -DCFG_ALLROLES=1 -DCFG_APP -DCFG_APP_BATT -DCFG_ATTS=1 -DCFG_CON=8 -DCFG_EMB=1 -DCFG_HOST=1 -DCFG_RF_ATLAS=1 -DCFG_ALLPRF=1 -DCFG_PRF=1 -DCFG_NB_PRF=8 -DCFG_CHNL_ASSESS=1 -DCFG_SEC_CON=1 -DCFG_EXT_DB -DCFG_PRF_BASS=1 -DCFG_PRF_DISS=1 -DCFG_PRF_HOGPD=1 -D_RTE_ -I"../include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ARM/CMSIS/5.6.0/CMSIS/Driver/Include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/drivers/dma_driver/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/drivers/gpio_driver/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/drivers/pwm_driver/include" -I"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/drivers/spi_driver/include" -I"C:\projekte\work\Project5\EasterEggApp/RTE" -I"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ARM/CMSIS/5.6.0/CMSIS/Driver/Include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/bb" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/ble/profiles" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/include/kernel" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/drivers/dma_driver/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/drivers/gpio_driver/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/drivers/pwm_driver/include" -isystem"C:/Users/mschmidl/ON_Semiconductor/PACK/ONSemiconductor/RSL10/3.4.154/source/firmware/drivers/spi_driver/include" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE" -isystem"C:\projekte\work\Project5\EasterEggApp/RTE/Device/RSL10" -std=gnu11 -Wmissing-prototypes -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


