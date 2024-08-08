/*
 * Flow_Sensor_Method_1.h
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */

#ifndef INC_FLOW_SENSOR_METHOD_1_H_
#define INC_FLOW_SENSOR_METHOD_1_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



extern I2C_HandleTypeDef hi2c2;

#define ADS1115_ADDRESS  (0x48 <<1)
#define ADS1115_ADDRESS_2  (0x49 <<1)

#define Flow_sensor_7002_Filter 4


typedef enum
{
	ADS1115_MODULE_WRITE_COMMAND_SIZE_1_BYTE = 0x01,
	ADS1115_MODULE_WRITE_COMMAND_SIZE = 0x03,
	ADS1115_MODULE_READ_COMMAND_SIZE = 0x02,
	ADS1115_MODULE_WAIT_TIME_1_MS = 1,
	ADS1115_MODULE_WAIT_TIME_2_MS = 2,

}ADS1115;



typedef enum
{
	ADS1115_MODULE_WORKING,
	ADS1115_MODULE_FAULT
}ADS1115_CONDITION;

typedef struct
{
	uint16_t AW_Flow_Offset;
	unsigned char ADSread[ADS1115_MODULE_READ_COMMAND_SIZE];

	unsigned char ADSread_2[ADS1115_MODULE_READ_COMMAND_SIZE];

	uint16_t AW_flow_milli_volt;
	uint16_t AW_flow_raw1_new;
	uint16_t AW_flow_raw_old;
	uint16_t  ADS1115_raw;
	uint16_t AW_flow_raw1_new_trigger;
	uint16_t AW_flow_raw_old_trigger;

}Flow;

typedef struct
{
	float delp_flow1;
	float dp;
	float dp1;
	double Flow1;
	int readings[15];
	int readIndex;
	long total ;
	long average;
	int _Flow_Val;
}Flow2;



typedef struct
{

	float Volume;
	int  Volume_Val;
	int Volume_max;
}Volume;




Flow Flow_sensor;
Flow2 Flow_Sensor_cal;

ADS1115_CONDITION ADS1115_Status;
Volume vol;


float AW_flow_raw_Volt(uint16_t r);
uint16_t AW_flow_moving_average(uint16_t value);
uint16_t ADS1115_AW_flow_sensor();
void Get_AW_Flow(void);
long adj(long x, long in_min, long in_max, long out_min, long out_max);
void _Flow();
void Flow_Sensor_7002_offset();
void Flow_Sensor_Value();
void Get_Volume();
void Trigger_Flow(uint16_t);
void Get_AW_Flow_ADS1115_OFFSET(uint16_t);
void Flow_Sensor_Offset();



#endif /* INC_FLOW_SENSOR_METHOD_1_H_ */
