/*
 * PIP_Control.c
 *
 *  Created on: Jan 21, 2023
 *      Author: asus
 */


#include "PIP_Control.h"


uint16_t Minimum_Inc_Value_1=50;
uint16_t Minimum_Inc_Value_2=100;
uint16_t Maximum_Inc_Value=140;


void PIP_Control_Task(void *argument)
{
	while(1)
	{

		if(Run_Current_Breathe_State == Run_Inspiration_Cycle)
		{
			if(Common_Mode_Parameter._FIO2_Val  == 100)
			{
				Pressure_Mode_blower_control.BLOWER_DAC_VAL = 0;
			}
			else
			{
				PIP_Control(Common_Mode_Parameter._PIP_Val);
			}
		}

		vTaskDelay(PIP_control_Task.PIP_Control_Task_Delay);
	}
}



void PIP_Control(uint8_t PIP_VAL)
{
	PIP_control_Task.PIP_Control_Event_Occured++;

	if (Pressure_sensor.Pressure_Val >= (PIP_VAL))
	{
		PIP_Status = PIP_ACHEIVED;
		if(Read_Time_vs_Pressure==READ_TIME_OPEN)
		{
			PIP_Acheieved();
			PIP_control_Task.PIP_Acheived_Time_Ms       = (PIP_control_Task.PIP_Control_Event_Occured*PIP_control_Task.PIP_Control_Task_Delay);
			Pressure_Mode_blower_control.BLOWER_DAC_VAL = Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL;
			Set_PIP_Status                              = SET_PIP_VALUE_ACHEIVED;
			Read_Time_vs_Pressure                       = READ_TIME_CLOSE;
		}



	}
	else if (PIP_Status == PIP_NOT_ACHEIVED)
	{
		if (Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL >= Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL)
		{
			Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL = Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL;
			PIP_Status = PIP_ACHEIVED;
		}
		else if (Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL < Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL)
		{
			Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL = Pressure_Mode_blower_control.BLOWER_DAC_VAL;
			Pressure_Mode_blower_control.BLOWER_DAC_VAL      = (Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL + (PIP_control_Task.Increase_Each_DAC_SET_CONST ));

			if( (Common_Mode_Parameter._PEEP_Val >= Smooth_Ramp_Time_Val) && (Common_Mode_Parameter._PIP_Val  >=30) )
			{
				if (Pressure_Mode_blower_control.BLOWER_DAC_VAL >= Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL)
				{
					Pressure_Mode_blower_control.BLOWER_DAC_VAL      = Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL;
					Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL = Pressure_Mode_blower_control.BLOWER_DAC_VAL;
				}
			}

			if(Pressure_Mode_blower_control.BLOWER_DAC_VAL >=4095)
				Pressure_Mode_blower_control.BLOWER_DAC_VAL=4095;

		}
	}
}




void INCREASE_EACH_DAC_SET_CONST(uint8_t PIP_VAL, uint16_t Rise_Time_MS)
{
	if(PIP_control_Task.PIP_Acheived_Time_Ms <=  (Rise_Time_MS-50) )
	{
		PIP_ACHEIVED_EARLY(PIP_VAL,Rise_Time_MS);
	}
	else if(PIP_control_Task.PIP_Acheived_Time_Ms >=  (Rise_Time_MS+50) )
	{
		PIP_ACHEIVED_SLOWLY(Rise_Time_MS);
	}
	else
	{
		PIP_ACHEIVED_NORMALY();
	}

	    _PIP_NOT_ACHEIVED();
}





void PIP_ACHEIVED_EARLY(uint8_t PIP_VAL,uint16_t Rise_Time_MS)
{
	if(Set_PIP_Status == SET_PIP_VALUE_NOT_ACHEIVED)
	{

	}
	else
	{
		PIP_control_Task.Early_Acheived_Ramp_Time_Percentage = (uint16_t) (  (PIP_control_Task.PIP_Acheived_Time_Ms  / ((float)Rise_Time_MS)) * 100);

		if(PIP_control_Task.Early_Acheived_Ramp_Time_Percentage <= 70)
		{
			PIP_control_Task.Increase_Each_DAC_SET_CONST = (PIP_control_Task.Increase_Each_DAC_SET_CONST - 5 );

			if( (Common_Mode_Parameter._PIP_Val < 30) && (Common_Mode_Parameter._PEEP_Val < 9) )
			{
			    if(PIP_control_Task.Increase_Each_DAC_SET_CONST <= Minimum_Inc_Value_1)
			    {
			    	PIP_control_Task.Increase_Each_DAC_SET_CONST = Minimum_Inc_Value_1;
			    	Need_PID_Task_Delay                          = YES;
			    }
			}
			else
			{
				if(PIP_control_Task.Increase_Each_DAC_SET_CONST <= Minimum_Inc_Value_2)
				{
					PIP_control_Task.Increase_Each_DAC_SET_CONST = Minimum_Inc_Value_2;
					Need_PID_Task_Delay                          = YES;
				}
			}



		}

		else if(  (PIP_control_Task.Early_Acheived_Ramp_Time_Percentage <= 80) && (PIP_control_Task.Early_Acheived_Ramp_Time_Percentage > 70) )
		{
			PIP_control_Task.Increase_Each_DAC_SET_CONST = (PIP_control_Task.Increase_Each_DAC_SET_CONST - 2 );
			if( (Common_Mode_Parameter._PIP_Val < 30) && (Common_Mode_Parameter._PEEP_Val < 9) )
			{
			    if(PIP_control_Task.Increase_Each_DAC_SET_CONST <= Minimum_Inc_Value_1)
			    {
			    	PIP_control_Task.Increase_Each_DAC_SET_CONST = Minimum_Inc_Value_1;
			    	Need_PID_Task_Delay                          = YES;
			    }
			}
			else
			{
				if(PIP_control_Task.Increase_Each_DAC_SET_CONST <= Minimum_Inc_Value_2)
				{
					PIP_control_Task.Increase_Each_DAC_SET_CONST = Minimum_Inc_Value_2;
					Need_PID_Task_Delay                          = YES;
				}
			}
		}

		else if(  (PIP_control_Task.Early_Acheived_Ramp_Time_Percentage <= 90) && (PIP_control_Task.Early_Acheived_Ramp_Time_Percentage > 80) )
		{
			PIP_control_Task.Increase_Each_DAC_SET_CONST = (PIP_control_Task.Increase_Each_DAC_SET_CONST - 1 );

			if( (Common_Mode_Parameter._PIP_Val < 30) && (Common_Mode_Parameter._PEEP_Val < 9) )
			{
			    if(PIP_control_Task.Increase_Each_DAC_SET_CONST <= Minimum_Inc_Value_1)
			    {
			    	PIP_control_Task.Increase_Each_DAC_SET_CONST = Minimum_Inc_Value_1;
			    	Need_PID_Task_Delay                          = YES;
			    }
			}
			else
			{
				if(PIP_control_Task.Increase_Each_DAC_SET_CONST <= Minimum_Inc_Value_2)
				{
					PIP_control_Task.Increase_Each_DAC_SET_CONST = Minimum_Inc_Value_2;
					Need_PID_Task_Delay                          = YES;
				}
			}
		}



		if(Need_PID_Task_Delay == YES)
		{
			if(PIP_Average_Parameter.Maximum_PIP_Acheived  < PIP_VAL)
			{
				PIP_control_Task.Low_PIP_Count++;
				 if(PIP_control_Task.Low_PIP_Count>=5)
				 {
					PIP_control_Task.PIP_Control_Task_Delay = PIP_control_Task.PIP_Control_Task_Delay;
					PIP_control_Task.Low_PIP_Count=6;
					Tune_PID_Task_Delay = T_LOCK;
				 }

			}
			else if(PIP_control_Task.Early_Acheived_Ramp_Time_Percentage  >= PIP_control_Task.Last_Early_Acheived_Ramp_Time_Percentage)
			{
				if(Tune_PID_Task_Delay == T_OPEN)
				{
					  if(PIP_control_Task.Early_Acheived_Ramp_Time_Percentage<60)
					  {
						  PIP_control_Task.PIP_Control_Task_Delay = (PIP_control_Task.PIP_Control_Task_Delay+10);
					  }
					  else if( (PIP_control_Task.Early_Acheived_Ramp_Time_Percentage<70) && (PIP_control_Task.Early_Acheived_Ramp_Time_Percentage>=60) )
					  {
						  PIP_control_Task.PIP_Control_Task_Delay = (PIP_control_Task.PIP_Control_Task_Delay+6);
					  }
					  else if( (PIP_control_Task.Early_Acheived_Ramp_Time_Percentage<=80) && (PIP_control_Task.Early_Acheived_Ramp_Time_Percentage=70) )
					  {
						  PIP_control_Task.PIP_Control_Task_Delay = (PIP_control_Task.PIP_Control_Task_Delay+4);
					  }
					  else
					  {
						  PIP_control_Task.PIP_Control_Task_Delay = (PIP_control_Task.PIP_Control_Task_Delay+1);
					  }
				}
			}
		}

	}
}




void PIP_ACHEIVED_SLOWLY(uint16_t Rise_Time_MS)
{
	PIP_control_Task.Slowly_Acheived_Ramp_Time_Percentage = (uint16_t) ((  PIP_control_Task.PIP_Acheived_Time_Ms  / ((float)Rise_Time_MS) ) * 100);




	if( (PIP_control_Task.Slowly_Acheived_Ramp_Time_Percentage >= 130)  )
	{
		PIP_control_Task.Increase_Each_DAC_SET_CONST = (PIP_control_Task.Increase_Each_DAC_SET_CONST + 5 );
		if(PIP_control_Task.Increase_Each_DAC_SET_CONST >= Maximum_Inc_Value)
		{
			PIP_control_Task.Increase_Each_DAC_SET_CONST = Maximum_Inc_Value;

		}
	}
	else if(  (PIP_control_Task.Slowly_Acheived_Ramp_Time_Percentage> 120) && (PIP_control_Task.Slowly_Acheived_Ramp_Time_Percentage <130) )
	{
		PIP_control_Task.Increase_Each_DAC_SET_CONST = (PIP_control_Task.Increase_Each_DAC_SET_CONST + 2 );
		if(PIP_control_Task.Increase_Each_DAC_SET_CONST >= Maximum_Inc_Value)
		{
			PIP_control_Task.Increase_Each_DAC_SET_CONST = Maximum_Inc_Value;

		}
	}
	else if( (PIP_control_Task.Slowly_Acheived_Ramp_Time_Percentage<=120) && (PIP_control_Task.Slowly_Acheived_Ramp_Time_Percentage >= 110) )
	{
		PIP_control_Task.Increase_Each_DAC_SET_CONST = (PIP_control_Task.Increase_Each_DAC_SET_CONST + 1 );
		if(PIP_control_Task.Increase_Each_DAC_SET_CONST >= Maximum_Inc_Value)
		{
			PIP_control_Task.Increase_Each_DAC_SET_CONST = Maximum_Inc_Value;

		}
	}

}


void PIP_ACHEIVED_NORMALY()
{
	PIP_control_Task.Increase_Each_DAC_SET_CONST = PIP_control_Task.Increase_Each_DAC_SET_CONST;
}


void _PIP_NOT_ACHEIVED()
{
	if(Set_PIP_Status == SET_PIP_VALUE_NOT_ACHEIVED)
	{
		PIP_control_Task.Increase_Each_DAC_SET_CONST = (PIP_control_Task.Increase_Each_DAC_SET_CONST + 1 );
		if(PIP_control_Task.Increase_Each_DAC_SET_CONST >= Maximum_Inc_Value)
		{
			PIP_control_Task.Increase_Each_DAC_SET_CONST = Maximum_Inc_Value;

		}
	}
}



void BLOWER_ENDING_DAC_SIGNAL_TUNE(uint8_t PIP_VAL,uint8_t Respiratory_Rate,uint8_t E_Ratio,uint8_t Rise_Time)
{

	static uint8_t count1,count2;

	if(Respiratory_Rate>=30 || Rise_Time>80 || (E_Ratio>4))
    {
		    if(PIP_Average_Parameter.Maximum_PIP_Acheived > (PIP_VAL+1))
			{
		    	count1++;
		    	count2=0;
		    	if(count1>2)
		    	{
		    		Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL=(Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL-1);
		    		count1=0;
					if(Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL<=500)
					{
						Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL=500;
					}
		    	}
			}
			else if(PIP_Average_Parameter.Maximum_PIP_Acheived < PIP_VAL)
			{
				count2++;
				count1=0;
				if(count2>2)
				{
					Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL=Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL+1;
				count2=0;
				if(  Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL  >= 4095)
					Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = 4095;
				}
			}
			else
			{
				Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL;
			}
    }

 else if(Respiratory_Rate < 30 || Rise_Time <= 80)
 {
	if(PIP_Average_Parameter.PIP_AVG_VAL > PIP_VAL)
	{
		Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = (Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL-5);
		if(Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL<=500)
			Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL=500;
	}
	else if(PIP_Average_Parameter.PIP_AVG_VAL < PIP_VAL)
	{
		Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = (Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL+5);
		if(Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL >= 4095)
			Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = 4095;
	}
	else
	{
		Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL;

	}
 }
}
