/*
 * Vc_Cmv.c
 *
 *  Created on: Feb 1, 2023
 *      Author: asus
 */

#include "Vc_Cmv.h"


static void Inspiration_Time_Blower_Control_VC_CMV(uint16_t);
static void Expiration_Time_Blower_Control_VC_CMV(uint16_t,float,uint16_t);
static void CHECK_Flow_Increment();
static void Check_Flow_Graph(uint8_t);


void Vc_Cmv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet)
{
	uint32_t One_Breathe_time;
	VC_CMV.VOLUME_Val             = Receive_Graph_Packet->VTI_VS;
	VC_CMV.PEEP_Val               = Receive_Graph_Packet->PEEP_CPAP_Plow;
	VC_CMV.FIO2_Val               = Receive_Graph_Packet->FiO2;

	VC_CMV.RESPIRATORY_RATE_Val   = Receive_Graph_Packet->RR;
	One_Breathe_time              = (One_Minite_In_MS / VC_CMV.RESPIRATORY_RATE_Val);
	VC_CMV.INSPIRATION_TIME       = ( ( Receive_Graph_Packet->T_high) * 100 );
	VC_CMV.EXPIRATION_TIME        = (One_Breathe_time - VC_CMV.INSPIRATION_TIME);


	VC_CMV.FLOW_RATE_Val          = Receive_Graph_Packet->Flow_Limit_FiO2_Flow_Limit;

    Mode_Initial_Flag_Set         = OPEN;
	vTaskResume(Mode_initial_Flag_Set_Handler);
}



void Vc_Cmv_Task (void *argument)
{

	while(1)
	{
		switch (Run_Current_Breathe_State)
		{
			case Run_Inspiration_Cycle:
				Inspiration_Time_Blower_Control_VC_CMV(Volume_Mode_Blower_control.BLOWER_DAC_VAL);
			break;
			case Run_Expiration_Cycle:
				Volume_Mode_Blower_control.BLOWER_DAC_VAL=0;
				Expiration_Time_Blower_Control_VC_CMV(Volume_Mode_Blower_control.BLOWER_DAC_VAL,
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


static void Inspiration_Time_Blower_Control_VC_CMV(uint16_t Blower_Signal)
{
	Volume_Mode_Blower_control.Blower_Signal_Voltage = ( (Blower_Signal*3.3) /4095);
	Blower_Signal(Blower_Signal);
}


static void Expiration_Time_Blower_Control_VC_CMV(uint16_t Blower_Signal,float PEEP,uint16_t Expiratory_Valve_Open_Time)
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
}



void VC_CMV_Inspiration_Time_Variables_Reset()
{

	Breath_Count         = Count_The_Breath;
	if( (VC_CMV.PEEP_Val == PEEP_Maintain_Parameter.PEEP_AVG_VAL ) )
	{
	   CHECK_Flow_Increment();
	}
	else
	{
		Flow_increment       = Flow_fine;
		memset(Volume_Array, 0,(100*sizeof(Volume_Array[0])) );
		Volume_Array_count = 0;
	}
	Reset_Blower_Status();
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
	if( (VC_CMV.PEEP_Val != PEEP_Maintain_Parameter.PEEP_AVG_VAL ) )
	{
		PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time     = PEEP_Control_For_Expiratory_Valve_Open_Time_In_Volume_Mode(PEEP_Maintain_Parameter.PEEP_AVG_VAL,
																												           VC_CMV.RESPIRATORY_RATE_Val,
													                                                                       VC_CMV.INSPIRATION_TIME,
													                                                                       VC_CMV.PEEP_Val,
																												           PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time);

	}
    VOLUME_STOP_POINT(Common_Mode_Parameter._VOLUME_Val,Volume_control_Task.Achieved_Volume);
    Volume_Mode_Blower_control.INITIAL_BLOWER_DAC_VAL          =  TUNE_INITIAL_DAC_VAL(Volume_Mode_Blower_control.PREVIOUS_INITIAL_BLOWER_DAC_VAL,
    		                                                                           Common_Mode_Parameter._FLOW_RATE,
																					   Volume_control_Task.Flow_Max_Achieved);

    Volume_Mode_Blower_control.PREVIOUS_INITIAL_BLOWER_DAC_VAL = Volume_Mode_Blower_control.INITIAL_BLOWER_DAC_VAL;

    LOW_COMPLIANCE_DAC                                         = LOW_Compliance_DAC_value(Common_Mode_Parameter._FLOW_RATE);

	BREATH_STATE                                               = INSPIRATION_CYCLE;
	Peep_Status                   				               = PEEP_NOT_ACHEIVED;
	Expiratory_Valve_Lock_Delay   			                   = OPENED;
	PIP_Average_Parameter.Maximum_PIP_Acheived                 = RESET;
	Maximum_Flow_Achieved                                      = FLOW_NOT_ACHIEVED;
	Volume_control_Task.Flow_Max_Achieved                      = RESET;
	Volume_Achieved_Status                                     = VOLUME_NOT_ACHIEVED;
	TIME_Base_parameter._INSPIRATION_TIME                      = VC_CMV.INSPIRATION_TIME;

	Led_Delay                                                  = (Total_Volume_Achieved_Time  /46);
	LED_ITER                                                   = 1;
	Volume_Achieved_Time                                       = 0;
	Total_Volume_Achieved_Time                                 = 0;

	TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED              = RESET;
	vol.Volume                                                 = RESET;
	vol.Volume_max                                             = RESET;
	Next_Half_Breath_Cycle                                     = Generate_Expiration_Cycle;
	Run_Current_Breathe_State                                  = Run_Inspiration_Cycle;
	vTaskDelay(VC_CMV.INSPIRATION_TIME);
}


void VC_CMV_Expiration_Time_Variables_Reset()
{
	Volume_control_Task.Achieved_Volume                 = vol.Volume ;
	BREATH_STATE                                        = EXPIRATION_CYCLE;
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger = RESET;
	PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger    = RESET;
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val         = RESET;
	PEEP_Maintain_Parameter.PEEP_Event_Count            = RESET;
	Adjust_Servo_And_Calculate_O2(Common_Mode_Parameter._FIO2_Val);
	Alert_Expiration_Time_Parameter();
	TIME_Base_parameter._EXPIRATION_TIME                = VC_CMV.EXPIRATION_TIME;

	Led_Delay                                           = ( (PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time + 500)  /46);
	LED_ITER                                            = 45;

	TIME_Base_parameter.EXPIRATION_TIME_ACHEIVED        = RESET;
	Next_Half_Breath_Cycle                              = Generate_Inspiration_Cycle;
	Run_Current_Breathe_State                           = Run_Expiration_Cycle;
	vTaskDelay(VC_CMV.EXPIRATION_TIME);
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
		Check_Flow_Graph(VC_CMV.FLOW_RATE_Val);
		memset(Volume_Array, 0,(100*sizeof(Volume_Array[0])) );
		Volume_Array_count   = 0;

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



void Volume_Not_Acheieved()
{
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}


void Volume_Acheieved()
{
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}


void Reset_Blower_Status()
{
	Blower_Status       = BlOWER_ON;
	Current_Pressure    = 0;

	if(Over_Pressure_Count > 10)
	{
		Over_Pressure_Count = 0;
	}

}





