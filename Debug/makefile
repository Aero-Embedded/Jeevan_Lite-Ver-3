################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/Aero_Src/VOLUME_CONTROL/subdir.mk
-include Core/Src/Aero_Src/VC_CMV/subdir.mk
-include Core/Src/Aero_Src/VC-SIMV/subdir.mk
-include Core/Src/Aero_Src/TASK_CREATE_&_PERIPHERALS_INIT/subdir.mk
-include Core/Src/Aero_Src/Sampled_Parameter/subdir.mk
-include Core/Src/Aero_Src/SUCTION/subdir.mk
-include Core/Src/Aero_Src/SENSORS/SENSORS_OFFSET/subdir.mk
-include Core/Src/Aero_Src/SENSORS/READ_SENSORS_VALUES/subdir.mk
-include Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/subdir.mk
-include Core/Src/Aero_Src/SENSORS/FLOW_SENSOR_7002/subdir.mk
-include Core/Src/Aero_Src/PSV/subdir.mk
-include Core/Src/Aero_Src/PIP_CONTROL/subdir.mk
-include Core/Src/Aero_Src/PC_CMV/subdir.mk
-include Core/Src/Aero_Src/PC-SIMV/subdir.mk
-include Core/Src/Aero_Src/OXYGEN_BLENDING/subdir.mk
-include Core/Src/Aero_Src/OTHERS/subdir.mk
-include Core/Src/Aero_Src/NEBULISER/subdir.mk
-include Core/Src/Aero_Src/LED_WS2812/subdir.mk
-include Core/Src/Aero_Src/HFNC/subdir.mk
-include Core/Src/Aero_Src/FUEL_GAUGE/subdir.mk
-include Core/Src/Aero_Src/Expiratory_Valve_Control/subdir.mk
-include Core/Src/Aero_Src/CPAP/subdir.mk
-include Core/Src/Aero_Src/CALIBRATION/subdir.mk
-include Core/Src/Aero_Src/Bluetooth/subdir.mk
-include Core/Src/Aero_Src/BIPAP/subdir.mk
-include Core/Src/Aero_Src/BACKUP_PC_CMV/subdir.mk
-include Core/Src/Aero_Src/APRV/subdir.mk
-include Core/Src/Aero_Src/ALERT/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Jeevan_Lite
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Jeevan_Lite.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Jeevan_Lite.list \

OBJCOPY_BIN += \
Jeevan_Lite.bin \


# All Target
all: main-build

# Main-build Target
main-build: Jeevan_Lite.elf secondary-outputs

# Tool invocations
Jeevan_Lite.elf: $(OBJS) $(USER_OBJS) C:\Users\sathi\OneDrive\Desktop\O2_blending_Test\Jeevan_Lite\STM32F407VETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Jeevan_Lite.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\sathi\OneDrive\Desktop\O2_blending_Test\Jeevan_Lite\STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Jeevan_Lite.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Jeevan_Lite.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Jeevan_Lite.list"
	@echo 'Finished building: $@'
	@echo ' '

Jeevan_Lite.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Jeevan_Lite.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Jeevan_Lite.bin Jeevan_Lite.elf Jeevan_Lite.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
