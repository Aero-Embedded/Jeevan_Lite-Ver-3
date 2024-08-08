/*
 * Volume_Control.c
 *
 *  Created on: Feb 1, 2023
 *      Author: asus
 */


#include "Volume_control.h"



void Volume_Control_Task (void *argument)
{
	while(1)
	{
		if(Run_Current_Breathe_State == Run_Inspiration_Cycle)
		{
			if(Common_Mode_Parameter._FIO2_Val  == 100)
			{
				Volume_Mode_Blower_control.BLOWER_DAC_VAL = 0;
			}
			else
			{
				if(Pressure_sensor.Pressure_Val >= 60)
				{
					Volume_Mode_Blower_control.BLOWER_DAC_VAL=0;
					Blower_Status =BlOWER_OFF;
				}
				else
				{
					if(Blower_Status == BlOWER_ON)
					{
					   Volume_Control(Common_Mode_Parameter._VOLUME_Val,Common_Mode_Parameter._FLOW_RATE,Volume_control_Task.Volume_Stop_Value);
					}
				}
			}
		}

		vTaskDelay(Ten_Millisecond_Delay);
	}
}





void Volume_Control(uint16_t Set_Volume_Val,uint8_t Set_Flow_Rate,uint8_t Volume_Stop_Value)
{
	if(Run_Current_Breathe_State == Run_Inspiration_Cycle)
	{
		Set_Volume_Val= (Set_Volume_Val-Volume_Stop_Value);
			if(vol.Volume <= Set_Volume_Val)
			{
				Volume_Achieved_Time++;
				Volume_Array[Volume_Array_count]=Flow_Sensor_cal._Flow_Val;
				Volume_Array_count++;
				if(Volume_Array_count>=300)
					Volume_Array_count=300;


				  if( (Sampled_Parameter_3.Staic_Compliance_int < 10)  )
				  {
					   Volume_Mode_Blower_control.BLOWER_DAC_VAL          = Volume_Mode_Blower_control.INITIAL_BLOWER_DAC_VAL;
						if(Flow_Sensor_cal._Flow_Val >= Set_Flow_Rate)
						{
							  if(Maximum_Flow_Achieved  == FLOW_NOT_ACHIEVED)
							  {
									Maximum_Flow_Achieved                     = FLOW_ACHIEVED;

							  }
					   }

				  }

				  else
				  {
						 if(Volume_Achieved_Status == VOLUME_NOT_ACHIEVED)
						 {
							 Volume_Mode_Blower_control.BLOWER_DAC_VAL          =   Volume_Mode_Blower_control.INITIAL_BLOWER_DAC_VAL+INCREMENT_DAC_VAL_PER_ITERATION;
							 Volume_Mode_Blower_control.INITIAL_BLOWER_DAC_VAL  =   Volume_Mode_Blower_control.BLOWER_DAC_VAL;
									if(Volume_Mode_Blower_control.BLOWER_DAC_VAL > 4095)
									{
										Volume_Mode_Blower_control.BLOWER_DAC_VAL = 4095;
									}
									if(Flow_Sensor_cal._Flow_Val >= Set_Flow_Rate)
									{
										  if(Maximum_Flow_Achieved  == FLOW_NOT_ACHIEVED)
										  {
												Maximum_Flow_Achieved                     = FLOW_ACHIEVED;
												TIME_Base_parameter.FLOW_ACHIEVED_TIMER   = 100;
										  }
								   }


						 }
				  }

			}
			else
			{
			    Total_Volume_Achieved_Time                        = (Volume_Achieved_Time *10);
				Volume_Acheieved();
				Volume_Achieved_Status                            = VOLUME_ACHIEVED;
				Volume_Mode_Blower_control.BLOWER_DAC_VAL         = RESET;
				Volume_Mode_Blower_control.INITIAL_BLOWER_DAC_VAL = Volume_Mode_Blower_control.PREVIOUS_INITIAL_BLOWER_DAC_VAL;

			}

		}
}




void VOLUME_STOP_POINT(uint16_t Set_Volume,uint16_t Achieved_Volume)
{
	static uint8_t count;
	static uint8_t count_2;
       if(Achieved_Volume > (Set_Volume + 10))
	   {
    	   count++;
			if (count >= 3)
			{
				Volume_control_Task.Volume_Stop_Value = Volume_control_Task.Volume_Stop_Value + 5;
				count   = 0;
				count_2 = 0;

				if(Volume_control_Task.Volume_Stop_Value >= 100)
					Volume_control_Task.Volume_Stop_Value = 100;
			}
		}
       else if(Achieved_Volume < (Set_Volume - 10))
	   {
    	   count_2++;
			if (count_2 >= 3)
			{
				Volume_control_Task.Volume_Stop_Value = Volume_control_Task.Volume_Stop_Value - 5;
				count_2 = 0;
				count   = 0;
				if(Volume_control_Task.Volume_Stop_Value <= 5)
									Volume_control_Task.Volume_Stop_Value = 5;
			}
		}
       else
       {
			count_2 = 0;
			count   = 0;
       }
}





uint16_t TUNE_INITIAL_DAC_VAL(uint16_t Previous_Dac_Val, uint8_t Flow_Rate,uint8_t Flow_Max)
{
	uint16_t DAC_VALUE=Previous_Dac_Val;
	    if (Flow_Max > (Flow_Rate +5))
		{
			if (Flow_Max > (Flow_Rate + 20))
			{
				DAC_VALUE = Previous_Dac_Val - 20;
			}
			else if ((Flow_Max <= (Flow_Rate + 20))&& (Flow_Max >= (Flow_Rate + 10)))
			{
				DAC_VALUE = Previous_Dac_Val - 5;
			}
			else if ((Flow_Max < (Flow_Rate + 10)) && (Flow_Max >= (Flow_Rate + 5)))
			{
				DAC_VALUE = Previous_Dac_Val - 1;
			}
			if (DAC_VALUE < 500)
			{
				DAC_VALUE = 500;
			}
		}
		else if ( (Flow_Max < (Flow_Rate- 5) ))
		{
			DAC_VALUE = Previous_Dac_Val + 30;
			    if (DAC_VALUE >= 2500)
				{
					DAC_VALUE = 2500;
				}
		}
		else
		{
			DAC_VALUE     = Previous_Dac_Val;
		}
	    return DAC_VALUE;
}



uint16_t LOW_Compliance_DAC_value(uint8_t Flow_Rate)
{
	uint16_t Temp_DAC;

	if(Flow_Rate >= 60)
		Temp_DAC = 1110;
	else if( (Flow_Rate < 60) && (Flow_Rate >= 50) )
		Temp_DAC = 1000;
	else if( (Flow_Rate < 50) && (Flow_Rate >= 40) )
		Temp_DAC = 850;
	else if( (Flow_Rate < 40) && (Flow_Rate >= 30) )
		Temp_DAC = 800;
	else if( (Flow_Rate < 30) && (Flow_Rate >= 20) )
		Temp_DAC = 700;
	else if( (Flow_Rate < 20) && (Flow_Rate >= 10) )
		Temp_DAC = 600;
	else if( (Flow_Rate < 10) && (Flow_Rate > 0) )
		Temp_DAC = 600;

	return Temp_DAC;
}
