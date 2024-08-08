/*
 * Oxygen_Blending.h
 *
 *  Created on: Feb 8, 2023
 *      Author: asus
 */

#ifndef INC_OXYGEN_BLENDING_H_
#define INC_OXYGEN_BLENDING_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



#define O2_Sensor_Pin_Voltage Adc.AdcData[1]
#define Parkar_valve_Signal(Y)  DAC1->DHR12R2=Y;
#define Servo_Angle(Z)  TIM12->CCR1=Z;

typedef struct
{
	uint32_t OXYGEN_Event_Count;
	uint32_t OXYGEN_Cumulative_Val;
	float OXYGEN_AVG_VAL;
	uint8_t  Achieved_Oxygen;
	uint8_t  Servo_Move_Forward;
	uint8_t  Servo_Move_Backward;
	uint8_t  Servo_Position;
	float    Servo_Duty_Cycle;
	float    Servo_Millisecond;
	int      O2_DAC;
	float Parker_Signal_Voltage;

}Oxygen_parameter;


typedef enum
{
	SERVO_FULL_OPEND  = 45,
	SERVO_FULL_CLOSED = 90
}servo_position_feedback;


typedef enum
{
	PRESSURE_CONTROLLED,
	VOLUME_CONTROLLED
}fio2_blending_method;

typedef enum
{
	OXYGEN_ACHIEVED,
	OXYGEN_NOT_ACHIEVED
}oxygen_blending_status;


Oxygen_parameter Oxygen_Parameter;
servo_position_feedback  Servo_Position_Feedback;
fio2_blending_method Fio2_Blending_Method;
oxygen_blending_status Oxygen_Blending_Status;


xTaskHandle Oxygen_Blending_Handler;
void Oxygen_Blending_Task(void *argument);
void Servo_Info(uint8_t);
void Oxygen_Blending(uint8_t);
void O2_Initial_Parameter(uint8_t);
void Adjust_Servo_And_Calculate_O2(uint8_t);
void Get_Oxygen_Value(uint8_t);
void Low_Oxygen_Achieved(uint8_t);
void High_Oxygen_Achieved(uint8_t);
void O2_DAC_Control(uint8_t);
uint16_t Set_Initial_O2_DAC_VAL_Pressure_Modes(uint8_t);
uint16_t Set_Initial_O2_DAC_VAL_Volume_Modes(uint8_t);
void Servo_Position_Volume_Controlled_Mode(uint8_t);
void Servo_Position_Pressure_Controlled_Mode(uint8_t);


#endif /* INC_OXYGEN_BLENDING_H_ */
