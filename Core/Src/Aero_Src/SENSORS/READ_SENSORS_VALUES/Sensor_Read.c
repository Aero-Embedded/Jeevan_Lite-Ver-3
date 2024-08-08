/*
 * Sensor_Read.c
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */


#include "Sensor_Read.h"

extern uint16_t AW_flow_raw;
void Sensor_Read_Task(void *argument)
{
	while(1)
	{
		Pressure_Sensor_Value();
		Flow_Sensor_Value();
		Temp_Flow    =  Flow_Sensor_cal._Flow_Val;
		Get_Volume();
		Get_Oxygen_Value(Common_Mode_Parameter._FIO2_Val );
		vTaskDelay(Two_Millisecond_Delay);
	}
}
