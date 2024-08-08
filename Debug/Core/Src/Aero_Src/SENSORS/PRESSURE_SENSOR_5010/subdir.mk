################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/Pressure_Sensor.c 

OBJS += \
./Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/Pressure_Sensor.o 

C_DEPS += \
./Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/Pressure_Sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/%.o: ../Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/%.c Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/LED_WS2812" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/SENSORS/FLOW_SENSOR_7002" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/SENSORS/PRESSURE_SENSOR_5010" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/SENSORS/READ_SENSORS_VALUES" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/SENSORS/SENSORS_OFFSET" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/ALERT" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/APRV" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/BACKUP_PC_CMV" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/BIPAP" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/Bluetooth" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/CALIBRATION" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/CPAP" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/Expiratory_Valve_Control" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/FUEL_GAUGE" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/HFNC" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/NEBULISER" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/OTHERS" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/OXYGEN_BLENDING" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/PC_CMV" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/PC-SIMV" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/PIP_CONTROL" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/PSV" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/Sampled_Parameter" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/SENSORS" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/SUCTION" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/TASK_CREATE_&_PERIPHERALS_INIT" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/VC_CMV" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/VC-SIMV" -I"C:/Users/sathi/OneDrive/Desktop/O2_blending_Test/Jeevan_Lite/Core/Inc/Aero_Inc/VOLUME_CONTROL" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Aero_Src-2f-SENSORS-2f-PRESSURE_SENSOR_5010

clean-Core-2f-Src-2f-Aero_Src-2f-SENSORS-2f-PRESSURE_SENSOR_5010:
	-$(RM) ./Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/Pressure_Sensor.d ./Core/Src/Aero_Src/SENSORS/PRESSURE_SENSOR_5010/Pressure_Sensor.o

.PHONY: clean-Core-2f-Src-2f-Aero_Src-2f-SENSORS-2f-PRESSURE_SENSOR_5010

