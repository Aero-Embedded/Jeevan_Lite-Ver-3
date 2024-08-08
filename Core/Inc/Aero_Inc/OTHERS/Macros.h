/*
 * Leds.h
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */

#ifndef INC_MACROS_H_
#define INC_MACROS_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"

#define Two_Milli_Second_Task_Led_Toggle()             GPIOE->ODR^=(1<<15)
#define Sensor_Read_Task_Led_Toggle()        		   GPIOE->ODR^=(1<<14)
#define Bluetooth_Data_Transmit_Task_Led_Toggle()      GPIOE->ODR^=(1<<13)
#define Initial_Flag_Set_Task_Led_Toggle()             GPIOE->ODR^=(1<<12)
#define One_Time_Task_Led_Toggle()                     GPIOE->ODR^=(1<<8)
#define Mode_Task_Led_Toggle()                         GPIOE->ODR^=(1<<9)
#define Pid_Task_Led_Toggle()                          GPIOE->ODR^=(1<<11)
//#define Dummy_Task_Led_Toggle()       	               GPIOE->ODR^=(1<<10)

//#define PIP_Not_Acheieved()  GPIOA->ODR|=((1<7))
//#define PIP_Acheieved() GPIOA->ODR&=(~(1<<7))



#define ExpValve_OPEN()  (GPIOB->ODR&=(~(1<<13)))
#define ExpValve_CLOSE() (GPIOB->ODR|=((1<<13)))

#define Blower_ON()  GPIOE->ODR|=(1<<5)
#define Blower_OFF()  GPIOE->ODR&=(~(1<<5))


#define Buzzer1_ON()  GPIOD->ODR|=(1<<6)
#define Buzzer1_OFF()  GPIOD->ODR&=(~(1<<6))
#define Buzzer2_ON()  GPIOD->ODR|=(1<<7)
#define Buzzer2_OFF()  GPIOD->ODR&=(~(1<<7))


#define Nebuliser_OFF()  (GPIOB->ODR&=(~(1<<15)))
#define Nebuliser_ON() (GPIOB->ODR|=((1<<15)))



#define Uart_Receive_Debug_Toggle_Led()  GPIOD->ODR^=(1<<1)

#define Blower_Signal(X)  DAC1->DHR12R1=X;

#endif /* INC_MACROS_H_ */
