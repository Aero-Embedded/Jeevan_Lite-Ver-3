/*
 * Pressure_Sensor.c
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */


#include "Pressure_Sensor.h"



uint16_t Runtime_Pressure_Val_filterd;
uint8_t Pressure_sensor_Filter=4;

uint16_t Pressure_Sensor_Moving_Average(uint16_t Sensor_Value)
{
	 Sensor_Value = ((Sensor_Value-Pressure_sensor.Last_Runtime_Pressure_Val)/Pressure_sensor_Filter)+Pressure_sensor.Last_Runtime_Pressure_Val;
	 Pressure_sensor.Last_Runtime_Pressure_Val = Sensor_Value;
     return Sensor_Value;
}


uint16_t Pressure_Sensor_Mv(uint16_t P_value)
{
 	  return (((P_value)*Sensor_Input_Voltage)/ADC_12_Bit_Value);
}

void Pressure_Sensor_offset(void)
{
	uint16_t Pressure_Sensor_Offset_Raw_Val;
	//uint16_t Ads1115_Raw_Value;
	int Loop_End=1000;


  	for(int i=0;i<Loop_End;i++)
  	{

#ifdef ADS1115_PRESSURE_SENSOR_DEBUG_0
  		Pressure_Sensor_Offset_Raw_Val=Pressure_Sensor_Mv(Pressure_Sensor_Pin_Voltage);
#endif

#ifdef ADS1115_PRESSURE_SENSOR_DEBUG_1
     Ads1115_Raw_Value    = ADS1115_Pressure_sensor();
     Pressure_Sensor_Offset_Raw_Val = AW_flow_raw_Volt(Ads1115_Raw_Value);
#endif

  	    Pressure_sensor.Pressure_Sensor_Offset_Val=Pressure_Sensor_Moving_Average(Pressure_Sensor_Offset_Raw_Val);
  		vTaskDelay(Two_Millisecond_Delay);
  	}
}

void Pressure_Sensor_Value()
{
	 uint16_t Runtime_Pressure_Val;
	 float    Pressure_Mv_Val_;
	 float    P_cmh2o_Val;
	 //uint16_t Ads1115_Raw_Value;

#ifdef ADS1115_PRESSURE_SENSOR_DEBUG_0
     Runtime_Pressure_Val=Pressure_Sensor_Mv(Pressure_Sensor_Pin_Voltage);
#endif

#ifdef ADS1115_PRESSURE_SENSOR_DEBUG_1

     Ads1115_Raw_Value    = ADS1115_Pressure_sensor();
     Runtime_Pressure_Val = AW_flow_raw_Volt(Ads1115_Raw_Value);
#endif




     Runtime_Pressure_Val_filterd=Pressure_Sensor_Moving_Average(Runtime_Pressure_Val);
     Pressure_Mv_Val_=(Runtime_Pressure_Val_filterd-Pressure_sensor.Pressure_Sensor_Offset_Val);
     P_cmh2o_Val=(Pressure_Mv_Val_/Pressure_Sensor_Sensitivity);
     Pressure_sensor.Pressure_Val=(uint8_t)P_cmh2o_Val;
}




uint16_t ADS1115_Pressure_sensor()
{
		uint16_t  ADCraw=0;
		unsigned char ADSwrite[ADS1115_MODULE_WRITE_COMMAND_SIZE];

		ADSwrite[0] = 0x01;
		ADSwrite[1] = 0x40;
		ADSwrite[2] = 0xE3;

      	if(HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS_2,(ADSwrite), ADS1115_MODULE_WRITE_COMMAND_SIZE, ADS1115_MODULE_WAIT_TIME_1_MS)!=HAL_OK)
      	{
      		ADS1115_Status=ADS1115_MODULE_FAULT;
      	}
      	ADSwrite[0] = 0x00;
		HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS_2, (ADSwrite), ADS1115_MODULE_WRITE_COMMAND_SIZE_1_BYTE, ADS1115_MODULE_WAIT_TIME_1_MS);



		HAL_I2C_Master_Receive(&hi2c2, (ADS1115_ADDRESS_2),(Flow_sensor.ADSread_2),ADS1115_MODULE_READ_COMMAND_SIZE,ADS1115_MODULE_WAIT_TIME_1_MS);
		ADCraw = ((Flow_sensor.ADSread_2[0]) << 8 | (Flow_sensor.ADSread_2[1]));

		return   ADCraw;
}


float Pressure_Sensor_raw_Volt(uint16_t r)
{
	float volt=0;
	volt=(r*6144.00)/32768;
	return volt;
}
