/*
 * Pressure_Sensor.h
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */

#ifndef INC_PRESSURE_SENSOR_H_
#define INC_PRESSURE_SENSOR_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


#define Pressure_Sensor_Sensitivity 44.13

#define Sensor_Input_Voltage 5000
#define ADC_12_Bit_Value 4095
#define Pressure_Sensor_Pin_Voltage Adc.AdcData[2]

typedef struct
{
	uint16_t Pressure_Sensor_Offset_Val;
	uint8_t  Pressure_Val;
	uint16_t Last_Runtime_Pressure_Val;
}Pressure;


typedef struct
{
	uint16_t AdcData[6];
}ADC_Pheriperal;


typedef enum
{
	One_Millisecond_Delay    =1,
	Two_Millisecond_Delay    =2,
	Ten_Millisecond_Delay    =10,
	Twenty_MilliSecond_Delay =20,
	Hundred_Millisecond_Delay=100,
	Four_Second_Delay        =4000,
}Task_Wait_time;




Pressure Pressure_sensor;
ADC_Pheriperal Adc;


void Pressure_Sensor_offset();
uint16_t Pressure_Sensor_Mv(uint16_t P_value);
void Pressure_Sensor_Value();
uint16_t Pressure_Sensor_Moving_Average(uint16_t);
uint16_t ADS1115_Pressure_sensor();
float Pressure_Sensor_raw_Volt(uint16_t);


#endif /* INC_PRESSURE_SENSOR_H_ */
