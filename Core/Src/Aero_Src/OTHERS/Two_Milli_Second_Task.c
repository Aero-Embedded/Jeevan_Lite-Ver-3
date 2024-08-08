/*
 * Two_Milli_Second_Task.c
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */


#include "Two_Milli_Second_Task.h"



void Time_Trace_Task (void *argument)
{
	while (1)
	{

		INSPIRATION_TIME_TRACE();
		EXPIRATION_TIME_TRACE();
		BREATH_COUNT();
		Nebuliser_Timing();


		if(Running_Mode == Pcsimv)
		{
			PC_SIMV_BLOWER_WAIT_TIME_TRACE();
		}
		else if(Running_Mode == Vcsimv)
		{
			VC_SIMV_BLOWER_WAIT_TIME_TRACE();
		}
		else if(Running_Mode == Psv)
		{
			PSV_APNEA_COUNTER_TRACE();
		}
		else if(Running_Mode == Cpap)
		{
			CPAP_APNEA_COUNTER_TRACE();
		}
		else if(Running_Mode == BiPap)
		{
			BIPAP_APNEA_COUNTER_TRACE();
		}
		else if(Running_Mode == Aprv)
		{
			APRV_APNEA_COUNTER_TRACE();
		}

		vTaskDelay(Two_Millisecond_Delay);
	}
}



void INSPIRATION_TIME_TRACE()
{
	if(TIME_Base_parameter._INSPIRATION_TIME > 0)
	{
		TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED  = TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED+2;
		TIME_Base_parameter._INSPIRATION_TIME          = TIME_Base_parameter._INSPIRATION_TIME -2;
		if(TIME_Base_parameter._INSPIRATION_TIME<0)
			TIME_Base_parameter._INSPIRATION_TIME=0;

		if(TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED > 100)
		{
			Find_Max_PIP();
		}
		if(TIME_Base_parameter._INSPIRATION_TIME <200)
		{
			PIP_Average_Parameter.PIP_Cumulative_Val += Pressure_sensor.Pressure_Val;
			PIP_Average_Parameter.PIP_Event_Count++;
		}

		if(TIME_Base_parameter.FLOW_ACHIEVED_TIMER > 0)
		{
			TIME_Base_parameter.FLOW_ACHIEVED_TIMER = (TIME_Base_parameter.FLOW_ACHIEVED_TIMER -2);
			if(TIME_Base_parameter.FLOW_ACHIEVED_TIMER < 0)
				TIME_Base_parameter.FLOW_ACHIEVED_TIMER = 0;

			if(Flow_Sensor_cal._Flow_Val > Volume_control_Task.Flow_Max_Achieved)
		    {
			  Volume_control_Task.Flow_Max_Achieved = Flow_Sensor_cal._Flow_Val;
		    }
		}
		if(TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED < 200)
		{
			Current_Pressure   = Pressure_sensor.Pressure_Val;
		}


	}
}


void EXPIRATION_TIME_TRACE()
{
	if(TIME_Base_parameter._EXPIRATION_TIME > 0)
	{
		TIME_Base_parameter.EXPIRATION_TIME_ACHEIVED   = TIME_Base_parameter.EXPIRATION_TIME_ACHEIVED+2;
		TIME_Base_parameter._EXPIRATION_TIME           = TIME_Base_parameter._EXPIRATION_TIME -2;
		if(TIME_Base_parameter._EXPIRATION_TIME<0)
			TIME_Base_parameter._EXPIRATION_TIME=0;
		if(TIME_Base_parameter._EXPIRATION_TIME <100)
		{
			PEEP_Maintain_Parameter.PEEP_Cumulative_Val += Pressure_sensor.Pressure_Val;
			PEEP_Maintain_Parameter.PEEP_Event_Count++;
			Find_Maximum_PEEP();
		}

		if(TIME_Base_parameter.EXPIRATION_TIME_ACHEIVED > 500)
		{
			PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger += Pressure_sensor.Pressure_Val;
			PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger++;
		}

		if(PEEP_Maintain_Parameter.CHECK_TRIGGER > 0)
		{
			PEEP_Maintain_Parameter.CHECK_TRIGGER = (PEEP_Maintain_Parameter.CHECK_TRIGGER-2);
			if(PEEP_Maintain_Parameter.CHECK_TRIGGER<0)
				PEEP_Maintain_Parameter.CHECK_TRIGGER=0;
		}
	}
}




void PC_SIMV_BLOWER_WAIT_TIME_TRACE()
{
	if(PCsimv.Blower_Wait_Time_Milli_Second > 0)
	{
		PCsimv.Blower_Wait_Time_Milli_Second = (PCsimv.Blower_Wait_Time_Milli_Second -2);
		if(PCsimv.Blower_Wait_Time_Milli_Second < 0)
			PCsimv.Blower_Wait_Time_Milli_Second=0;
		if(PCsimv.Blower_Wait_Time_Milli_Second < 100)
			PCsimv.Trigger_Check_Wait_Time = 500;
	}

	if(PCsimv.Trigger_Check_Wait_Time > 0 )
	{
		PCsimv.Trigger_Check_Wait_Time = (PCsimv.Trigger_Check_Wait_Time -2);
		 if(PCsimv.Trigger_Check_Wait_Time < 0)
			 PCsimv.Trigger_Check_Wait_Time =0;
	}
}


void VC_SIMV_BLOWER_WAIT_TIME_TRACE()
{
	if(VCsimv.Blower_Wait_Time_Milli_Second > 0)
	{
		VCsimv.Blower_Wait_Time_Milli_Second = (VCsimv.Blower_Wait_Time_Milli_Second -2);
		if(VCsimv.Blower_Wait_Time_Milli_Second < 0)
			VCsimv.Blower_Wait_Time_Milli_Second=0;
		if(VCsimv.Blower_Wait_Time_Milli_Second < 100)
			VCsimv.Trigger_Check_Wait_Time = 500;
	}

	if(VCsimv.Trigger_Check_Wait_Time > 0 )
	{
		VCsimv.Trigger_Check_Wait_Time = (VCsimv.Trigger_Check_Wait_Time -2);
		 if(VCsimv.Trigger_Check_Wait_Time < 0)
			 VCsimv.Trigger_Check_Wait_Time =0;
	}
}



void PSV_APNEA_COUNTER_TRACE()
{
	if(PSV.APNEA_COUNTER > 0)
	{
		PSV.APNEA_COUNTER = (PSV.APNEA_COUNTER -2);
		if(PSV.APNEA_COUNTER < 0)
		{
			PSV.APNEA_COUNTER = 0;
		}
	}

	if(PSV_Control.PSV_PEEP_Settle_Time > 0)
	{
		PSV_Control.PSV_PEEP_Settle_Time = (PSV_Control.PSV_PEEP_Settle_Time -2);
		if(PSV_Control.PSV_PEEP_Settle_Time < 0)
		{
			PSV_Control.PSV_PEEP_Settle_Time = 0;
		}
	}

	if(PSV_Control.Blower_Wait_Time_Milli_Second > 0)
	{
		PSV_Control.Blower_Wait_Time_Milli_Second = (PSV_Control.Blower_Wait_Time_Milli_Second -2);
		if(PSV_Control.Blower_Wait_Time_Milli_Second < 0)
			PSV_Control.Blower_Wait_Time_Milli_Second=0;
		if(PSV_Control.Blower_Wait_Time_Milli_Second < 100)
			PSV_Control.Trigger_Check_Wait_Time = 1000;
	}

	if(PSV_Control.Trigger_Check_Wait_Time > 0 )
	{
		PSV_Control.Trigger_Check_Wait_Time = (PSV_Control.Trigger_Check_Wait_Time -2);
		 if(PSV_Control.Trigger_Check_Wait_Time < 0)
			 PSV_Control.Trigger_Check_Wait_Time =0;
	}

	if(PSV_Control._INSPIRATION_TIME  > 0)
	{
		PSV_Control._INSPIRATION_TIME = (PSV_Control._INSPIRATION_TIME -2);
		if(PSV_Control._INSPIRATION_TIME < 0)
			PSV_Control._INSPIRATION_TIME =0;
	}

}




void BIPAP_APNEA_COUNTER_TRACE()
{
	if(BIPAP.APNEA_COUNTER > 0)
	{
		BIPAP.APNEA_COUNTER = (BIPAP.APNEA_COUNTER -2);
		if(BIPAP.APNEA_COUNTER < 0)
		{
			BIPAP.APNEA_COUNTER = 0;
		}
	}
	if(BIPAP_Control.BIPAP_EPAP_Settle_Time > 0)
	{
		BIPAP_Control.BIPAP_EPAP_Settle_Time = (BIPAP_Control.BIPAP_EPAP_Settle_Time -2);
		if(BIPAP_Control.BIPAP_EPAP_Settle_Time < 0)
		{
			BIPAP_Control.BIPAP_EPAP_Settle_Time = 0;
		}
	}

	if(BIPAP_Control.Blower_Wait_Time_Milli_Second > 0)
	{
		BIPAP_Control.Blower_Wait_Time_Milli_Second = (BIPAP_Control.Blower_Wait_Time_Milli_Second -2);
		if(BIPAP_Control.Blower_Wait_Time_Milli_Second < 0)
			BIPAP_Control.Blower_Wait_Time_Milli_Second=0;
		if(BIPAP_Control.Blower_Wait_Time_Milli_Second < 100)
			BIPAP_Control.Trigger_Check_Wait_Time = 1000;
	}

	if(BIPAP_Control.Trigger_Check_Wait_Time > 0 )
	{
		BIPAP_Control.Trigger_Check_Wait_Time = (BIPAP_Control.Trigger_Check_Wait_Time -2);
		 if(BIPAP_Control.Trigger_Check_Wait_Time < 0)
			 BIPAP_Control.Trigger_Check_Wait_Time =0;
	}

	if(BIPAP_Control._INSPIRATION_TIME  > 0)
	{
		BIPAP_Control._INSPIRATION_TIME = (BIPAP_Control._INSPIRATION_TIME -2);
		if(BIPAP_Control._INSPIRATION_TIME < 0)
			BIPAP_Control._INSPIRATION_TIME =0;
	}
}



void CPAP_APNEA_COUNTER_TRACE()
{
	if(CPAP.APNEA_COUNTER > 0)
	{
		CPAP.APNEA_COUNTER = (CPAP.APNEA_COUNTER -2);
		if(CPAP.APNEA_COUNTER < 0)
		{
			CPAP.APNEA_COUNTER = 0;
		}
	}
	if(CPAP_Control.CPAP_Settle_Time > 0)
	{

		CPAP_Control.CPAP_Settle_Time = (CPAP_Control.CPAP_Settle_Time -2);
		if(CPAP_Control.CPAP_Settle_Time < 0)
		{
			Alert_Status                  = NO_ALERT;
			LED_Alert();
			CPAP_Control.CPAP_Settle_Time = 0;
		}
	}

	if(CPAP_Control.Blower_Wait_Time_Milli_Second > 0)
	{
		CPAP_Control.Blower_Wait_Time_Milli_Second = (CPAP_Control.Blower_Wait_Time_Milli_Second -2);
		if(CPAP_Control.Blower_Wait_Time_Milli_Second < 0)
			CPAP_Control.Blower_Wait_Time_Milli_Second=0;
		if(CPAP_Control.Blower_Wait_Time_Milli_Second < 100)
		{
			CPAP_Control.Trigger_Check_Wait_Time = 1000;
			Patient_Trigger                      = NO_PATIENT_TRIGGER;
		}
	}

	if(CPAP_Control.Trigger_Check_Wait_Time > 0 )
	{
		CPAP_Control.Trigger_Check_Wait_Time = (CPAP_Control.Trigger_Check_Wait_Time -2);
		 if(CPAP_Control.Trigger_Check_Wait_Time < 0)
			 CPAP_Control.Trigger_Check_Wait_Time =0;
	}
}


void APRV_APNEA_COUNTER_TRACE()
{
	if( APRV.T_HIGH_VAL_COUNTER >0)
	{
		APRV.T_HIGH_VAL_COUNTER = (APRV.T_HIGH_VAL_COUNTER- 2);
		if( APRV.T_HIGH_VAL_COUNTER < 100)
		{
			APRV_PIP_average.PIP_Cumulative_Val += Pressure_sensor.Pressure_Val;
			APRV_PIP_average.PIP_Event_Count++;
		}
	}

	if( APRV.T_LOW_VAL_COUNTER >0)
	{
		APRV.T_LOW_VAL_COUNTER = (APRV.T_LOW_VAL_COUNTER- 2);
		if( APRV.T_LOW_VAL_COUNTER < 100)
		{
			APRV_PEEP_average.PEEP_Cumulative_Val += Pressure_sensor.Pressure_Val;
			APRV_PEEP_average.PEEP_Event_Count++;
		}
	}
}


void BREATH_COUNT()
{
	if(One_Minite_Or_60000_Ms>0)
	{
		One_Minite_Or_60000_Ms=One_Minite_Or_60000_Ms-2;

		if(One_Minite_Or_60000_Ms==0)
		{
			Breath_In_Minite       = Breath;
			One_Minite_Or_60000_Ms = 60000;
			Breath=0;
		}

			if(  ( ! ( BREATH_STATE >> 8) & 1)  )
			{
				if(Breath_Count == Count_The_Breath)
				{
					Breath++;
					Breath_Count = Leave_Count_The_Breath;
				}
			}
	}
}


void Nebuliser_Timing()
{
	if(Nebuliser.NEBULISER_TIMER>0)
	{
		Nebuliser.NEBULISER_TIMER = (Nebuliser.NEBULISER_TIMER - 2);

		if(Nebuliser.NEBULISER_TIMER < 0)
		{
			Nebuliser.NEBULISER_TIMER = 0;
		}
	}
}
