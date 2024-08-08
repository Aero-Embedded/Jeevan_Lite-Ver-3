/*
 * Sensor_Offset.h
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */

#ifndef INC_SENSOR_OFFSET_H_
#define INC_SENSOR_OFFSET_H_



#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


xTaskHandle Sensor_Offset_Handler;
xTaskHandle One_Time_Handler;
void Sensor_Offset_Task(void *argument);
void One_Time_Task(void *argument);

#endif /* INC_SENSOR_OFFSET_H_ */
