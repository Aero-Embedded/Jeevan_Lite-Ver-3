/*
 * Bluetooth_Data_Split.h
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */

#ifndef INC_BLUETOOTH_DATA_SPLIT_H_
#define INC_BLUETOOTH_DATA_SPLIT_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



typedef enum
{
	Idle=0,
	Pccmv=1,
	Vccmv=2,
	Aprv=4,
	Pcsimv=5,
	Vcsimv=6,
	Psv=7,
	Spont=10,
	Cpap=11,
	BiPap=12,
	Hfnc=13,
	Suction=15,
	Backup_Pccmv=21,
}Modes;

typedef enum
{
	No_Backup_Mode,
	Backup_PCCMV_Mode,
}Backup_Modes;



typedef enum
{
	Generate_Inspiration_Cycle = 0,
	Generate_Expiration_Cycle,
	NoWaveFormState
}Next_half_Breath_Cycle;

typedef enum
{
	No_Run_State=0,
	Run_Inspiration_Cycle,
	Run_Expiration_Cycle
}Run_current_Breathe_State;


Modes Mode,Running_Mode;
Backup_Modes Current_Backup_Mode;
Next_half_Breath_Cycle Next_Half_Breath_Cycle;
Run_current_Breathe_State Run_Current_Breathe_State;


uint8_t chksum8_2(const unsigned char *buff, size_t len);




#endif /* INC_BLUETOOTH_DATA_SPLIT_H_ */
