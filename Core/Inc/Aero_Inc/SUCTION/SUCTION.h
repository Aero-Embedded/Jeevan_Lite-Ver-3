/*
 * SUCTION.h
 *
 *  Created on: Mar 15, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_SUCTION_SUCTION_H_
#define INC_AERO_INC_SUCTION_SUCTION_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


xTaskHandle Suction_Handler;
void Suction_Task(void *argument);

#endif /* INC_AERO_INC_SUCTION_SUCTION_H_ */
