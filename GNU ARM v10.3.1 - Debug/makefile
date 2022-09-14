################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include gecko_sdk_4.1.1/platform/service/udelay/src/subdir.mk
-include gecko_sdk_4.1.1/platform/service/system/src/subdir.mk
-include gecko_sdk_4.1.1/platform/service/sleeptimer/src/subdir.mk
-include gecko_sdk_4.1.1/platform/service/device_init/src/subdir.mk
-include gecko_sdk_4.1.1/platform/emlib/src/subdir.mk
-include gecko_sdk_4.1.1/platform/emdrv/uartdrv/src/subdir.mk
-include gecko_sdk_4.1.1/platform/emdrv/gpiointerrupt/src/subdir.mk
-include gecko_sdk_4.1.1/platform/emdrv/dmadrv/src/subdir.mk
-include gecko_sdk_4.1.1/platform/driver/leddrv/src/subdir.mk
-include gecko_sdk_4.1.1/platform/driver/i2cspm/src/subdir.mk
-include gecko_sdk_4.1.1/platform/driver/debug/src/subdir.mk
-include gecko_sdk_4.1.1/platform/common/toolchain/src/subdir.mk
-include gecko_sdk_4.1.1/platform/common/src/subdir.mk
-include gecko_sdk_4.1.1/platform/Device/SiliconLabs/EFR32MG21/Source/subdir.mk
-include gecko_sdk_4.1.1/hardware/driver/configuration_over_swo/src/subdir.mk
-include gecko_sdk_4.1.1/hardware/board/src/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Test1
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: Test1.axf

# Tool invocations
Test1.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "C:\Users\Mukarram Saeed\SimplicityStudio\v5_workspace\Test1\autogen\linkerfile.ld" -Xlinker --gc-sections -Xlinker -Map="Test1.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o Test1.axf -Wl,--start-group "./RFID_display.o" "./app.o" "./i2c_init.o" "./main.o" "./rc522.o" "./rtc3231.o" "./ssd1306.o" "./autogen/sl_board_default_init.o" "./autogen/sl_device_init_clocks.o" "./autogen/sl_event_handler.o" "./autogen/sl_i2cspm_init.o" "./autogen/sl_simple_rgb_pwm_led_instances.o" "./autogen/sl_uartdrv_init.o" "./gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.o" "./gecko_sdk_4.1.1/hardware/board/src/sl_board_init.o" "./gecko_sdk_4.1.1/hardware/driver/configuration_over_swo/src/sl_cos.o" "./gecko_sdk_4.1.1/platform/Device/SiliconLabs/EFR32MG21/Source/startup_efr32mg21.o" "./gecko_sdk_4.1.1/platform/Device/SiliconLabs/EFR32MG21/Source/system_efr32mg21.o" "./gecko_sdk_4.1.1/platform/common/src/sl_assert.o" "./gecko_sdk_4.1.1/platform/common/toolchain/src/sl_memory.o" "./gecko_sdk_4.1.1/platform/driver/debug/src/sl_debug_swo.o" "./gecko_sdk_4.1.1/platform/driver/i2cspm/src/sl_i2cspm.o" "./gecko_sdk_4.1.1/platform/driver/leddrv/src/sl_led.o" "./gecko_sdk_4.1.1/platform/driver/leddrv/src/sl_pwm_led.o" "./gecko_sdk_4.1.1/platform/driver/leddrv/src/sl_simple_rgb_pwm_led.o" "./gecko_sdk_4.1.1/platform/emdrv/dmadrv/src/dmadrv.o" "./gecko_sdk_4.1.1/platform/emdrv/gpiointerrupt/src/gpiointerrupt.o" "./gecko_sdk_4.1.1/platform/emdrv/uartdrv/src/uartdrv.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_burtc.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_cmu.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_core.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_emu.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_gpio.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_i2c.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_ldma.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_msc.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_rtcc.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_system.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_timer.o" "./gecko_sdk_4.1.1/platform/emlib/src/em_usart.o" "./gecko_sdk_4.1.1/platform/service/device_init/src/sl_device_init_emu_s2.o" "./gecko_sdk_4.1.1/platform/service/device_init/src/sl_device_init_hfrco.o" "./gecko_sdk_4.1.1/platform/service/device_init/src/sl_device_init_hfxo_s2.o" "./gecko_sdk_4.1.1/platform/service/device_init/src/sl_device_init_lfxo_s2.o" "./gecko_sdk_4.1.1/platform/service/device_init/src/sl_device_init_nvic.o" "./gecko_sdk_4.1.1/platform/service/sleeptimer/src/sl_sleeptimer.o" "./gecko_sdk_4.1.1/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o" "./gecko_sdk_4.1.1/platform/service/sleeptimer/src/sl_sleeptimer_hal_prortc.o" "./gecko_sdk_4.1.1/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./gecko_sdk_4.1.1/platform/service/system/src/sl_system_init.o" "./gecko_sdk_4.1.1/platform/service/system/src/sl_system_process_action.o" "./gecko_sdk_4.1.1/platform/service/udelay/src/sl_udelay.o" "./gecko_sdk_4.1.1/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: Test1.hex'
	arm-none-eabi-objcopy -O ihex "Test1.axf" "Test1.hex"
	@echo ' '

	@echo 'Building bin file: Test1.bin'
	arm-none-eabi-objcopy -O binary "Test1.axf" "Test1.bin"
	@echo ' '

	@echo 'Building s37 file: Test1.s37'
	arm-none-eabi-objcopy -O srec "Test1.axf" "Test1.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "Test1.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) Test1.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
