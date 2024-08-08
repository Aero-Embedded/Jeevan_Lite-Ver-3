/*
 * Expiratory_Valve_Control_Volume_Mode.h
 *
 *  Created on: Feb 3, 2023
 *      Author: asus
 */

#ifndef INC_EXPIRATORY_VALVE_CONTROL_VOLUME_MODE_H_
#define INC_EXPIRATORY_VALVE_CONTROL_VOLUME_MODE_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"

uint16_t PEEP_Control_For_Expiratory_Valve_Open_Time_In_Volume_Mode(uint8_t,uint8_t,uint32_t,uint8_t,uint16_t);
//uint16_t Below_30_RR_Control(uint8_t,uint8_t,uint32_t,uint8_t,uint16_t);
//uint16_t Above_30_RR_Control(uint8_t,uint8_t,uint32_t,uint8_t,uint16_t);

#endif /* INC_EXPIRATORY_VALVE_CONTROL_VOLUME_MODE_H_ */
