/*
 * Flow_Sensor_Method_1.c
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */


#include "Flow_Sensor_Method_1.h"

uint16_t AW_flow_raw;
uint16_t ADS1115_7002_FLOW_SENSOR_OFFSET;
uint8_t  Flow_sensor_7002_Filter_Trigger=1;
uint16_t Trigger_Raw_Flow;
uint16_t AW_flow_raw_filtered;

float Volume_Mul  = 2;        //float Volume_Mul  = 8;
uint8_t  numReadings  = 15;



void Get_Volume()
{
	float Flow_Volume;
	Flow_Volume=(Flow_Sensor_cal.Flow1/60.00)*(Volume_Mul);
	vol.Volume=vol.Volume+Flow_Volume;
	vol.Volume_Val=(int)vol.Volume;
	if(vol.Volume_max<vol.Volume_Val)
	{
		vol.Volume_max=vol.Volume_Val;
	}

}


void  Flow_Sensor_Value()
{
	Get_AW_Flow();
	_Flow();
}


float AW_flow_raw_Volt(uint16_t r)
{
	float volt=0;
	volt=(r*6144.00)/32768;
	return volt;
}

uint16_t AW_flow_moving_average(uint16_t value)
{
	    Flow_sensor.AW_flow_raw1_new = ((value-Flow_sensor.AW_flow_raw_old)/Flow_sensor_7002_Filter)+Flow_sensor.AW_flow_raw_old;
	    Flow_sensor.AW_flow_raw_old = Flow_sensor.AW_flow_raw1_new;
		return Flow_sensor.AW_flow_raw1_new;
}

uint16_t ADS1115_AW_flow_sensor()
{
		uint16_t  ADCraw=0;
		unsigned char ADSwrite[ADS1115_MODULE_WRITE_COMMAND_SIZE];

		ADSwrite[0] = 0x01;
		ADSwrite[1] = 0x40;
		ADSwrite[2] = 0xE3;

      	if(HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS,(ADSwrite), ADS1115_MODULE_WRITE_COMMAND_SIZE, ADS1115_MODULE_WAIT_TIME_1_MS)!=HAL_OK)
      	{
      		ADS1115_Status=ADS1115_MODULE_FAULT;
      	}
      	ADSwrite[0] = 0x00;
		HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS, (ADSwrite), ADS1115_MODULE_WRITE_COMMAND_SIZE_1_BYTE, ADS1115_MODULE_WAIT_TIME_1_MS);



		HAL_I2C_Master_Receive(&hi2c2, (ADS1115_ADDRESS),(Flow_sensor.ADSread),ADS1115_MODULE_READ_COMMAND_SIZE,ADS1115_MODULE_WAIT_TIME_1_MS);
		ADCraw = ((Flow_sensor.ADSread[0]) << 8 | (Flow_sensor.ADSread[1]));

		return   ADCraw;
}

void Get_AW_Flow(void)
{
	AW_flow_raw = ADS1115_AW_flow_sensor();
	AW_flow_raw_filtered = AW_flow_moving_average(AW_flow_raw);
	Trigger_Flow(AW_flow_raw);
	Flow_sensor.AW_flow_milli_volt = AW_flow_raw_Volt(AW_flow_raw_filtered);
}

void Flow_Sensor_7002_offset(void)
{

	for(int i=0;i<=1000;i++)
	{
			Get_AW_Flow();
			Get_AW_Flow_ADS1115_OFFSET(AW_flow_raw);
			Flow_sensor.AW_Flow_Offset=Flow_sensor.AW_flow_milli_volt;
			vTaskDelay(Two_Millisecond_Delay);
	}
	xSemaphoreGive(binarysem);
}





long adj(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}





void _Flow()
{


	    Flow_Sensor_cal.delp_flow1 =(Flow_sensor.AW_flow_milli_volt-Flow_sensor.AW_Flow_Offset)/1000.00;
	    Flow_Sensor_cal.dp=Flow_Sensor_cal.delp_flow1*10;


		if(Flow_Sensor_cal.dp>0)
		{
			Flow_Sensor_cal.Flow1=0.1512*(Flow_Sensor_cal.dp)*(Flow_Sensor_cal.dp)*(Flow_Sensor_cal.dp)-3.3424*(Flow_Sensor_cal.dp)*(Flow_Sensor_cal.dp)+41.657*(Flow_Sensor_cal.dp);
			Flow_Sensor_cal.Flow1=adj(Flow_Sensor_cal.Flow1,0,160,0,185);
		}
		else if(Flow_Sensor_cal.dp<0)
		{
			Flow_Sensor_cal.dp*=-1;
			Flow_Sensor_cal.Flow1=0.1512*Flow_Sensor_cal.dp*Flow_Sensor_cal.dp*Flow_Sensor_cal.dp-3.3424*Flow_Sensor_cal.dp*Flow_Sensor_cal.dp+41.657*Flow_Sensor_cal.dp;
			Flow_Sensor_cal.Flow1=adj(Flow_Sensor_cal.Flow1,0,160,0,185);
			Flow_Sensor_cal.Flow1*=-1;

		}


		      Flow_Sensor_cal.total = Flow_Sensor_cal.total - Flow_Sensor_cal.readings[Flow_Sensor_cal.readIndex];
		      Flow_Sensor_cal.readings[Flow_Sensor_cal.readIndex] = Flow_Sensor_cal.Flow1;
		      Flow_Sensor_cal.total = Flow_Sensor_cal.total + Flow_Sensor_cal.readings[Flow_Sensor_cal.readIndex];
		      Flow_Sensor_cal.readIndex = Flow_Sensor_cal.readIndex + 1;
			  if (Flow_Sensor_cal.readIndex >= numReadings)
			  {
				  Flow_Sensor_cal.readIndex = 0;
			  }
			  Flow_Sensor_cal.average = Flow_Sensor_cal.total / numReadings;
			  Flow_Sensor_cal.Flow1= Flow_Sensor_cal.average;
		if((Flow_Sensor_cal.Flow1<3)&&(Flow_Sensor_cal.Flow1> (-3)) )
	    {
			Flow_Sensor_cal.Flow1=0;
		}




		Flow_Sensor_cal._Flow_Val=(int)Flow_Sensor_cal.Flow1;

			if(Flow_Sensor_cal._Flow_Val> Max_Flow)
			{
				Max_Flow= Flow_Sensor_cal._Flow_Val;
			}
			if(Flow_Sensor_cal._Flow_Val< Min_Flow)
			{
				Min_Flow= Flow_Sensor_cal._Flow_Val;
			}

}



uint16_t Trigger_AW_flow_moving_average(uint16_t value)
{
	Flow_sensor.AW_flow_raw1_new_trigger = ((value-Flow_sensor.AW_flow_raw_old_trigger)/Flow_sensor_7002_Filter_Trigger)+Flow_sensor.AW_flow_raw_old_trigger;
	Flow_sensor.AW_flow_raw_old_trigger  = Flow_sensor.AW_flow_raw1_new_trigger;
	return Flow_sensor.AW_flow_raw1_new_trigger;
}


void Trigger_Flow(uint16_t value)
{


	Trigger_Raw_Flow                     = Trigger_AW_flow_moving_average(value);

	Raw_volt_Flow_Trigger_Temp           = (Trigger_Raw_Flow - ADS1115_7002_FLOW_SENSOR_OFFSET); //14450

	Raw_volt_Flow_Trigger                = (uint16_t)((float) Raw_volt_Flow_Trigger_Temp/10);


	switch(Raw_volt_Flow_Trigger)
	{
	    case 1:
		    FLOW_TRIGGER  =1;
	    case 2:
	    	FLOW_TRIGGER  =2;
	    break;
	    case 3:
	    	FLOW_TRIGGER  =3;
		break;
		case 4:
			FLOW_TRIGGER  =4;
		break;
	    case 5:
	    	FLOW_TRIGGER  =5;
	    break;
	    case 6:
	    	FLOW_TRIGGER  =6;
	    break;
	    case 7:
	    	FLOW_TRIGGER  =7;
		break;
		case 8:
			FLOW_TRIGGER  =8;
		break;
	    case 9:
	    	FLOW_TRIGGER  =9;
		break;
		case 10:
			FLOW_TRIGGER  =10;
		break;
		case 11:
			FLOW_TRIGGER  =11;
		break;
		default:
			FLOW_TRIGGER  =0;
		break;


	}

}


void Get_AW_Flow_ADS1115_OFFSET(uint16_t AW_flow_raw)
{
	ADS1115_7002_FLOW_SENSOR_OFFSET = Trigger_AW_flow_moving_average(AW_flow_raw);
}



