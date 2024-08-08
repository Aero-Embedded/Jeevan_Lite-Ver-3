/*
 * Task_Create.h
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */

#ifndef INC_TASK_CREATE_H_
#define INC_TASK_CREATE_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"

uint32_t free_heap_size;


void Initial_Task_Create();
void ADS1115_initialisation();

#endif /* INC_TASK_CREATE_H_ */
