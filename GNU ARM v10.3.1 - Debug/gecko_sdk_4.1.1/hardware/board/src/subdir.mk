################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Mukarram\ Saeed/SimplicityStudio/SDKs/gecko_sdk/hardware/board/src/sl_board_control_gpio.c \
C:/Users/Mukarram\ Saeed/SimplicityStudio/SDKs/gecko_sdk/hardware/board/src/sl_board_init.c 

OBJS += \
./gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.o \
./gecko_sdk_4.1.1/hardware/board/src/sl_board_init.o 

C_DEPS += \
./gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.d \
./gecko_sdk_4.1.1/hardware/board/src/sl_board_init.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.o: C:/Users/Mukarram\ Saeed/SimplicityStudio/SDKs/gecko_sdk/hardware/board/src/sl_board_control_gpio.c gecko_sdk_4.1.1/hardware/board/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFR32MG21A020F1024IM32=1' '-DSL_BOARD_NAME="BRD4180B"' '-DSL_BOARD_REV="A03"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"C:\Users\Mukarram Saeed\SimplicityStudio\v5_workspace\Test1\config" -I"C:\Users\Mukarram Saeed\SimplicityStudio\v5_workspace\Test1\autogen" -I"C:\Users\Mukarram Saeed\SimplicityStudio\v5_workspace\Test1" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/EFR32MG21/Include" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//hardware/driver/configuration_over_swo/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/driver/debug/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/dmadrv/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/common/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/gpiointerrupt/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/driver/i2cspm/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/driver/leddrv/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/service/sleeptimer/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/uartdrv/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gecko_sdk_4.1.1/hardware/board/src/sl_board_init.o: C:/Users/Mukarram\ Saeed/SimplicityStudio/SDKs/gecko_sdk/hardware/board/src/sl_board_init.c gecko_sdk_4.1.1/hardware/board/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFR32MG21A020F1024IM32=1' '-DSL_BOARD_NAME="BRD4180B"' '-DSL_BOARD_REV="A03"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"C:\Users\Mukarram Saeed\SimplicityStudio\v5_workspace\Test1\config" -I"C:\Users\Mukarram Saeed\SimplicityStudio\v5_workspace\Test1\autogen" -I"C:\Users\Mukarram Saeed\SimplicityStudio\v5_workspace\Test1" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/EFR32MG21/Include" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//hardware/driver/configuration_over_swo/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/driver/debug/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/dmadrv/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/common/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/gpiointerrupt/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/driver/i2cspm/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/driver/leddrv/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/service/sleeptimer/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/uartdrv/inc" -I"C:/Users/Mukarram Saeed/SimplicityStudio/SDKs/gecko_sdk//platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.1.1/hardware/board/src/sl_board_init.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


