/*
 * PSV.c
 *
 *  Created on: Feb 18, 2023
 *      Author: asus
 */


#include "PSV.h"


static void PSV_CONTROL();
static void STOP_BLOWER_IF_FLOW_SENSE();
static void CHECK_PRESSURE_TRIGGER();
static void CHECK_FLOW_TRIGGER();
static void CHECK_TRIGGER_OFFSET();
static void RUN_PSV_PEEP_MAINTAIN_BLOWER();
static void PSV_PEEP_AVERAGE();
static uint16_t PSV_TUNE_PEEP_BLOWER(uint16_t *,uint8_t);
static void PSV_PIP_AVERAGE();
static uint16_t PSV_TUNE_PIP_BLOWER(uint16_t *,uint8_t );
static void BACKUP_PC_CMV_START();
static void RESET_FLAGS();
static void INSPIRATION_PROCESS_PSV();
static void Patient_Circuit_Disconnected_Alert_PSV();


void PSV_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet)
{
	 PSV.PRESSURE_SUPPORT_Val             = Receive_Graph_Packet->PIP_PS_Phigh;
	 PSV.PEEP_Val                         = Receive_Graph_Packet->PEEP_CPAP_Plow;

	 PSV.TRIG_TYPE                        = (0x40 & (Receive_Graph_Packet->Control_Byte))>>6;
	 PSV.TRIG_LMT                         = ( (Receive_Graph_Packet->Trigger_Limit) / 10);
	 PSV.TRIG_TIME                        = ( (Receive_Graph_Packet->Trigger_Time) * 100);

	 PSV.APNEA_TIME                       = Receive_Graph_Packet->Apnea_Time;
	 PSV.APNEA_COUNTER                    = (1000*PSV.APNEA_TIME) ;
	 if(PSV.TRIG_TYPE == 0)
		Trigger_Type = Pressure_Trigger;
	else if(PSV.TRIG_TYPE == 1)
		Trigger_Type = Flow_Trigger;
	 Mode_Initial_Flag_Set                        = OPEN;

	 vTaskResume(Mode_initial_Flag_Set_Handler);
}



void PSV_Task(void *argument)
{
	while(1)
	{
		if(PSV.APNEA_COUNTER > 0)
		{
			PSV_CONTROL();
		}
		else
		{
			LED_STATUS = Idle_LED;
			RESET_FLAGS();
			 if(Current_Backup_Mode == Backup_PCCMV_Mode)
			 {
				 BACKUP_PC_CMV_START();
			 }

		}
		vTaskDelay(Two_Millisecond_Delay);
	}
}





static void INSPIRATION_PROCESS_PSV()
{
	Breath_Count         = Count_The_Breath;
	 if(Current_Backup_Mode == Backup_PCCMV_Mode)
	 {
		 PSV_Control._INSPIRATION_TIME                   = BACKUP_PC_CMV.INSPIRATION_TIME;
	 }

	PIP_control_Task.PIP_Control_Task_Delay               = 2;
	PIP_control_Task.Increase_Each_DAC_SET_CONST          = 140;
	PIP_Status                    				          = PIP_NOT_ACHEIVED;
	Pressure_Mode_blower_control.BLOWER_DAC_VAL           = DAC_VAL(PSV.PEEP_Val);;
	Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL      = Pressure_Mode_blower_control.BLOWER_DAC_VAL;
	Set_PIP_Status                                        = SET_PIP_VALUE_NOT_ACHEIVED;
	Read_Time_vs_Pressure                                 = READ_TIME_OPEN;
	PIP_control_Task.PIP_Control_Event_Occured            = RESET;
	PIP_Average_Parameter.Maximum_PIP_Acheived            = RESET;
	Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = PSV_TUNE_PIP_BLOWER(&Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL,
                                                                                  PSV.PRESSURE_SUPPORT_Val );




	vol.Volume                                            = RESET;
	vol.Volume_max                                        = RESET;
	PSV_PEEP_AVERAGE();
	LED_STATUS = INSPIRATION_lED;
	vTaskResume(PSV_PID_Handler);
	vTaskSuspend(PSV_Handler);
}


void PSV_PID_Task(void *argument)
{
	while(1)
	{
		if(PSV_Control._INSPIRATION_TIME > 0)
		{
			BREATH_STATE                                          = PATIENT_TRIGGER_INSPIRATION;
			PIP_Control(PSV.PRESSURE_SUPPORT_Val );
			Blower_Signal(Pressure_Mode_blower_control.BLOWER_DAC_VAL);
			Pressure_Mode_blower_control.Blower_Signal_Voltage = ( (Pressure_Mode_blower_control.BLOWER_DAC_VAL*3.3) /4095);
			   if(PSV_Control._INSPIRATION_TIME < 100)
			   {
				   PSV_Pip_average.PRESSURE_SUPPORT_Cumulative_Val += Pressure_sensor.Pressure_Val;
				   PSV_Pip_average.PRESSURE_SUPPORT_Event_Count++;
			   }
		}
		else if(PSV_Control._INSPIRATION_TIME == 0)
		{
			Blower_Signal(0);
			ExpValve_OPEN();
			vTaskDelay(700);
			PSV.APNEA_COUNTER                                = (1000*PSV.APNEA_TIME) ;
			PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL      = DAC_VAL(PSV.PEEP_Val );

			Patient_Trigger                                   = NO_PATIENT_TRIGGER;
		    BREATH_STATE                                      = PATIENT_TRIGGER_EXPIRATION;


			PSV_PIP_AVERAGE();
			PSV_Control.PSV_PEEP_Settle_Time                 = 2000;
			BIPAP_Peep_Average.EPAP_AVG_VAL                  = 0;
			vTaskResume(PSV_Handler);
			vTaskSuspend(PSV_PID_Handler);
		}
		vTaskDelay(Ten_Millisecond_Delay);
	}
}


static void PSV_CONTROL()
{
	    BREATH_STATE                                     = EXPIRATION_CYCLE;
	    if(Pressure_sensor.Pressure_Val >= (PSV.PEEP_Val + 5) )
	    {
	    	Blower_Signal(0);
	    	Pressure_Mode_blower_control.Blower_Signal_Voltage = 0;
	    	Pressure_Status = OVER_PRESSURE;
	    	ExpValve_OPEN();
	    	vTaskDelay(700);
	    	PSV_Control.PSV_PEEP_Settle_Time   = 2000;
	    	Pressure_Status = NORMAL_PRESSURE;
	    }
	    else
	    {
	    	LED_STATUS = EXPIRATION_LED;
				if(PSV_Control.Blower_Wait_Time_Milli_Second == 0)
				{
					RUN_PSV_PEEP_MAINTAIN_BLOWER();
				}

				if(PSV_Control.PSV_PEEP_Settle_Time == 0 )
				{
					Patient_Circuit_Disconnected_Alert_PSV();
					if( (Flow_Sensor_cal._Flow_Val > 0) && (PSV_Control.Trigger_Check_Wait_Time == 0))
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
					else
					{
						PSV_Peep_Average.PEEP_Cumulative_Val += Pressure_sensor.Pressure_Val;
						PSV_Peep_Average.PEEP_Event_Count++;
						if(PSV_Peep_Average.PEEP_Event_Count >1000)
						{
							Reset_Patient_Circuit_Diconnceted_Alert_Flags();
							PSV_PEEP_AVERAGE();
							PSV_Mode_Blower_control.LAST_PEEP_BLOWER_DAC_VAL    = PSV_TUNE_PEEP_BLOWER(&PSV_Mode_Blower_control.LAST_PEEP_BLOWER_DAC_VAL,
																										 PSV.PEEP_Val );
							PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL         = PSV_Mode_Blower_control.LAST_PEEP_BLOWER_DAC_VAL;
							PSV_Control.PSV_PEEP_Settle_Time                    = 1000;
						}
					}
				}
				else
				{
					CHECK_TRIGGER_OFFSET();
				}
	    }
}


static void CHECK_PRESSURE_TRIGGER()
{
	if (( Pressure_sensor.Pressure_Val < (Pressure_Trigger_Offset - PSV.TRIG_LMT )))
	{
		Patient_Trigger                             = PATIENT_TRIGGER_HAPPEN;
		PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL = 0;
		Blower_Signal( PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL);
		INSPIRATION_PROCESS_PSV();

	}
}

static void CHECK_FLOW_TRIGGER()
{
	if ((FLOW_TRIGGER > (Flow_Trigger_Offset+PSV.TRIG_LMT )))
	{
		LAST_FLOW_TRIGGER 							= FLOW_TRIGGER;
		Patient_Trigger  							= PATIENT_TRIGGER_HAPPEN;
		PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL = 0;
		Blower_Signal( PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL);
		INSPIRATION_PROCESS_PSV();

	}
}

static void CHECK_TRIGGER_OFFSET()
{
	if(Flow_Sensor_cal._Flow_Val==0 || (Flow_Sensor_cal._Flow_Val>=(-8) && Flow_Sensor_cal._Flow_Val<0))
	{
		FLOW_TRIGGER             = 0;
		Pressure_Trigger_Offset  =  Pressure_sensor.Pressure_Val;
		Flow_Trigger_Offset      =  Flow_Sensor_cal._Flow_Val;
	}
}


static void RUN_PSV_PEEP_MAINTAIN_BLOWER()
{
	ExpValve_CLOSE();
	Blower_Signal( PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL);
	Pressure_Mode_blower_control.Blower_Signal_Voltage = ( (PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL*3.3) /4095);
}


static void STOP_BLOWER_IF_FLOW_SENSE()
{
	Blower_Signal(0);
	PSV_Control.Blower_Wait_Time_Milli_Second = 2000;
}



static void PSV_PEEP_AVERAGE()
{
	if((PSV_Peep_Average.PEEP_Cumulative_Val == 0) || (PSV_Peep_Average.PEEP_Event_Count == 0))
	{
		PSV_Peep_Average.PEEP_Cumulative_Val = 1;
		PSV_Peep_Average.PEEP_Event_Count    = 1;
	}
	PSV_Peep_Average.PEEP_AVG_VAL        = (uint32_t)(PSV_Peep_Average.PEEP_Cumulative_Val/ PSV_Peep_Average.PEEP_Event_Count);
	PSV_Peep_Average.PEEP_Cumulative_Val = RESET;
	PSV_Peep_Average.PEEP_Event_Count    = RESET;
	if(PSV_Peep_Average.PEEP_AVG_VAL == PSV.PEEP_Val)
	{
		Change_DAC_Values(PSV.PEEP_Val,PSV_Mode_Blower_control.LAST_PEEP_BLOWER_DAC_VAL);
	}
}



static void PSV_PIP_AVERAGE()
{
	if((PSV_Pip_average.PRESSURE_SUPPORT_Cumulative_Val == 0) || (PSV_Pip_average.PRESSURE_SUPPORT_Event_Count == 0))
	{
		PSV_Pip_average.PRESSURE_SUPPORT_Cumulative_Val = 1;
		PSV_Pip_average.PRESSURE_SUPPORT_Event_Count    = 1;
	}
	PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL        = (uint32_t)(PSV_Pip_average.PRESSURE_SUPPORT_Cumulative_Val/ PSV_Pip_average.PRESSURE_SUPPORT_Event_Count);
	PSV_Pip_average.PRESSURE_SUPPORT_Cumulative_Val = RESET;
	PSV_Pip_average.PRESSURE_SUPPORT_Event_Count    = RESET;
	if(PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL == PSV.PRESSURE_SUPPORT_Val)
	{
		Change_DAC_Values(PSV.PRESSURE_SUPPORT_Val,Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL);
	}
}


static uint16_t PSV_TUNE_PEEP_BLOWER(uint16_t *Last_Blower_DAC_Val,uint8_t Set_PEEP)
{
	if(  ( PSV_Peep_Average.PEEP_AVG_VAL)  > ( Set_PEEP ) && (PSV_Peep_Average.PEEP_AVG_VAL  < (Set_PEEP + 2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-10);
	}
	else if(  ( PSV_Peep_Average.PEEP_AVG_VAL)  >= ( Set_PEEP + 2 ) && (PSV_Peep_Average.PEEP_AVG_VAL  < (Set_PEEP + 4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-20);
	}
	else if(  ( PSV_Peep_Average.PEEP_AVG_VAL)  >= ( Set_PEEP + 4 )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-40);
	}
	else if( (PSV_Peep_Average.PEEP_AVG_VAL  < Set_PEEP) && (PSV_Peep_Average.PEEP_AVG_VAL  > (Set_PEEP -2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+10);
	}
	else if(  (PSV_Peep_Average.PEEP_AVG_VAL  <= (Set_PEEP-2) ) && (PSV_Peep_Average.PEEP_AVG_VAL  >  (Set_PEEP-4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+20);
	}
	else if(  (PSV_Peep_Average.PEEP_AVG_VAL  <= (Set_PEEP-4) )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+40);
	}
	else
	{
		*Last_Blower_DAC_Val = *Last_Blower_DAC_Val;
	}
	return *Last_Blower_DAC_Val;
}


static uint16_t PSV_TUNE_PIP_BLOWER(uint16_t *Last_Blower_DAC_Val,uint8_t Set_PIP)
{

	if(  ( PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL)  > ( Set_PIP ) && (PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL  < (Set_PIP + 2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-10);
	}
	else if(  ( PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL)  >= ( Set_PIP + 2 ) && (PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL  < (Set_PIP + 4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-20);
	}
	else if(  ( PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL)  >= ( Set_PIP + 4 )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-40);
	}
	else if( (PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL  < Set_PIP) && (PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL  > (Set_PIP -2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+10);
	}
	else if(  (PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL  <= (Set_PIP-2) ) && (PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL  >  (Set_PIP-4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+20);
	}
	else if(  (PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL  <= (Set_PIP-4) )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+40);
	}
	else
	{
		*Last_Blower_DAC_Val = *Last_Blower_DAC_Val;
	}
	return *Last_Blower_DAC_Val;
}



static void BACKUP_PC_CMV_START()
{
	if(Backup_PC_CMV_SET_VALUE == CHANGE_HAPPEN)
	{
		Mode                                                  = Backup_Pccmv;
		Peep_Status                   				          = PEEP_NOT_ACHEIVED;
		Expiratory_Valve_Lock_Delay   			              = OPENED;
		Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = DAC_VAL(BACKUP_PC_CMV.PIP_Val);
		if( (Common_Mode_Parameter._PIP_Val < 30) && (Common_Mode_Parameter._PEEP_Val < 9) )
		{
			PIP_control_Task.PIP_Control_Task_Delay           = 10;
			PIP_control_Task.Increase_Each_DAC_SET_CONST      = 50;
		}
		else
		{
			PIP_control_Task.PIP_Control_Task_Delay           = 2;
			PIP_control_Task.Increase_Each_DAC_SET_CONST      = 140;
		}

		if( (Common_Mode_Parameter._PIP_Val < 30) && (Common_Mode_Parameter._PEEP_Val < 9) )
		{
			BACKUP_PC_CMV.RISE_TIME_MS_Val = 800;
		}
		else if((Common_Mode_Parameter._PEEP_Val < 9))
		{
			BACKUP_PC_CMV.RISE_TIME_MS_Val = 600;
		}
		Smooth_Ramp_Time_Val_Count = 0;
		Smooth_Ramp_Time_Val       = 15;
		if(Common_Mode_Parameter._PEEP_Val >= 15)
		{
		  Smooth_Ramp_Time           = Smooth_Ramp_Time_Val_Finished;
		}
		else if( (Common_Mode_Parameter._PEEP_Val < 15) && (Common_Mode_Parameter._PEEP_Val >= 9))
		{
		  Smooth_Ramp_Time           = Smooth_Ramp_Time_Val_Pending;
		}
		else
		{
			Smooth_Ramp_Time           = Smooth_Ramp_Time_Val_Finished;
		}
		PIP_Average_Parameter.PIP_Cumulative_Val              = 1;
		PIP_Average_Parameter.PIP_Event_Count                 = 1;
		PEEP_Maintain_Parameter.PEEP_Cumulative_Val           = 1;
		PEEP_Maintain_Parameter.PEEP_Event_Count              = 1;
		Need_PID_Task_Delay                                   = NO;
		Tune_PID_Task_Delay                                   = T_OPEN;
		PIP_control_Task.Low_PIP_Count                        = RESET;
		PIP_control_Task.Last_Early_Acheived_Ramp_Time_Percentage  = RESET;
		Next_Half_Breath_Cycle                                = Generate_Expiration_Cycle;
	 }
	 else if(Backup_PC_CMV_SET_VALUE == NO_CHANGES)
	 {
		Mode                                                  = Backup_Pccmv;
		Peep_Status                   				          = PEEP_NOT_ACHEIVED;
		Expiratory_Valve_Lock_Delay   			              = OPENED;
		Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = OLD.LAST_BLOWER_ENDING_DAC_SIGNAL;;
		PIP_control_Task.PIP_Control_Task_Delay               = OLD.LAST_PIP_Control_Task_Delay;
		PIP_control_Task.Increase_Each_DAC_SET_CONST          = OLD.LAST_Increase_Each_DAC_SET_CONST;
		PEEP_Maintain_Parameter.PEEP_Cumulative_Val           = PEEP_Maintain_Parameter.PEEP_AVG_VAL;
		PEEP_Maintain_Parameter.PEEP_Event_Count              = 1;
		Next_Half_Breath_Cycle                                = Generate_Expiration_Cycle;
	 }
		vTaskResume(One_Time_Handler);
		vTaskResume(BACKUP_PC_Handler);
		vTaskResume(PIP_Control_Task_Handler);
		vTaskResume(Alert_Handler);
		if(Common_Mode_Parameter._FIO2_Val > 21)
		{
			O2_Initial_Parameter(Common_Mode_Parameter._FIO2_Val);
			vTaskResume(Oxygen_Blending_Handler);
		}
		else
		{
			Parkar_valve_Signal(0);
			vTaskSuspend(Oxygen_Blending_Handler);
		}



		vTaskSuspend(PSV_PID_Handler);
		vTaskSuspend(PSV_Handler);
}




static void RESET_FLAGS()
{
	Blower_Signal(0);
	Peep_Status                   				= PEEP_NOT_ACHEIVED;
	Expiratory_Valve_Lock_Delay   			    = OPENED;
	Clear_All_Alert_Bits();
	Send_Bluetooth_Data_Farme                   = SEND_ALERT_FRAME;
	Apnea_Alert                                 = APNEA_ALERT_HAPPEN;
	Alert_Status                                = ALERT_HAPPEN;
	SET_ALERT_BIT(SECOND_FRAME_UN,_ALERT_APNEA);
	LED_Alert();
}


static void Patient_Circuit_Disconnected_Alert_PSV()
{
	if(  (Pressure_sensor.Pressure_Val==0) && (Flow_Sensor_cal._Flow_Val==0 || Flow_Sensor_cal._Flow_Val==-1 || Flow_Sensor_cal._Flow_Val==-2))
	{
			if((DAC1->DHR12R1)>=400)
			{
				if(PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK == CHECK)
				{
					Alert_Count.Patient_Circuit_disconnected_Alert_Count++;
						if(Alert_Count.Patient_Circuit_disconnected_Alert_Count >= 500)
						{
							Clear_All_Alert_Bits();
							Alert_Status = ALERT_HAPPEN;
							SET_ALERT_BIT(FIRST_FRAME_UN,_ALERT_PATIENT_CIRCUIT_DISCONNECTED);
							PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK = DO_NOT_CHECK;
							Send_Bluetooth_Data_Farme                = SEND_ALERT_FRAME;
							Apnea_Alert                              = APNEA_ALERT_HAPPEN;
							PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK = DO_NOT_CHECK;
							LED_Alert();
							PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL           = DAC_VAL(PSV.PEEP_Val );
							PSV_Mode_Blower_control.LAST_PEEP_BLOWER_DAC_VAL      = PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL;


						}
				}
			}
	}
}
