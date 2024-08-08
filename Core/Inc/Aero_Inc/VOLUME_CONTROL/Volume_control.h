/*
 * Volume_control.h
 *
 *  Created on: Feb 1, 2023
 *      Author: asus
 */

#ifndef INC_VOLUME_CONTROL_H_
#define INC_VOLUME_CONTROL_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


//#define INCREMENT_DAC_VAL_PER_ITERATION 3

typedef struct
{
	uint8_t  Volume_Stop_Value;
	uint8_t  Flow_Max_Achieved;
	uint16_t Achieved_Volume;

}volume_control_Task;


typedef struct
{
	uint16_t BLOWER_DAC_VAL;
	uint16_t INITIAL_BLOWER_DAC_VAL;
	uint16_t PREVIOUS_INITIAL_BLOWER_DAC_VAL;
	float Blower_Signal_Voltage;
	uint16_t PEEP_BLOWER_DAC_VAL;
	uint16_t LAST_PEEP_BLOWER_DAC_VAL;
}Volume_mode_blower_control;


typedef enum
{
	VOLUME_NOT_ACHIEVED,
	VOLUME_ACHIEVED
}volume_achieved_status;



typedef enum
{
	FLOW_NOT_ACHIEVED,
	FLOW_ACHIEVED
}flow_achieved_status;

volume_control_Task Volume_control_Task;
Volume_mode_blower_control Volume_Mode_Blower_control;

volume_achieved_status Volume_Achieved_Status;
flow_achieved_status Maximum_Flow_Achieved;

xTaskHandle Volume_Control_Task_Handler;
void Volume_Control_Task (void *argument);

void Volume_Control(uint16_t,uint8_t,uint8_t);
void VOLUME_STOP_POINT(uint16_t,uint16_t);
uint16_t TUNE_INITIAL_DAC_VAL(uint16_t, uint8_t ,uint8_t );


#endif /* INC_VOLUME_CONTROL_H_ */
