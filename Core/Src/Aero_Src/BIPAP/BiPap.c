/*
 * BiPap.c
 *
 *  Created on: Feb 24, 2023
 *      Author: asus
 */


#include "BiPap.h"


static void BIBAP_CONTROL();
static void RUN_BIPAP_EPAP_MAINTAIN_BLOWER();
static void STOP_BLOWER_IF_FLOW_SENSE();
static void CHECK_PRESSURE_TRIGGER();
static void CHECK_FLOW_TRIGGER();
static void CHECK_TRIGGER_OFFSET();
static void BIPAP_EPAP_AVERAGE();
static uint16_t BIPAP_TUNE_EPAP_BLOWER(uint16_t *,uint8_t);
static uint16_t BIBAP_TUNE_IPAP_BLOWER(uint16_t *,uint8_t );
static void RESET_FLAGS();
static void INSPIRATION_PROCESS_BIPAP();
static void BACKUP_PC_CMV_START();
static void BIBAP_IPAP_AVERAGE();
static void Patient_Circuit_Disconnected_Alert_BIBAP();



void BIPAP_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet)
{
	 BIPAP.IPAP_Val                         = Receive_Graph_Packet->PIP_PS_Phigh;
	 BIPAP.EPAP_Val                         = Receive_Graph_Packet->PEEP_CPAP_Plow;

	 BIPAP.TRIG_TYPE                        = (0x40 & (Receive_Graph_Packet->Control_Byte))>>6;
	 BIPAP.TRIG_LMT                         = ( (Receive_Graph_Packet->Trigger_Limit) / 10);
	 BIPAP.TRIG_TIME                        = ( (Receive_Graph_Packet->Trigger_Time) * 100);


	 BIPAP.APNEA_TIME                       = Receive_Graph_Packet->Apnea_Time;
	 BIPAP.APNEA_COUNTER                    = (1000*BIPAP.APNEA_TIME) ;

	 BIPAP._IPAP_TIME                       = ((Receive_Graph_Packet->T_high));
	 BIPAP.IPAP_TIME                        = (BIPAP._IPAP_TIME*100);

	 if(BIPAP.TRIG_TYPE == 0)
		Trigger_Type = Pressure_Trigger;
	else if(BIPAP.TRIG_TYPE == 1)
		Trigger_Type = Flow_Trigger;
	 Mode_Initial_Flag_Set                  = OPEN;
	 vTaskResume(Mode_initial_Flag_Set_Handler);

}




void BIPAP_Task(void *argument)
{
	while(1)
	{
		if(BIPAP.APNEA_COUNTER > 0)
		{
			BIBAP_CONTROL();
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




void BIPAP_PID_Task(void *argument)
{
	while(1)
	{
		if(BIPAP_Control._INSPIRATION_TIME > 0)
		{
			BREATH_STATE                                          = PATIENT_TRIGGER_INSPIRATION;
			PIP_Control(BIPAP.IPAP_Val );
			Blower_Signal(Pressure_Mode_blower_control.BLOWER_DAC_VAL);
			Pressure_Mode_blower_control.Blower_Signal_Voltage = ( (Pressure_Mode_blower_control.BLOWER_DAC_VAL*3.3) /4095);
			   if(BIPAP_Control._INSPIRATION_TIME < 100)
			   {
				   BIPAP_Pip_average.IPAP_Cumulative_Val += Pressure_sensor.Pressure_Val;
				   BIPAP_Pip_average.IPAP_Event_Count++;
			   }
		}
		else if(BIPAP_Control._INSPIRATION_TIME == 0)
		{
			Blower_Signal(0);
			ExpValve_OPEN();
			vTaskDelay(700);
			BIPAP.APNEA_COUNTER                                = (1000*BIPAP.APNEA_TIME) ;
			BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL      = DAC_VAL(BIPAP.EPAP_Val );
			BREATH_STATE                                       = EXPIRATION_CYCLE;
			Patient_Trigger                                    = NO_PATIENT_TRIGGER;
			BREATH_STATE                                       = PATIENT_TRIGGER_EXPIRATION;
			BIBAP_IPAP_AVERAGE();
			BIPAP_Control.BIPAP_EPAP_Settle_Time               = 2000;
			vTaskResume(BIPAP_Handler);
			vTaskSuspend(BIPAP_PID_Handler);
		}
		vTaskDelay(Ten_Millisecond_Delay);

	}
}


static void BIBAP_CONTROL()
{
	    BREATH_STATE                                     = EXPIRATION_CYCLE;
	    if(Pressure_sensor.Pressure_Val >= (BIPAP.EPAP_Val + 5) )
	    {
	    	Blower_Signal(0);
	    	Pressure_Mode_blower_control.Blower_Signal_Voltage = 0;
	    	Pressure_Status                                    = OVER_PRESSURE;
	    	ExpValve_OPEN();
	    	BREATH_STATE                                       = PATIENT_TRIGGER_EXPIRATION;
	    	vTaskDelay(700);
	    	BIPAP_Control.BIPAP_EPAP_Settle_Time   = 2000;
	    	Pressure_Status                        = NORMAL_PRESSURE;
	    }

	    else
	    {
	    	    LED_STATUS      = EXPIRATION_LED;

				if(BIPAP_Control.Blower_Wait_Time_Milli_Second == 0)
				{
					RUN_BIPAP_EPAP_MAINTAIN_BLOWER();
				}

				if(BIPAP_Control.BIPAP_EPAP_Settle_Time == 0 )
				{
					Patient_Circuit_Disconnected_Alert_BIBAP();

					if( (Flow_Sensor_cal._Flow_Val > 0) && (BIPAP_Control.Trigger_Check_Wait_Time == 0))
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
						BIPAP_Peep_Average.EPAP_Cumulative_Val += Pressure_sensor.Pressure_Val;
						BIPAP_Peep_Average.EPAP_Event_Count++;
						if(BIPAP_Peep_Average.EPAP_Event_Count >1000)
						{
							Reset_Patient_Circuit_Diconnceted_Alert_Flags();
							BIPAP_EPAP_AVERAGE();
							BIPAP_Mode_Blower_control.LAST_EPAP_BLOWER_DAC_VAL    = BIPAP_TUNE_EPAP_BLOWER(&BIPAP_Mode_Blower_control.LAST_EPAP_BLOWER_DAC_VAL,
									                                                                       BIPAP.EPAP_Val );
							BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL         = BIPAP_Mode_Blower_control.LAST_EPAP_BLOWER_DAC_VAL;
							BIPAP_Control.BIPAP_EPAP_Settle_Time                  = 1000;
						}
					}
				}
				else
				{
					CHECK_TRIGGER_OFFSET();
				}
	    }
}



static void RUN_BIPAP_EPAP_MAINTAIN_BLOWER()
{
	ExpValve_CLOSE();
	Blower_Signal( BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL);
	Pressure_Mode_blower_control.Blower_Signal_Voltage = ( (BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL*3.3) /4095);
}


static void STOP_BLOWER_IF_FLOW_SENSE()
{
	Blower_Signal(0);
	BIPAP_Control.Blower_Wait_Time_Milli_Second = 2000;
}


static void CHECK_PRESSURE_TRIGGER()
{
	if (( Pressure_sensor.Pressure_Val < (Pressure_Trigger_Offset - BIPAP.TRIG_LMT )))
	{
		Patient_Trigger                               = PATIENT_TRIGGER_HAPPEN;
		BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL = 0;
		Blower_Signal( BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL);
		INSPIRATION_PROCESS_BIPAP();

	}
}

static void CHECK_FLOW_TRIGGER()
{
	if ((FLOW_TRIGGER > (Flow_Trigger_Offset+BIPAP.TRIG_LMT )))
	{
		LAST_FLOW_TRIGGER 							= FLOW_TRIGGER;
		Patient_Trigger  							= PATIENT_TRIGGER_HAPPEN;
		BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL = 0;
		Blower_Signal( BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL);
		INSPIRATION_PROCESS_BIPAP();

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




static void BIPAP_EPAP_AVERAGE()
{
	if((BIPAP_Peep_Average.EPAP_Cumulative_Val == 0) || (BIPAP_Peep_Average.EPAP_Event_Count == 0))
	{
		BIPAP_Peep_Average.EPAP_Cumulative_Val = 1;
		BIPAP_Peep_Average.EPAP_Event_Count    = 1;
	}
	BIPAP_Peep_Average.EPAP_AVG_VAL        = (uint32_t)(BIPAP_Peep_Average.EPAP_Cumulative_Val/ BIPAP_Peep_Average.EPAP_Event_Count);
	BIPAP_Peep_Average.EPAP_Cumulative_Val = RESET;
	BIPAP_Peep_Average.EPAP_Event_Count    = RESET;

	if(BIPAP_Peep_Average.EPAP_AVG_VAL  == BIPAP.EPAP_Val)
	{
		Change_DAC_Values(BIPAP.EPAP_Val,BIPAP_Mode_Blower_control.LAST_EPAP_BLOWER_DAC_VAL);
	}
}



static uint16_t BIPAP_TUNE_EPAP_BLOWER(uint16_t *Last_Blower_DAC_Val,uint8_t Set_PEEP)
{
	if(  ( BIPAP_Peep_Average.EPAP_AVG_VAL)  > ( Set_PEEP ) && (BIPAP_Peep_Average.EPAP_AVG_VAL  < (Set_PEEP + 2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-10);
	}
	else if(  ( BIPAP_Peep_Average.EPAP_AVG_VAL)  >= ( Set_PEEP + 2 ) && (BIPAP_Peep_Average.EPAP_AVG_VAL  < (Set_PEEP + 4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-20);
	}
	else if(  ( BIPAP_Peep_Average.EPAP_AVG_VAL)  >= ( Set_PEEP + 4 )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-40);
	}
	else if( (BIPAP_Peep_Average.EPAP_AVG_VAL  < Set_PEEP) && (BIPAP_Peep_Average.EPAP_AVG_VAL  > (Set_PEEP -2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+10);
	}
	else if(  (BIPAP_Peep_Average.EPAP_AVG_VAL  <= (Set_PEEP-2) ) && (BIPAP_Peep_Average.EPAP_AVG_VAL  >  (Set_PEEP-4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+20);
	}
	else if(  (BIPAP_Peep_Average.EPAP_AVG_VAL  <= (Set_PEEP-4) )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+40);
	}
	else
	{
		*Last_Blower_DAC_Val = *Last_Blower_DAC_Val;
	}
	return *Last_Blower_DAC_Val;
}



static uint16_t BIBAP_TUNE_IPAP_BLOWER(uint16_t *Last_Blower_DAC_Val,uint8_t Set_PIP)
{
	if(  ( BIPAP_Pip_average.IPAP_AVG_VAL)  > ( Set_PIP ) && (BIPAP_Pip_average.IPAP_AVG_VAL  < (Set_PIP + 2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-10);
	}
	else if(  ( BIPAP_Pip_average.IPAP_AVG_VAL)  >= ( Set_PIP + 2 ) && (BIPAP_Pip_average.IPAP_AVG_VAL  < (Set_PIP + 4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-20);
	}
	else if(  ( BIPAP_Pip_average.IPAP_AVG_VAL)  >= ( Set_PIP + 4 )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-40);
	}
	else if( (BIPAP_Pip_average.IPAP_AVG_VAL  < Set_PIP) && (BIPAP_Pip_average.IPAP_AVG_VAL  > (Set_PIP -2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+10);
	}
	else if(  (BIPAP_Pip_average.IPAP_AVG_VAL  <= (Set_PIP-2) ) && (BIPAP_Pip_average.IPAP_AVG_VAL  >  (Set_PIP-4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+20);
	}
	else if(  (BIPAP_Pip_average.IPAP_AVG_VAL  <= (Set_PIP-4) )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+40);
	}
	else
	{
		*Last_Blower_DAC_Val = *Last_Blower_DAC_Val;
	}
	return *Last_Blower_DAC_Val;
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


static void INSPIRATION_PROCESS_BIPAP()
{
	Breath_Count                                          = Count_The_Breath;
	BIPAP_Control._INSPIRATION_TIME                       = BIPAP.IPAP_TIME;
	PIP_control_Task.PIP_Control_Task_Delay               = 10;
	PIP_control_Task.Increase_Each_DAC_SET_CONST          = 70;
	PIP_Status                    				          = PIP_NOT_ACHEIVED;
	Pressure_Mode_blower_control.BLOWER_DAC_VAL           = DAC_VAL(BIPAP.EPAP_Val);;
	Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL      = Pressure_Mode_blower_control.BLOWER_DAC_VAL;
	Set_PIP_Status                                        = SET_PIP_VALUE_NOT_ACHEIVED;
	Read_Time_vs_Pressure                                 = READ_TIME_OPEN;
	PIP_control_Task.PIP_Control_Event_Occured            = RESET;
	PIP_Average_Parameter.Maximum_PIP_Acheived            = RESET;
	Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = BIBAP_TUNE_IPAP_BLOWER(&Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL,
			                                                                      BIPAP.IPAP_Val );
	BREATH_STATE                                          = INSPIRATION_CYCLE;
	vol.Volume                                            = RESET;
	vol.Volume_max                                        = RESET;
	BIPAP_EPAP_AVERAGE();
	LED_STATUS = INSPIRATION_lED;
	vTaskResume(BIPAP_PID_Handler);
	vTaskSuspend(BIPAP_Handler);

}




static void BACKUP_PC_CMV_START()
{
	if(Backup_PC_CMV_SET_VALUE == CHANGE_HAPPEN)
	{
			Mode                                                  = Backup_Pccmv;
			Peep_Status                   				          = PEEP_NOT_ACHEIVED;
			Expiratory_Valve_Lock_Delay   			              = OPENED;
			Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = DAC_VAL(BACKUP_PC_CMV.PIP_Val);
			PIP_control_Task.PIP_Control_Task_Delay               = 10;
			PIP_control_Task.Increase_Each_DAC_SET_CONST          = 70;
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
		vTaskResume(Oxygen_Blending_Handler);
	}
	else
	{
		vTaskSuspend(Oxygen_Blending_Handler);
	}


	//vTaskResume(LED_Strip_Task_Handler);

	vTaskSuspend(BIPAP_PID_Handler);
	vTaskSuspend(BIPAP_Handler);
}






static void BIBAP_IPAP_AVERAGE()
{
	if((BIPAP_Pip_average.IPAP_Cumulative_Val == 0) || (BIPAP_Pip_average.IPAP_Event_Count == 0))
	{
		BIPAP_Pip_average.IPAP_Cumulative_Val = 1;
		BIPAP_Pip_average.IPAP_Event_Count    = 1;
	}
	BIPAP_Pip_average.IPAP_AVG_VAL        = (uint32_t)(BIPAP_Pip_average.IPAP_Cumulative_Val/ BIPAP_Pip_average.IPAP_Event_Count);
	BIPAP_Pip_average.IPAP_Cumulative_Val = RESET;
	BIPAP_Pip_average.IPAP_Event_Count    = RESET;
	if(BIPAP_Pip_average.IPAP_AVG_VAL == BIPAP.IPAP_Val)
	{
		Change_DAC_Values(BIPAP.IPAP_Val,Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL);
	}
}



static void Patient_Circuit_Disconnected_Alert_BIBAP()
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

							BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL           = DAC_VAL(BIPAP.EPAP_Val );
							BIPAP_Mode_Blower_control.LAST_EPAP_BLOWER_DAC_VAL      = BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL;

						}
				}
			}
	}
}
