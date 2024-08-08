/*
 * BiPap.h
 *
 *  Created on: Feb 24, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_BIPAP_BIPAP_H_
#define INC_AERO_INC_BIPAP_BIPAP_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


typedef struct
{
	uint16_t  IPAP_Val;
	uint8_t   EPAP_Val;
	uint16_t  TRIG_TYPE;
    float    TRIG_LMT;
	uint16_t  TRIG_TIME;
	uint16_t  CALC_TRIG_VAL;
	uint16_t  TOLERANCE_EWAVE;
	uint16_t  TRIG_WINDOW;
	uint16_t  APNEA_TIME;
	uint8_t   _IPAP_TIME;
	int       APNEA_COUNTER;
	int       IPAP_TIME;
}bipap_mode_Parameter;



typedef struct
{
	uint16_t BLOWER_DAC_VAL;
	uint16_t LAST_BLOWER_DAC_VAL;
	uint16_t BLOWER_ENDING_DAC_SIGNAL;
	uint16_t EPAP_BLOWER_DAC_VAL;
	uint16_t LAST_EPAP_BLOWER_DAC_VAL;
	float    Blower_Signal_Voltage;
	float    BLOWER_VOLTAGE_SIGNAL;

}bipap_mode_blower_control;


typedef struct
{
	int BIPAP_EPAP_Settle_Time;
	int Blower_Wait_Time_Milli_Second;
	int Trigger_Check_Wait_Time;
	int _INSPIRATION_TIME;
}bipap_Flags;


typedef struct
{
	uint32_t EPAP_Cumulative_Val;
	uint32_t EPAP_Event_Count;
	uint8_t  EPAP_AVG_VAL;
}bipap_peep_average;

typedef struct
{
	uint8_t  Maximum_IPAP_Acheived;
	uint32_t IPAP_Cumulative_Val;
	uint32_t IPAP_Event_Count;
	uint8_t  IPAP_AVG_VAL;

}bipap_pip_average;


xTaskHandle BIPAP_Handler;
xTaskHandle BIPAP_PID_Handler;

bipap_mode_Parameter BIPAP;
bipap_Flags BIPAP_Control;
bipap_mode_blower_control BIPAP_Mode_Blower_control;
bipap_peep_average BIPAP_Peep_Average;
bipap_pip_average BIPAP_Pip_average;


void BIPAP_Task(void *argument);
void BIPAP_PID_Task(void *argument);



#endif /* INC_AERO_INC_BIPAP_BIPAP_H_ */
