/*
 * PIP_Control.h
 *
 *  Created on: Jan 21, 2023
 *      Author: asus
 */

#ifndef INC_PIP_CONTROL_H_
#define INC_PIP_CONTROL_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


typedef struct
{
	uint16_t PIP_Control_Task_Delay;
	uint16_t PIP_Control_Event_Occured;
	uint16_t PIP_Acheived_Time_Ms;
	uint16_t Increase_Each_DAC_SET_CONST;
	uint8_t  Early_Acheived_Ramp_Time_Percentage;
	uint8_t  Last_Early_Acheived_Ramp_Time_Percentage;
	uint8_t  Low_PIP_Count;
	uint16_t  Slowly_Acheived_Ramp_Time_Percentage;

}pip_control_Task;


typedef struct
{
	uint16_t BLOWER_DAC_VAL;
	uint16_t LAST_BLOWER_DAC_VAL;
	float    BLOWER_VOLTAGE_SIGNAL;
	uint16_t BLOWER_ENDING_DAC_SIGNAL;
	float Blower_Signal_Voltage;
	uint16_t PEEP_BLOWER_DAC_VAL;
	uint16_t LAST_PEEP_BLOWER_DAC_VAL;

}Pressure_mode_blower_control;



typedef enum
{
	PIP_NOT_ACHEIVED,
	PIP_ACHEIVED
}pip_status;

typedef enum
{
	READ_TIME_OPEN,
	READ_TIME_CLOSE
}Read_time_vs_pressure;

typedef enum
{
	SET_PIP_VALUE_NOT_ACHEIVED,
	SET_PIP_VALUE_ACHEIVED
}Set_pip_Status;


typedef enum
{
	NO,
	YES
}Need_pid_Task_Delay;


typedef enum
{
	T_OPEN,
	T_LOCK
}Tune_pid_Task_Delay;

pip_control_Task PIP_control_Task;
pip_status PIP_Status;
Read_time_vs_pressure Read_Time_vs_Pressure;
Set_pip_Status Set_PIP_Status;
Need_pid_Task_Delay Need_PID_Task_Delay;
Tune_pid_Task_Delay Tune_PID_Task_Delay;
Pressure_mode_blower_control Pressure_Mode_blower_control;



xTaskHandle PIP_Control_Task_Handler;
void PIP_Control_Task(void *argument);


void PIP_Control(uint8_t);
void PIP_ACHEIVED_EARLY(uint8_t,uint16_t);
void PIP_ACHEIVED_SLOWLY(uint16_t);
void PIP_ACHEIVED_NORMALY();
void _PIP_NOT_ACHEIVED();
void BLOWER_ENDING_DAC_SIGNAL_TUNE(uint8_t,uint8_t,uint8_t,uint8_t);
void INCREASE_EACH_DAC_SET_CONST(uint8_t,uint16_t);


#endif /* INC_PIP_CONTROL_H_ */
