/*
 * Vc-Simv_Assist_ON.c
 *
 *  Created on: Feb 17, 2023
 *      Author: asus
 */


#include "Vc_Simv_Assist_ON.h"


static void Inspiration_Time_Blower_Control_VC_SIMV_ASSIST_ON(uint16_t);
static void Expiration_Time_Blower_Control_VC_SIMV_ASSIST_ON(uint16_t,float,uint16_t);
static void CHECK_TRIGGER_ASSIST_ON(uint16_t);
static void Switch_TASK_Inspiration_CYCLE();
static void CHECK_PRESSURE_TRIGGER();
static void CHECK_FLOW_TRIGGER();
static void CHECK_TRIGGER_OFFSET();
static void CHECK_Flow_Increment();
static void Check_Flow_Graph(uint8_t);
static void Trigger_Time_Monitor();

extern uint16_t Expiratory_Valve_Open_Time;

void Vc_Simv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet)
{
	uint32_t One_Breathe_time;
	VC_SIMV.VOLUME_Val             = Receive_Graph_Packet->VTI_VS;
	VC_SIMV.PEEP_Val               = Receive_Graph_Packet->PEEP_CPAP_Plow;
	VC_SIMV.FIO2_Val               = Receive_Graph_Packet->FiO2;

	VC_SIMV.RESPIRATORY_RATE_Val   = Receive_Graph_Packet->RR;
	One_Breathe_time               = (One_Minite_In_MS / VC_SIMV.RESPIRATORY_RATE_Val);
	VC_SIMV.INSPIRATION_TIME       = ( ( Receive_Graph_Packet->T_high) * 100 );
	VC_SIMV.EXPIRATION_TIME        = (One_Breathe_time - VC_SIMV.INSPIRATION_TIME);

	VC_SIMV.FLOW_RATE_Val          = Receive_Graph_Packet->Flow_Limit_FiO2_Flow_Limit;


	VC_SIMV.Assist_Control    	   = (0x80 & (Receive_Graph_Packet->Control_Byte))>>7;
	VC_SIMV.TRIG_TYPE     	       = (0x40 & (Receive_Graph_Packet->Control_Byte))>>6;
	VC_SIMV.TRIG_LMT     	       = ( (Receive_Graph_Packet->Trigger_Limit) / 10);
	VC_SIMV.TRIG_TIME     	       = ( (Receive_Graph_Packet->Trigger_Time) * 100);

	VC_SIMV.CALC_TRIG_VAL 	                 =  (VC_SIMV.EXPIRATION_TIME   - Expiratory_Valve_Open_Time );
	if(VC_SIMV.CALC_TRIG_VAL <0)
	{
		VC_SIMV.CALC_TRIG_VAL = (VC_SIMV.CALC_TRIG_VAL * (-1));
	}

	VC_SIMV.TOLERANCE_EWAVE =  VC_SIMV.TRIG_TIME;
	if(VC_SIMV.CALC_TRIG_VAL < VC_SIMV.TRIG_TIME )
	{
		VC_SIMV.TOLERANCE_EWAVE =  (VC_SIMV.TRIG_TIME - 200) ;
	}
	VC_SIMV.TRIG_WINDOW		                 = VC_SIMV.TOLERANCE_EWAVE;

	if(VC_SIMV.Assist_Control == 1)
		Assist_Control=ASSIST_ON;
	else if(VC_SIMV.Assist_Control == 0)
		Assist_Control=ASSIST_OFF;
	if(VC_SIMV.TRIG_TYPE == 0)
		Trigger_Type = Pressure_Trigger;
	else if(VC_SIMV.TRIG_TYPE == 1)
		Trigger_Type = Flow_Trigger;
	Mode_Initial_Flag_Set                        = OPEN;
	vTaskResume(Mode_initial_Flag_Set_Handler);
}


void Vc_Simv_Assist_ON_Task(void *argument)
{
	while(1)
	{
		switch (Run_Current_Breathe_State)
		{
			case Run_Inspiration_Cycle:
				Inspiration_Time_Blower_Control_VC_SIMV_ASSIST_ON(Volume_Mode_Blower_control.BLOWER_DAC_VAL);
			break;
			case Run_Expiration_Cycle:
				Volume_Mode_Blower_control.BLOWER_DAC_VAL=0;
				Expiration_Time_Blower_Control_VC_SIMV_ASSIST_ON(Volume_Mode_Blower_control.BLOWER_DAC_VAL,
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




void VC_SIMV_Inspiration_Time_Variables_Reset()
{
	if(Assist_Control == ASSIST_ON)
	{
		VC_SIMV_Assist_ON_Inspiration_Time_Variables_Reset();
	}
	else if(Assist_Control == ASSIST_OFF)
	{
		VC_SIMV_Assist_OFF_Inspiration_Time_Variables_Reset();
	}
}



void VC_SIMV_Expiration_Time_Variables_Reset()
{
	if(Assist_Control == ASSIST_ON)
	{
		VC_SIMV_Assist_ON_Expiration_Time_Variables_Reset();
	}
	else if(Assist_Control == ASSIST_OFF)
	{
		VC_SIMV_Assist_OFF_Expiration_Time_Variables_Reset();
	}
}


static void Inspiration_Time_Blower_Control_VC_SIMV_ASSIST_ON(uint16_t Blower_Signal)
{
	Volume_Mode_Blower_control.Blower_Signal_Voltage = ( (Blower_Signal*3.3) /4095);
	Blower_Signal(Blower_Signal);
}




static void Expiration_Time_Blower_Control_VC_SIMV_ASSIST_ON(uint16_t Blower_Signal,float PEEP,uint16_t Expiratory_Valve_Open_Time)
{
	Blower_Signal( Blower_Signal);
		if(Peep_Status==PEEP_ACHEIVED)
		{
				if(Pressure_sensor.Pressure_Val < PEEP)
				{
					Blower_Signal( Blower_Signal);
				}
				else
				{
					Blower_Signal( Blower_Signal);
				}
		 }
		if(Pressure_sensor.Pressure_Val<= PEEP)
		{
			if(Expiratory_Valve_Lock_Delay==OPENED)
			{
				Expiratory_Valve_Lock_Delay=CLOSED;
				vTaskDelay(Expiratory_Valve_Open_Time);
			}
			Peep_Status=PEEP_ACHEIVED;
			ExpValve_CLOSE();
		}
		else if(Peep_Status==PEEP_NOT_ACHEIVED)
		{
			ExpValve_OPEN();
		}

		CHECK_TRIGGER_ASSIST_ON(VC_SIMV.TRIG_WINDOW);
}


static void CHECK_TRIGGER_ASSIST_ON(uint16_t Trigger_Check_Time)
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


static void Switch_TASK_Inspiration_CYCLE()
{
	vTaskDelete(One_Time_Handler);
	TIME_Base_parameter._INSPIRATION_TIME = 0 ;
	TIME_Base_parameter._EXPIRATION_TIME  = 0 ;
	Next_Half_Breath_Cycle                = Generate_Inspiration_Cycle;
	xTaskCreate(One_Time_Task,"One-Time-Task", 128, NULL,3, &One_Time_Handler);
}


void VC_SIMV_Assist_ON_Inspiration_Time_Variables_Reset()
{
	Reset_Blower_Status();
	Breath_Count         = Count_The_Breath;
	if(VC_SIMV.PEEP_Val == PEEP_Maintain_Parameter.PEEP_AVG_VAL )
	{
	   CHECK_Flow_Increment();
	}
	else
	{
		Flow_increment       = Flow_fine;
		memset(Volume_Array, 0,(100*sizeof(Volume_Array[0])) );
		Volume_Array_count = 0;
	}
	Volume_Not_Acheieved();
	ExpValve_CLOSE();
	PIP_AVERAGE();
	PEEP_AVERAGE();
	O2_DAC_Control(Common_Mode_Parameter._FIO2_Val);
	Check_Alert();
	Alert_Inspiration_Time_Parameter();
	LED_Alert();
	Alert_Status = NO_ALERT;
	Sampled_Parameter_Inspiration_time();
	if(VC_SIMV.PEEP_Val != PEEP_Maintain_Parameter.PEEP_AVG_VAL )
	{
		PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time      = PEEP_Control_For_Expiratory_Valve_Open_Time_In_Volume_Mode(PEEP_Maintain_Parameter.PEEP_AVG_VAL,
																												           VC_SIMV.RESPIRATORY_RATE_Val,
													                                                                       VC_SIMV.INSPIRATION_TIME,
													                                                                       VC_SIMV.PEEP_Val,
																												           PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time);

	}
    VOLUME_STOP_POINT(Common_Mode_Parameter._VOLUME_Val,Volume_control_Task.Achieved_Volume);
    Volume_Mode_Blower_control.INITIAL_BLOWER_DAC_VAL          =  TUNE_INITIAL_DAC_VAL(Volume_Mode_Blower_control.PREVIOUS_INITIAL_BLOWER_DAC_VAL,
    		                                                                           Common_Mode_Parameter._FLOW_RATE,
																					   Volume_control_Task.Flow_Max_Achieved);
    Volume_Mode_Blower_control.PREVIOUS_INITIAL_BLOWER_DAC_VAL = Volume_Mode_Blower_control.INITIAL_BLOWER_DAC_VAL;


	if(Patient_Trigger  == PATIENT_TRIGGER_HAPPEN)
	{
		BREATH_STATE                                      = PATIENT_TRIGGER_INSPIRATION;
		LED_STATUS                                        = EXPIRATION_LED;
	}
	else
	{
		BREATH_STATE                                       = INSPIRATION_CYCLE;
	}

	Peep_Status                   				               = PEEP_NOT_ACHEIVED;
	Expiratory_Valve_Lock_Delay   			                   = OPENED;
	PIP_Average_Parameter.Maximum_PIP_Acheived                 = RESET;
	Maximum_Flow_Achieved                                      = FLOW_NOT_ACHIEVED;
	Volume_control_Task.Flow_Max_Achieved                      = RESET;
	Volume_Achieved_Status                                     = VOLUME_NOT_ACHIEVED;
	TIME_Base_parameter._INSPIRATION_TIME                      = VC_SIMV.INSPIRATION_TIME;

	Led_Delay                                                  = (Total_Volume_Achieved_Time  /46);
	LED_ITER                                                   = 1;
	Volume_Achieved_Time                                       = 0;
	Total_Volume_Achieved_Time                                 = 0;

	TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED              = RESET;
	vol.Volume                                                 = RESET;
	vol.Volume_max                                             = RESET;
	Next_Half_Breath_Cycle                                     = Generate_Expiration_Cycle;
	Run_Current_Breathe_State                                  = Run_Inspiration_Cycle;
	vTaskDelay(VC_SIMV.INSPIRATION_TIME);
}


void VC_SIMV_Assist_ON_Expiration_Time_Variables_Reset()
{
	Volume_control_Task.Achieved_Volume                 = vol.Volume ;
	if(Patient_Trigger  == PATIENT_TRIGGER_HAPPEN)
	{
		BREATH_STATE                                      = PATIENT_TRIGGER_EXPIRATION;
		LED_STATUS                                        = Idle_LED;
	}
	else
	{
		BREATH_STATE                                       = EXPIRATION_CYCLE;
	}
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger = RESET;
	PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger    = RESET;
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val         = RESET;
	PEEP_Maintain_Parameter.PEEP_Event_Count            = RESET;

	Patient_Trigger                                     = NO_PATIENT_TRIGGER;
	Trigger_Time_Monitor();

	Adjust_Servo_And_Calculate_O2(Common_Mode_Parameter._FIO2_Val);
	Alert_Expiration_Time_Parameter();
	TIME_Base_parameter._EXPIRATION_TIME                = VC_SIMV.EXPIRATION_TIME;

	Led_Delay                                           = ( (PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time + 500)  /46);
	LED_ITER                                            = 45;

	TIME_Base_parameter.EXPIRATION_TIME_ACHEIVED        = RESET;
	Next_Half_Breath_Cycle                              = Generate_Inspiration_Cycle;
	Run_Current_Breathe_State                           = Run_Expiration_Cycle;
	vTaskDelay(VC_SIMV.EXPIRATION_TIME);
}



static void CHECK_PRESSURE_TRIGGER()
{
	if (( Pressure_sensor.Pressure_Val < (Pressure_Trigger_Offset - VC_SIMV.TRIG_LMT )))
	{
		Patient_Trigger = PATIENT_TRIGGER_HAPPEN;
		Switch_TASK_Inspiration_CYCLE();
	}
}


static void CHECK_FLOW_TRIGGER()
{
	if ((FLOW_TRIGGER > (Flow_Trigger_Offset+VC_SIMV.TRIG_LMT )))
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
		FLOW_TRIGGER             =  RESET;
	}
}



static void CHECK_Flow_Increment()
{
	   if( Flow_increment == Low_Flow_Problem )
	   {
		   INCREMENT_DAC_VAL_PER_ITERATION = (INCREMENT_DAC_VAL_PER_ITERATION + 1);
		       if(INCREMENT_DAC_VAL_PER_ITERATION >10)
		    	   INCREMENT_DAC_VAL_PER_ITERATION=10;
	   }
	   else if(Flow_increment == High_Flow_Problem)
	   {
		   INCREMENT_DAC_VAL_PER_ITERATION = (INCREMENT_DAC_VAL_PER_ITERATION - 1);
		       if(INCREMENT_DAC_VAL_PER_ITERATION <3)
		    	   INCREMENT_DAC_VAL_PER_ITERATION=3;
	   }
	    Set_Flow_value       = Not_Detect;
		Flow_increment       = Flow_fine;
		Check_Flow_Graph(VC_SIMV.FLOW_RATE_Val);
		memset(Volume_Array, 0,(100*sizeof(Volume_Array[0])) );
		Volume_Array_count = 0;
}



static void Check_Flow_Graph(uint8_t flow_rate)
{
	for(int i=0;i<100;i++)
	{
		if(Volume_Array[i] == flow_rate)
		{
			Set_Flow_value = Detect;
		}
		if(Set_Flow_value == Detect)
		{
			if( (Volume_Array[i] < (flow_rate-9))  && (Volume_Array[i] !=0) )
			{
				Flow_increment = Low_Flow_Problem;
			}
			else if( (Volume_Array[i] > (flow_rate+8)))
			{
				Flow_increment = High_Flow_Problem;
			}
		}
	}
}



static void Trigger_Time_Monitor()
{
	VC_SIMV.CALC_TRIG_VAL 	                 =  (VC_SIMV.EXPIRATION_TIME   - PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time );
	if(VC_SIMV.CALC_TRIG_VAL <0)
	{
		VC_SIMV.CALC_TRIG_VAL = (VC_SIMV.CALC_TRIG_VAL * (-1));
	}
	VC_SIMV.TOLERANCE_EWAVE =  VC_SIMV.TRIG_TIME;
	if(VC_SIMV.CALC_TRIG_VAL < VC_SIMV.TRIG_TIME )
	{
		VC_SIMV.TOLERANCE_EWAVE =  (VC_SIMV.TRIG_TIME - 200) ;
	}

	VC_SIMV.TRIG_WINDOW		                 = VC_SIMV.TOLERANCE_EWAVE;
}
