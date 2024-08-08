/*
 * Sensor_Read.h
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */

#ifndef INC_SENSOR_READ_H_
#define INC_SENSOR_READ_H_



#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"




xTaskHandle Sensor_Read_Handler;

void Sensor_Read_Task(void *argument);

#endif /* INC_SENSOR_READ_H_ */
