/*
 * PC-SIMV_ASSIST_OFF.c
 *
 *  Created on: Feb 14, 2023
 *      Author: asus
 */


#include "Pc_Simv_Assist_OFF.h"



static void PC_SIMV_Assist_OFF_PATIENT_TRIGGER_HAPPEN_Inspiration_Time_Variables_Reset();
static void PC_SIMV_Assist_OFF_PATIENT_TRIGGER_HAPPEN_Expiration_Time_Variables_Reset();
static void After_Trigger(uint16_t);
static void Before_Trigger(uint16_t);
static void CHECK_TRIGGER_ASSIST_OFF(uint16_t);
static void Switch_TASK_Inspiration_CYCLE();
static void Inspiration_Time_Blower_Control_PC_SIMV_ASSIST_OFF(uint16_t);
static void Expiration_Time_Blower_Control_PC_SIMV_ASSIST_OFF(uint16_t,float,uint16_t);
static void STOP_BLOWER_IF_FLOW_SENSE();
static void CHECK_PRESSURE_TRIGGER();
static void CHECK_FLOW_TRIGGER();
static void CHECK_TRIGGER_OFFSET();
static void Set_Blower_Val();
static void Trigger_Time_Monitor();

void Pc_Simv_Assist_OFF_Task(void *argument)
{
	while(1)
	{
		switch (Run_Current_Breathe_State)
		{
			case Run_Inspiration_Cycle:
				Inspiration_Time_Blower_Control_PC_SIMV_ASSIST_OFF(Pressure_Mode_blower_control.BLOWER_DAC_VAL);
			break;
			case Run_Expiration_Cycle:
				Expiration_Time_Blower_Control_PC_SIMV_ASSIST_OFF(Pressure_Mode_blower_control.BLOWER_DAC_VAL,
											   PEEP_Maintain_Parameter.PEEP_Temp,
											   PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time);
			break;
			case No_Run_State:
			break;
			default:
			break;
		}

		vTaskDelay(Two_Millisecond_Delay);
	}
}

static void Inspiration_Time_Blower_Control_PC_SIMV_ASSIST_OFF(uint16_t Blower_Signal)
{
	Pressure_Mode_blower_control.Blower_Signal_Voltage = ( (Blower_Signal*3.3) /4095);
	Blower_Signal(Blower_Signal);
}

static void Expiration_Time_Blower_Control_PC_SIMV_ASSIST_OFF(uint16_t Blower_Signal,float PEEP,uint16_t Expiratory_Valve_Open_Time)
{
	if(PCsimv.Blower_Wait_Time_Milli_Second ==0)
	{
		Blower_Signal( Blower_Signal);
	}
		if(Peep_Status==PEEP_ACHEIVED)
		{

		 }
		if(Pressure_sensor.Pressure_Val<= PEEP)
		{
			if(Expiratory_Valve_Lock_Delay==OPENED)
			{
				Expiratory_Valve_Lock_Delay=CLOSED;
				if(Trigger_Process == ONE_TIME_TRIGGER)
				{
				   vTaskDelay(Expiratory_Valve_Open_Time);
				}
				else if(Trigger_Process == COUNTINOUS_TRIGGER)
				{
				   vTaskDelay(0);
				}
			}
			Peep_Status=PEEP_ACHEIVED;
			ExpValve_CLOSE();
		}
		else if(Peep_Status==PEEP_NOT_ACHEIVED)
		{
			ExpValve_OPEN();
		}

	CHECK_TRIGGER_ASSIST_OFF(PC_SIMV.TRIG_WINDOW);
}

static void CHECK_TRIGGER_ASSIST_OFF(uint16_t Trigger_Check_Time)
{
	if(Trigger_Process == ONE_TIME_TRIGGER)
	{
		Before_Trigger(Trigger_Check_Time);
	}
	else if(Trigger_Process == COUNTINOUS_TRIGGER)
	{
		After_Trigger(Trigger_Check_Time);
	}

}



static void Before_Trigger(uint16_t Trigger_Check_Time)
{
	if ( (TIME_Base_parameter._EXPIRATION_TIME <= Trigger_Check_Time) && (TIME_Base_parameter._EXPIRATION_TIME != 0))
	{

		if (Trigger_Type == Pressure_Trigger)
		{
			CHECK_PRESSURE_TRIGGER();
		}
		else if (Trigger_Type == Flow_Trigger)
		{
			CHECK_FLOW_TRIGGER();
		}

	}

	else
	{
		CHECK_TRIGGER_OFFSET();
	}
}



static void After_Trigger(uint16_t Trigger_Check_Time)
{
	if ( (TIME_Base_parameter._EXPIRATION_TIME <= Trigger_Check_Time) && (TIME_Base_parameter._EXPIRATION_TIME != 0))
	{
		if( (Flow_Sensor_cal._Flow_Val > 0) && (Peep_Status==PEEP_ACHEIVED) && (PEEP_Maintain_Parameter.CHECK_TRIGGER==0) && (PCsimv.Trigger_Check_Wait_Time == 0))
		{
			STOP_BLOWER_IF_FLOW_SENSE();

				if (Trigger_Type == Pressure_Trigger)
				{
					CHECK_PRESSURE_TRIGGER();
				}
				else if (Trigger_Type == Flow_Trigger)
				{
					CHECK_FLOW_TRIGGER();
				}
		 }
	}
	else
	{
		CHECK_TRIGGER_OFFSET();
	}
}
void PC_SIMV_Assist_OFF_Inspiration_Time_Variables_Reset()
{
	if(Patient_Trigger == PATIENT_TRIGGER_HAPPEN)
	{
		Trigger_Process = COUNTINOUS_TRIGGER;
		Pressure_Mode_blower_control.BLOWER_DAC_VAL = RESET;
		PC_SIMV_Assist_OFF_PATIENT_TRIGGER_HAPPEN_Inspiration_Time_Variables_Reset();
	}
	else
	{
		Trigger_Process = ONE_TIME_TRIGGER;
		PC_SIMV_Assist_ON_Inspiration_Time_Variables_Reset();
	}
}



void PC_SIMV_Assist_OFF_Expiration_Time_Variables_Reset()
{
	if(Patient_Trigger == PATIENT_TRIGGER_HAPPEN)
	{
		PC_SIMV_Assist_OFF_PATIENT_TRIGGER_HAPPEN_Expiration_Time_Variables_Reset();
	}
	else
	{
		Pressure_Mode_blower_control.BLOWER_DAC_VAL=RESET;
		PC_SIMV_Assist_ON_Expiration_Time_Variables_Reset();
	}
}


static void Switch_TASK_Inspiration_CYCLE()
{
	vTaskDelete(One_Time_Handler);
	TIME_Base_parameter._INSPIRATION_TIME = 0 ;
	TIME_Base_parameter._EXPIRATION_TIME  = 0 ;
	Next_Half_Breath_Cycle                = Generate_Inspiration_Cycle;
	xTaskCreate(One_Time_Task,"One-Time-Task", 128, NULL,3, &One_Time_Handler);
}

static void PC_SIMV_Assist_OFF_PATIENT_TRIGGER_HAPPEN_Inspiration_Time_Variables_Reset()
{
	    Breath_Count         = Count_The_Breath;
		PIP_Not_Acheieved();
		ExpValve_CLOSE();
		PIP_AVERAGE();
		PEEP_AVERAGE();
		Set_Blower_Val();
		O2_DAC_Control(Common_Mode_Parameter._FIO2_Val);
		Check_Alert();
		Alert_Inspiration_Time_Parameter();
		LED_Alert();
		Alert_Status = NO_ALERT;
		Sampled_Parameter_Inspiration_time();
		PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time  = PEEP_Control_For_Expiratory_Valve_Open_Time_In_Pressure_Mode(PEEP_Maintain_Parameter.PEEP_AVG_VAL,
																															 PC_SIMV.RESPIRATORY_RATE_Val,
																															 PC_SIMV.INSPIRATION_TIME,
																															 PC_SIMV.PEEP_Val,
																															 PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time);

		if(Patient_Trigger  == PATIENT_TRIGGER_HAPPEN)
		{
			BREATH_STATE                                      = PATIENT_TRIGGER_INSPIRATION;
			LED_STATUS                                        = EXPIRATION_LED;
		}
		else
		{
			BREATH_STATE                                       = INSPIRATION_CYCLE;
		}

		Peep_Status                   				         = PEEP_NOT_ACHEIVED;
		Expiratory_Valve_Lock_Delay   			             = OPENED;
		PIP_Average_Parameter.Maximum_PIP_Acheived           = RESET;
		TIME_Base_parameter._INSPIRATION_TIME                = PC_SIMV.INSPIRATION_TIME;

		Led_Delay                                            = (PC_SIMV.RISE_TIME_MS_Val  /46);
		LED_ITER                                             = 1;

		TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED        = RESET;
		vol.Volume                                           = RESET;
		vol.Volume_max                                       = RESET;
		Next_Half_Breath_Cycle                               = Generate_Expiration_Cycle;
		Run_Current_Breathe_State                            = Run_Inspiration_Cycle;
		vTaskDelay(PC_SIMV.INSPIRATION_TIME);
}

static void PC_SIMV_Assist_OFF_PATIENT_TRIGGER_HAPPEN_Expiration_Time_Variables_Reset()
{
		if(Patient_Trigger  == PATIENT_TRIGGER_HAPPEN)
		{
			BREATH_STATE                                      = PATIENT_TRIGGER_EXPIRATION;
			LED_STATUS                                        = Idle_LED;
		}
		else
		{
			BREATH_STATE                                       = EXPIRATION_CYCLE;
		}
		PEEP_Maintain_Parameter.CHECK_TRIGGER               = 1500;
		PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger = RESET;
		PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger    = RESET;
		PEEP_Maintain_Parameter.PEEP_Cumulative_Val         = RESET;
		PEEP_Maintain_Parameter.PEEP_Event_Count            = RESET;
		Patient_Trigger                                     = NO_PATIENT_TRIGGER;
		Trigger_Time_Monitor();
		Pressure_Mode_blower_control.PEEP_BLOWER_DAC_VAL    = TUNE_PEEP_BLOWER(&Pressure_Mode_blower_control.LAST_PEEP_BLOWER_DAC_VAL,
				                                                               Common_Mode_Parameter._PEEP_Val );
		Pressure_Mode_blower_control.BLOWER_DAC_VAL         = Pressure_Mode_blower_control.PEEP_BLOWER_DAC_VAL;
		Adjust_Servo_And_Calculate_O2(Common_Mode_Parameter._FIO2_Val);
		Alert_Expiration_Time_Parameter();
		TIME_Base_parameter._EXPIRATION_TIME                = PC_SIMV.EXPIRATION_TIME;


		Led_Delay                                           = ( (PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time + 500)  /46);
		LED_ITER                                            = 45;

		TIME_Base_parameter.EXPIRATION_TIME_ACHEIVED        = RESET;
		Next_Half_Breath_Cycle                              = Generate_Inspiration_Cycle;
		Run_Current_Breathe_State                           = Run_Expiration_Cycle;
		vTaskDelay(PC_SIMV.EXPIRATION_TIME);
}




uint16_t TUNE_PEEP_BLOWER(uint16_t *Last_Blower_DAC_Val,uint8_t Set_PEEP)
{
	if(PEEP_Maintain_Parameter.PEEP_AVG_VAL  > Set_PEEP)
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-10);
	}
	else if(PEEP_Maintain_Parameter.PEEP_AVG_VAL  < Set_PEEP)
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+10);
	}
	else
	{
		*Last_Blower_DAC_Val = *Last_Blower_DAC_Val;
	}
	return *Last_Blower_DAC_Val;
}


static void CHECK_PRESSURE_TRIGGER()
{
	if (( Pressure_sensor.Pressure_Val < (Pressure_Trigger_Offset - PC_SIMV.TRIG_LMT )))
	{
		Patient_Trigger = PATIENT_TRIGGER_HAPPEN;
		Switch_TASK_Inspiration_CYCLE();
	}
}


static void CHECK_FLOW_TRIGGER()
{
	if ((FLOW_TRIGGER > (Flow_Trigger_Offset+PC_SIMV.TRIG_LMT )))
	{
		LAST_FLOW_TRIGGER = FLOW_TRIGGER;
		Patient_Trigger   = PATIENT_TRIGGER_HAPPEN;
		Switch_TASK_Inspiration_CYCLE();

	}
}

static void CHECK_TRIGGER_OFFSET()
{
	if(Flow_Sensor_cal._Flow_Val==0 || (Flow_Sensor_cal._Flow_Val>=(-8) && Flow_Sensor_cal._Flow_Val<0))
	{
		Pressure_Trigger_Offset  =  Pressure_sensor.Pressure_Val;
		Flow_Trigger_Offset      =  Flow_Sensor_cal._Flow_Val;
		FLOW_TRIGGER             =  0;

	}
}


static void STOP_BLOWER_IF_FLOW_SENSE()
{
	Blower_Signal(0);
	PCsimv.Blower_Wait_Time_Milli_Second = 500;
}


static void Set_Blower_Val()
{
	if(PEEP_Maintain_Parameter.PEEP_AVG_VAL   == PC_SIMV.PEEP_Val  )
	{
		Change_DAC_Values(PC_SIMV.PEEP_Val ,Pressure_Mode_blower_control.PEEP_BLOWER_DAC_VAL);
	}
}



static void Trigger_Time_Monitor()
{
	PC_SIMV.CALC_TRIG_VAL 	                 =  (PC_SIMV.EXPIRATION_TIME   - PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time );
	if(PC_SIMV.CALC_TRIG_VAL <0)
	{
		PC_SIMV.CALC_TRIG_VAL = (PC_SIMV.CALC_TRIG_VAL * (-1));
	}
	PC_SIMV.TOLERANCE_EWAVE =  PC_SIMV.TRIG_TIME;
	if(PC_SIMV.CALC_TRIG_VAL < PC_SIMV.TRIG_TIME )
	{
		PC_SIMV.TOLERANCE_EWAVE =  (PC_SIMV.TRIG_TIME - 200) ;
	}

	PC_SIMV.TRIG_WINDOW		                 = PC_SIMV.TOLERANCE_EWAVE;
}
