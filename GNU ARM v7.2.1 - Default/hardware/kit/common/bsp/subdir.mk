################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/kit/common/bsp/bsp_stk.c 

OBJS += \
./hardware/kit/common/bsp/bsp_stk.o 

C_DEPS += \
./hardware/kit/common/bsp/bsp_stk.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/kit/common/bsp/bsp_stk.o: ../hardware/kit/common/bsp/bsp_stk.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__StackLimit=0x20000000' '-DNVM3_DEFAULT_NVM_SIZE=24576' '-D__HEAP_SIZE=0xD00' '-DEFR32MG22A224F512IM40=1' -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/Device/SiliconLabs/EFR32MG22/Include" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emlib/src" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emlib/inc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/CMSIS/Include" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/common/inc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/protocol/bluetooth/ble_stack/inc/common" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/hardware/kit/common/drivers" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/service/sleeptimer/inc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/hardware/kit/common/halconfig" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/radio/rail_lib/protocol/ble" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emdrv/nvm3/inc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/radio/rail_lib/common" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/radio/rail_lib/protocol/ieee802154" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/protocol/bluetooth/ble_stack/inc/soc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/app/bluetooth/common/util" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/service/sleeptimer/src" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emdrv/nvm3/src" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emdrv/sleep/inc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/hardware/kit/common/bsp" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/Device/SiliconLabs/EFR32MG22/Source" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/hardware/kit/EFR32MG22_BRD4182A/config" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/service/sleeptimer/config" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/halconfig/inc/hal-config" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emdrv/sleep/src" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emdrv/gpiointerrupt/inc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emdrv/common/inc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/Device/SiliconLabs/EFR32MG22/Source/GCC" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/bootloader/api" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/emdrv/uartdrv/inc" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt" -I"/Users/clfilho/SimplicityStudio/March20/BG22HwPeripheralsGatt/platform/bootloader" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv5-sp-d16 -mfloat-abi=hard -MMD -MP -MF"hardware/kit/common/bsp/bsp_stk.d" -MT"hardware/kit/common/bsp/bsp_stk.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


