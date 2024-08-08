/*
 * HFNC.h
 *
 *  Created on: Mar 15, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_HFNC_HFNC_H_
#define INC_AERO_INC_HFNC_HFNC_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



typedef struct
{
	uint16_t _Flow_Rate;
	uint8_t _FIO2_Val;

}HFNC_Mode_Parameter;

xTaskHandle HFNC_Handler;
void HFNC_Task (void *argument);
uint16_t HFNC_DAC_Value(uint8_t);

HFNC_Mode_Parameter HFNC;

#endif /* INC_AERO_INC_HFNC_HFNC_H_ */
