/*
 * Nebuliser.h
 *
 *  Created on: Mar 14, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_NEBULISER_NEBULISER_H_
#define INC_AERO_INC_NEBULISER_NEBULISER_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"




typedef struct  __attribute__((packed))
{
	uint16_t  _header;
	uint8_t   _length;
	uint8_t   NEBULISER;
	uint8_t   MANUOVER;
	uint8_t   VENTILATION_SETTING;
	uint8_t   TUBE_SIZE;
	uint8_t   COMPENSATION;
	uint8_t   NC1;
	uint8_t   NC2;
	uint8_t   NC3;
	uint8_t   NC4;
	uint8_t   NC5;
	uint8_t   NC6;
	uint8_t   NC7;
	uint8_t   NC8;
	uint8_t   NC9;
	uint8_t   NC10;
	uint8_t   NC11;
	uint8_t   _CRC8 ;
} NEBULISER_PACKET ;


typedef struct
{
	uint8_t NEBULISER_ON_Val;
	uint8_t NEBULISER_SYNC_ON_Val;
	int     NEBULISER_TIME;
	int     NEBULISER_TIMER;
	uint8_t SUCTION;
}NEBULISER_FLAGS;







xTaskHandle Nebuliser_Handler;

xTaskHandle O2_Calibration_Handler;

NEBULISER_FLAGS Nebuliser;


void Nebuliser_Task(void *argument);
void Split_Nebuliser_Packets(NEBULISER_PACKET * );
void Nebuliser_Func();
void Call_Shutdown_Func();

uint8_t chksum8_cal_neb(const unsigned char *buff, size_t len);
void Shutdown_Task(void *argument);

void Nebuliser_Timing();


#endif /* INC_AERO_INC_NEBULISER_NEBULISER_H_ */
