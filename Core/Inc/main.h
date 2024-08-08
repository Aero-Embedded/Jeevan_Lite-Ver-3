/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_device.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "Sensor_Offset.h"
#include "Task_Create.h"
#include "Problems_Happen.h"
#include "Pressure_Sensor.h"
#include "Flow_Sensor_Method_1.h"
#include "Two_Milli_Second_Task.h"
#include "Sensor_Read.h"
#include "Bluetooth_Graph.h"
#include "Macros.h"
#include "Bluetooth_Data_Split.h"
#include "Pc_cmv.h"
#include "Mode_Initial_Flag_Set.h"
#include "PIP_Control.h"
#include "Common_Parameter.h"
#include "Vc_Cmv.h"
#include "Volume_control.h"
#include "Expiratory_Valve_Control_Pressure_Mode.h"
#include "Expiratory_Valve_Control_Volume_Mode.h"
#include "Oxygen_Blending.h"
#include "Alert.h"
#include "Pc_Simv_Assist_ON.h"
#include "Pc_Simv_Assist_OFF.h"
#include "Vc_Simv_Assist_ON.h"
#include "Vc_Simv_Assist_OFF.h"
#include "PSV.h"
#include "Backup_Pc_cmv.h"
#include "BiPap.h"
#include "Cpap.h"
#include "APRV.h"
#include "Nebuliser.h"
#include "SUCTION.h"
#include "HFNC.h"
#include "pid.h"
#include "Calibration.h"
#include "Sampled_Parameter.h"
#include "Max17261.h"
#include "LED_WS2812.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
UART_HandleTypeDef huart6;
UART_HandleTypeDef huart5;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

float O2_Flow_Val;
float O2_Flow_Sensor_Voltage;


char UART_GARAPH_SAMPLES[16];
char USB_GARAPH_SAMPLES[16];


uint8_t Device_Control;
int Pressure_Trigger_Offset;
int Flow_Trigger_Offset;
uint32_t Patient_Trigger_Count;

uint8_t Smooth_Ramp_Time_Val,Smooth_Ramp_Time_Val_Count;


uint8_t FLOW_TRIGGER,LAST_FLOW_TRIGGER;
int16_t Raw_volt_Flow_Trigger,Raw_volt_Flow_Trigger_Temp;
uint16_t DAC_VALUE;

uint8_t  Volume_Array[300];
uint16_t Volume_Array_count;
uint8_t INCREMENT_DAC_VAL_PER_ITERATION;

double Temp, PIDOut, TempSetpoint,Temp_Flow;
uint16_t HFNC_DAC_VALUE;
uint16_t HFNC_Dac1;
uint16_t HFNC_Dac2;
uint8_t kp;
uint8_t ki;
double kd;
int Time;
extern int Speed;


uint16_t One_Minite_Or_60000_Ms;
uint8_t Breath_In_Minite,Breath;

float Led_Delay;
uint8_t LED_ITER;

int Received_APP_data_found,Received_index_Number,Received_index_number2;
int arrangecount;

uint16_t Total_Volume_Achieved_Time;
uint16_t Volume_Achieved_Time;

SemaphoreHandle_t  binarysem;

uint8_t Receive_Led_Signal;
uint8_t Alert_Status_Led_Strip;


uint8_t Led_Pause_Status;


float Bat_Avg,Bat_Avg_count;
uint8_t Battery_Voltage;

uint8_t Blower_Status;

#define UART_COMMUNICATION_1
#define USB_COMMUNICATION_1

#define UART5_GRAPH_DATA_DEBUG_0
#define USB_GRAPH_DATA_DEBUG_0
#define ADS1115_PRESSURE_SENSOR_DEBUG_0
#define PID_ON_0



/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Blower_Enable_Pin GPIO_PIN_5
#define Blower_Enable_GPIO_Port GPIOE
#define Oxygen_Sensor_Pin GPIO_PIN_0
#define Oxygen_Sensor_GPIO_Port GPIOA
#define O2_Flow_Sensor_Pin GPIO_PIN_2
#define O2_Flow_Sensor_GPIO_Port GPIOA
#define Pressure_Sensor_Pin GPIO_PIN_3
#define Pressure_Sensor_GPIO_Port GPIOA
#define Blower_Signal_Pin GPIO_PIN_4
#define Blower_Signal_GPIO_Port GPIOA
#define Propotional_valve_signal_Pin GPIO_PIN_5
#define Propotional_valve_signal_GPIO_Port GPIOA
#define Battery_Voltage_Read_Pin GPIO_PIN_5
#define Battery_Voltage_Read_GPIO_Port GPIOC
#define ADS1115_SCL_Pin GPIO_PIN_10
#define ADS1115_SCL_GPIO_Port GPIOB
#define ADS1115_SDA_Pin GPIO_PIN_11
#define ADS1115_SDA_GPIO_Port GPIOB
#define Expiratory_Valve_Control_Pin GPIO_PIN_13
#define Expiratory_Valve_Control_GPIO_Port GPIOB
#define Servo_Motor_Signal_Pin GPIO_PIN_14
#define Servo_Motor_Signal_GPIO_Port GPIOB
#define Nebuliser_Motor_Control_Pin GPIO_PIN_15
#define Nebuliser_Motor_Control_GPIO_Port GPIOB
#define Bluetooth_Tx_Pin GPIO_PIN_6
#define Bluetooth_Tx_GPIO_Port GPIOC
#define Blutooth_Rx_Pin GPIO_PIN_7
#define Blutooth_Rx_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
