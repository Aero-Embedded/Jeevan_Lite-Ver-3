/*
 * Cpap.c
 *
 *  Created on: Feb 27, 2023
 *      Author: asus
 */


#include "Cpap.h"


static void CPAP_CONTROL();
static void RUN_CPAP_MAINTAIN_BLOWER();
static void STOP_BLOWER_IF_FLOW_SENSE();
static void CHECK_PRESSURE_TRIGGER();
static void CHECK_FLOW_TRIGGER();
static void CHECK_TRIGGER_OFFSET();
static void CPAP_AVERAGE();
static uint16_t TUNE_CPAP_BLOWER(uint16_t *,uint8_t);
static void RESET_FLAGS();
static void BACKUP_PC_CMV_START();
static void Patient_Circuit_Disconnected_Alert_CPAP();



void CPAP_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet)
{
	 CPAP.CPAP_Val                         = Receive_Graph_Packet->PEEP_CPAP_Plow;

	 CPAP.TRIG_TYPE                        = (0x40 & (Receive_Graph_Packet->Control_Byte))>>6;
	 CPAP.TRIG_LMT                         = ( (Receive_Graph_Packet->Trigger_Limit) / 10);
	 CPAP.TRIG_TIME                        = ( (Receive_Graph_Packet->Trigger_Time) * 100);

	 CPAP.APNEA_TIME                       = Receive_Graph_Packet->Apnea_Time;
	 CPAP.APNEA_COUNTER                    = (1000*CPAP.APNEA_TIME) ;
	 if(CPAP.TRIG_TYPE == 0)
		Trigger_Type = Pressure_Trigger;
	else if(CPAP.TRIG_TYPE == 1)
		Trigger_Type = Flow_Trigger;

	 Mode_Initial_Flag_Set                  = OPEN;
	 vTaskResume(Mode_initial_Flag_Set_Handler);

}


void CPAP_Task(void *argument)
{
	while(1)
	{
		if(CPAP.APNEA_COUNTER > 0)
		{
			//Green_Led();
			CPAP_CONTROL();
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



static void CPAP_CONTROL()
{

	    if(Pressure_sensor.Pressure_Val >= (CPAP.CPAP_Val + 5) )
	    {
	    	Blower_Signal(0);
	    	Pressure_Mode_blower_control.Blower_Signal_Voltage = 0;
	    	Pressure_Status = OVER_PRESSURE;
	    	ExpValve_OPEN();
	    	BREATH_STATE    = PATIENT_TRIGGER_EXPIRATION;
	    	vTaskDelay(700);
	    	CPAP_Control.CPAP_Settle_Time   = 2000;
	    	Pressure_Status = NORMAL_PRESSURE;
	    }

	    else
	    {
	    	BREATH_STATE    = EXPIRATION_CYCLE;
	    	LED_STATUS      = EXPIRATION_LED;
				if(CPAP_Control.Blower_Wait_Time_Milli_Second == 0)
				{
					RUN_CPAP_MAINTAIN_BLOWER();
				}


				if(CPAP_Control.CPAP_Settle_Time == 0 )
				{

					Patient_Circuit_Disconnected_Alert_CPAP();

					if( (Flow_Sensor_cal._Flow_Val > 0) && (CPAP_Control.Trigger_Check_Wait_Time == 0))
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
						CPAP_Average.CPAP_Cumulative_Val += Pressure_sensor.Pressure_Val;
						CPAP_Average.CPAP_Event_Count++;
						if(CPAP_Average.CPAP_Event_Count >1000)
						{
							Reset_Patient_Circuit_Diconnceted_Alert_Flags();
							CPAP_AVERAGE();
							CPAP_Mode_Blower_control.LAST_CPAP_BLOWER_DAC_VAL    = TUNE_CPAP_BLOWER(&CPAP_Mode_Blower_control.LAST_CPAP_BLOWER_DAC_VAL ,
									                                                                CPAP.CPAP_Val );
							CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL         = CPAP_Mode_Blower_control.LAST_CPAP_BLOWER_DAC_VAL;
							CPAP_Control.CPAP_Settle_Time                    = 1000;
						}
					}
				}
				else
				{
					CHECK_TRIGGER_OFFSET();
				}
	    }
}



static void RUN_CPAP_MAINTAIN_BLOWER()
{
	ExpValve_CLOSE();
	Blower_Signal( CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL);
	Pressure_Mode_blower_control.Blower_Signal_Voltage = ( (CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL*3.3) /4095);
}


static void STOP_BLOWER_IF_FLOW_SENSE()
{
	Blower_Signal(0);
	CPAP_Control.Blower_Wait_Time_Milli_Second = 2000;
}




static void CHECK_PRESSURE_TRIGGER()
{
	if (( Pressure_sensor.Pressure_Val < (Pressure_Trigger_Offset - CPAP.TRIG_LMT )))
	{

		Breath_Count                                 = Count_The_Breath;
		Alert_Status                                 = ALERT_HAPPEN;
		LED_Alert();
		Patient_Trigger                              = PATIENT_TRIGGER_HAPPEN;
		Patient_Trigger_Count++;
		CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL = 0;
		Blower_Signal( CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL);
		CPAP_Control.CPAP_Settle_Time                = 1000;
		CPAP.APNEA_COUNTER                           = (1000*CPAP.APNEA_TIME) ;

	}
}

static void CHECK_FLOW_TRIGGER()
{
	if ((FLOW_TRIGGER > (Flow_Trigger_Offset+CPAP.TRIG_LMT )))
	{
		Breath_Count                                = Count_The_Breath;
		Alert_Status                                = ALERT_HAPPEN;
		LED_Alert();
		LAST_FLOW_TRIGGER 							= FLOW_TRIGGER;
		Patient_Trigger  							= PATIENT_TRIGGER_HAPPEN;
		Patient_Trigger_Count++;
		CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL = 0;
		Blower_Signal( CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL);
		CPAP.APNEA_COUNTER                    = (1000*CPAP.APNEA_TIME) ;


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


static void CPAP_AVERAGE()
{
	if((CPAP_Average.CPAP_Cumulative_Val == 0) || (CPAP_Average.CPAP_Event_Count == 0))
	{
		CPAP_Average.CPAP_Cumulative_Val = 1;
		CPAP_Average.CPAP_Event_Count    = 1;
	}
	CPAP_Average.CPAP_AVG_VAL        = (uint32_t)(CPAP_Average.CPAP_Cumulative_Val/ CPAP_Average.CPAP_Event_Count);
	CPAP_Average.CPAP_Cumulative_Val = RESET;
	CPAP_Average.CPAP_Event_Count    = RESET;

	if(CPAP_Average.CPAP_AVG_VAL   == CPAP.CPAP_Val)
	{
		Change_DAC_Values(CPAP.CPAP_Val,CPAP_Mode_Blower_control.LAST_CPAP_BLOWER_DAC_VAL );
	}
}



static uint16_t TUNE_CPAP_BLOWER(uint16_t *Last_Blower_DAC_Val,uint8_t Set_PEEP)
{
	if(  ( CPAP_Average.CPAP_AVG_VAL)  > ( Set_PEEP ) && (CPAP_Average.CPAP_AVG_VAL  < (Set_PEEP + 2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-10);
	}
	else if(  ( CPAP_Average.CPAP_AVG_VAL)  >= ( Set_PEEP + 2 ) && (CPAP_Average.CPAP_AVG_VAL  < (Set_PEEP + 4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-20);
	}
	else if(  ( CPAP_Average.CPAP_AVG_VAL)  >= ( Set_PEEP + 4 )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-40);
	}
	else if( (CPAP_Average.CPAP_AVG_VAL  < Set_PEEP) && (CPAP_Average.CPAP_AVG_VAL  > (Set_PEEP -2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+10);
	}
	else if(  (CPAP_Average.CPAP_AVG_VAL  <= (Set_PEEP-2) ) && (CPAP_Average.CPAP_AVG_VAL  >  (Set_PEEP-4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+20);
	}
	else if(  (CPAP_Average.CPAP_AVG_VAL  <= (Set_PEEP-4) )  )
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
	Patient_Trigger_Count                       = 0;
	Clear_All_Alert_Bits();
	Send_Bluetooth_Data_Farme                   = SEND_ALERT_FRAME;
	Apnea_Alert                                 = APNEA_ALERT_HAPPEN;
	Alert_Status                                = ALERT_HAPPEN;
	SET_ALERT_BIT(SECOND_FRAME_UN,_ALERT_APNEA);
	LED_Alert();
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
	vTaskSuspend(CPAP_Handler);
}







static void Patient_Circuit_Disconnected_Alert_CPAP()
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
							CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL           = DAC_VAL(CPAP.CPAP_Val );
							CPAP_Mode_Blower_control.LAST_CPAP_BLOWER_DAC_VAL      = CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL;

						}
				}
			}
	}
}
