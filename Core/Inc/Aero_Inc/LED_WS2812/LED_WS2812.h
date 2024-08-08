/*
 * LED_WS2812.h
 *
 *  Created on: Jul 26, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_LED_WS2812_LED_WS2812_H_
#define INC_AERO_INC_LED_WS2812_LED_WS2812_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"




typedef enum
{
	Idle_LED,
	INSPIRATION_lED,
	EXPIRATION_LED

}LED_status;

LED_status LED_STATUS;

xTaskHandle LED_Strip_Task_Handler;
void LED_Strip_Task(void *argument);

void Green_Led_Inspiration_Expiration();
void Set_Brightness (int);
void WS2812_Send (void);
void Loading_Led();
void Blue_Led();
void Initial_Led();
void PC_CMV_Inspiration_Time_LED_Function(float);
void PC_CMV_Expiration_Time_LED_Function(float);
void Toggle_Led();
void Blue_Led_Resume();
void Green_Led();
void Pink_Led();


#endif /* INC_AERO_INC_LED_WS2812_LED_WS2812_H_ */
