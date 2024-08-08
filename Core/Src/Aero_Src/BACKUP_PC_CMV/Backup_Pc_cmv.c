/*
 * Backup_Pc_cmv.c
 *
 *  Created on: Feb 21, 2023
 *      Author: asus
 */


#include "Backup_Pc_cmv.h"



static void Inspiration_Time_Blower_Control_BACKUP_PC_CMV(uint16_t);
static void Expiration_Time_Blower_Control_BACKUP_PC_CMV(uint16_t,float,uint16_t);
static void CHECK_TRIGGER_BACKUP_PC_CMV(uint16_t);
static void CHECK_PRESSURE_TRIGGER();
static void CHECK_FLOW_TRIGGER();
static void CHECK_TRIGGER_OFFSET();
static void Set_Blower_Val();
static void Smooth_Ramp_Pressure();
static void Trigger_Time_Monitor();

extern uint16_t Expiratory_Valve_Open_Time;

void Backup_Pc_Cmv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet)
{
	uint32_t One_Breathe_time;
	BACKUP_PC_CMV.PIP_Val                = Receive_Graph_Packet->PIP_PS_Phigh;
	BACKUP_PC_CMV.PEEP_Val               = Receive_Graph_Packet->PEEP_CPAP_Plow;
	BACKUP_PC_CMV.FIO2_Val               = Receive_Graph_Packet->FiO2;

	BACKUP_PC_CMV.RESPIRATORY_RATE_Val   = Receive_Graph_Packet->RR;
	One_Breathe_time                     = (One_Minite_In_MS / BACKUP_PC_CMV.RESPIRATORY_RATE_Val);
	BACKUP_PC_CMV.INSPIRATION_TIME       = ( ( Receive_Graph_Packet->T_high) * 100 );
	BACKUP_PC_CMV.EXPIRATION_TIME        = (One_Breathe_time - BACKUP_PC_CMV.INSPIRATION_TIME);


	BACKUP_PC_CMV.Rise_Time              = Receive_Graph_Packet->Rise_Time;
	BACKUP_PC_CMV.RISE_TIME_MS_Val       = (BACKUP_PC_CMV.Rise_Time *100);


	Back_Up_Mode_Initial_Flag_Set        = OPEN;
	vTaskResume(Mode_initial_Flag_Set_Handler);
}



void BACKUP_PC_CMV_Task(void *argument)
{
	while(1)
	{
		switch (Run_Current_Breathe_State)
		{
			case Run_Inspiration_Cycle:
				Inspiration_Time_Blower_Control_BACKUP_PC_CMV(Pressure_Mode_blower_control.BLOWER_DAC_VAL);
			break;
			case Run_Expiration_Cycle:
				Pressure_Mode_blower_control.BLOWER_DAC_VAL=0;
				Expiration_Time_Blower_Control_BACKUP_PC_CMV(Pressure_Mode_blower_control.BLOWER_DAC_VAL,
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




static void Inspiration_Time_Blower_Control_BACKUP_PC_CMV(uint16_t Blower_Signal)
{
	Pressure_Mode_blower_control.Blower_Signal_Voltage = ( (Blower_Signal*3.3) /4095);
	Blower_Signal(Blower_Signal);
}

static void Expiration_Time_Blower_Control_BACKUP_PC_CMV(uint16_t Blower_Signal,float PEEP,uint16_t Expiratory_Valve_Open_Time)
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

		CHECK_TRIGGER_BACKUP_PC_CMV(Common_Mode_Parameter.TRIG_WINDOW );
}



static void CHECK_TRIGGER_BACKUP_PC_CMV(uint16_t Trigger_Check_Time)
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



static void CHECK_PRESSURE_TRIGGER()
{
	if (( Pressure_sensor.Pressure_Val < (Pressure_Trigger_Offset - Common_Mode_Parameter.TRIG_LMT  )))
	{
		Patient_Trigger = PATIENT_TRIGGER_HAPPEN;
				if(Running_Mode == Psv)
				{
					START_PSV();
				}
				else if(Running_Mode == BiPap)
				{
					START_BIPAP();
				}
				else if(Running_Mode == Cpap)
				{
					START_CPAP();
				}
	}
}


static void CHECK_FLOW_TRIGGER()
{
	if ((FLOW_TRIGGER > (Flow_Trigger_Offset+Common_Mode_Parameter.TRIG_LMT  )))
	{
		LAST_FLOW_TRIGGER = FLOW_TRIGGER;
		Patient_Trigger   = PATIENT_TRIGGER_HAPPEN;
	        	if(Running_Mode == Psv)
				{
					START_PSV();
				}
				else if(Running_Mode == BiPap)
				{
					START_BIPAP();
				}
				else if(Running_Mode == Cpap)
				{
					START_CPAP();
				}
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




void BACKUP_PC_CMV_Inspiration_Time_Variables_Reset()
{
	Breath_Count         = Count_The_Breath;
	Smooth_Ramp_Pressure();
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
	if( (BACKUP_PC_CMV.PEEP_Val != PEEP_Maintain_Parameter.PEEP_AVG_VAL ) )
	{
		PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time     = PEEP_Control_For_Expiratory_Valve_Open_Time_In_Pressure_Mode(PEEP_Maintain_Parameter.PEEP_AVG_VAL,
																															 BACKUP_PC_CMV.RESPIRATORY_RATE_Val,
																															 BACKUP_PC_CMV.INSPIRATION_TIME,
																															 BACKUP_PC_CMV.PEEP_Val,
																											                 PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time);

	}
	INCREASE_EACH_DAC_SET_CONST(Common_Mode_Parameter._PIP_Val,BACKUP_PC_CMV.RISE_TIME_MS_Val);
	BLOWER_ENDING_DAC_SIGNAL_TUNE(BACKUP_PC_CMV.PIP_Val,
								  BACKUP_PC_CMV.RESPIRATORY_RATE_Val,
								  BACKUP_PC_CMV.Expiration_Time_Ratio,
								  BACKUP_PC_CMV.Rise_Time);
	BREATH_STATE                                               = INSPIRATION_CYCLE;
	Peep_Status                   				               = PEEP_NOT_ACHEIVED;
	Expiratory_Valve_Lock_Delay   			                   = OPENED;
	PIP_Status                    				               = PIP_NOT_ACHEIVED;
	Set_PIP_Status                                             = SET_PIP_VALUE_NOT_ACHEIVED;
	Read_Time_vs_Pressure                                      = READ_TIME_OPEN;
	PIP_control_Task.PIP_Control_Event_Occured                 = RESET;
	PIP_Average_Parameter.Maximum_PIP_Acheived                 = RESET;
	Pressure_Mode_blower_control.BLOWER_DAC_VAL                = DAC_VAL(BACKUP_PC_CMV.PEEP_Val );
	Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL           = Pressure_Mode_blower_control.BLOWER_DAC_VAL;
	TIME_Base_parameter._INSPIRATION_TIME                      = BACKUP_PC_CMV.INSPIRATION_TIME;

	Led_Delay                                                  = (PC_CMV.RISE_TIME_MS_Val  /46);
	LED_ITER                                                   = 1;

	TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED              = RESET;
	PIP_control_Task.Last_Early_Acheived_Ramp_Time_Percentage  = PIP_control_Task.Early_Acheived_Ramp_Time_Percentage;
	vol.Volume                                                 = RESET;
	vol.Volume_max                                             = RESET;
	Next_Half_Breath_Cycle                                     = Generate_Expiration_Cycle;
	Run_Current_Breathe_State                                  = Run_Inspiration_Cycle;
	vTaskDelay(BACKUP_PC_CMV.INSPIRATION_TIME);
}

void BACKUP_PC_CMV_Expiration_Time_Variables_Reset()
{
	BREATH_STATE                                        = EXPIRATION_CYCLE;
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger = RESET;
	PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger    = RESET;
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val         = RESET;
	PEEP_Maintain_Parameter.PEEP_Event_Count            = RESET;

	Patient_Trigger                                     = NO_PATIENT_TRIGGER;

	Trigger_Time_Monitor();

	Adjust_Servo_And_Calculate_O2(Common_Mode_Parameter._FIO2_Val);
	Alert_Expiration_Time_Parameter();
	TIME_Base_parameter._EXPIRATION_TIME                = BACKUP_PC_CMV.EXPIRATION_TIME;

	Led_Delay                                           = ( (PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time + 500)  /46);
	LED_ITER                                            = 45;

	TIME_Base_parameter.EXPIRATION_TIME_ACHEIVED        = RESET;
	Next_Half_Breath_Cycle                              = Generate_Inspiration_Cycle;
	Run_Current_Breathe_State                           = Run_Expiration_Cycle;
	vTaskDelay(BACKUP_PC_CMV.EXPIRATION_TIME);
}




void START_PSV()
{
	     if(Current_Backup_Mode == Backup_PCCMV_Mode)
		 {
	    	 OLD.LAST_BLOWER_ENDING_DAC_SIGNAL        =	DAC_VAL(BACKUP_PC_CMV.PIP_Val);;
	    	 OLD.LAST_PIP_Control_Task_Delay          =	PIP_control_Task.PIP_Control_Task_Delay ;
	    	 OLD.LAST_Increase_Each_DAC_SET_CONST     = PIP_control_Task.Increase_Each_DAC_SET_CONST ;
	 	     vTaskSuspend(PIP_Control_Task_Handler);
	 	     PSV_Control._INSPIRATION_TIME            = BACKUP_PC_CMV.INSPIRATION_TIME;

		 }



	    Parkar_valve_Signal(0);
		Oxygen_Parameter.Servo_Position=45;
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Servo_Position_Feedback = SERVO_FULL_OPEND;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
	    vTaskSuspend(Oxygen_Blending_Handler);
	    vTaskSuspend(Alert_Handler);
	    vTaskSuspend(One_Time_Handler);
	    vTaskSuspend(PSV_Handler);
		PIP_control_Task.PIP_Control_Task_Delay               = 2;
		PIP_control_Task.Increase_Each_DAC_SET_CONST          = 140;
		PIP_Status                    				          = PIP_NOT_ACHEIVED;
		Pressure_Mode_blower_control.BLOWER_DAC_VAL           = DAC_VAL(PSV.PEEP_Val);
		Pressure_Mode_blower_control.LAST_BLOWER_DAC_VAL      = Pressure_Mode_blower_control.BLOWER_DAC_VAL;
		Set_PIP_Status                                        = SET_PIP_VALUE_NOT_ACHEIVED;
		Read_Time_vs_Pressure                                 = READ_TIME_OPEN;
		PIP_control_Task.PIP_Control_Event_Occured            = RESET;
		PIP_Average_Parameter.Maximum_PIP_Acheived            = RESET;
		Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = DAC_VAL(PSV.PRESSURE_SUPPORT_Val);
		BREATH_STATE                                          = INSPIRATION_CYCLE;
		vol.Volume                                            = RESET;
		vol.Volume_max                                        = RESET;
		PSV_Peep_Average.PEEP_AVG_VAL                         = 0;
		vTaskResume(PSV_PID_Handler);
		if(Current_Backup_Mode == Backup_PCCMV_Mode)
		{
		   vTaskSuspend(BACKUP_PC_Handler);
		}

}



void START_BIPAP()
{
		if(Current_Backup_Mode == Backup_PCCMV_Mode)
		{
			 OLD.LAST_BLOWER_ENDING_DAC_SIGNAL     =  DAC_VAL(BACKUP_PC_CMV.PIP_Val);;
			 OLD.LAST_PIP_Control_Task_Delay       =  10;
			 OLD.LAST_Increase_Each_DAC_SET_CONST  =  70;
			 vTaskSuspend(PIP_Control_Task_Handler);
		}

		Parkar_valve_Signal(0);
		Oxygen_Parameter.Servo_Position=45;
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Servo_Position_Feedback = SERVO_FULL_OPEND;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		BIPAP_Peep_Average.EPAP_AVG_VAL = 0;
		vTaskSuspend(Oxygen_Blending_Handler);
		vTaskSuspend(Alert_Handler);
	    vTaskSuspend(One_Time_Handler);
		Blower_Signal(0);
		vol.Volume                                            =  RESET;
		vol.Volume_max                                        =  RESET;
		BIPAP.APNEA_COUNTER                                   = (1000*BIPAP.APNEA_TIME) ;
		BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL         = DAC_VAL(BIPAP.EPAP_Val );
		Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = DAC_VAL(BIPAP.IPAP_Val);
		BREATH_STATE                                          = EXPIRATION_CYCLE;
		Patient_Trigger                                       = NO_PATIENT_TRIGGER;
		BIPAP_Control.BIPAP_EPAP_Settle_Time                  = 2000;
		vTaskSuspend(BIPAP_PID_Handler);
		vTaskResume(BIPAP_Handler);
		if(Current_Backup_Mode == Backup_PCCMV_Mode)
		{
			vTaskSuspend(BACKUP_PC_Handler);
		}

}



void START_CPAP()
{
	   BREATH_STATE                                          = PATIENT_TRIGGER_INSPIRATION;
		 if(Current_Backup_Mode == Backup_PCCMV_Mode)
		 {
			 OLD.LAST_BLOWER_ENDING_DAC_SIGNAL     =	 DAC_VAL(BACKUP_PC_CMV.PIP_Val);;
			 OLD.LAST_PIP_Control_Task_Delay       =	 10;
			 OLD.LAST_Increase_Each_DAC_SET_CONST  =     70;
			 vTaskSuspend(PIP_Control_Task_Handler);
		 }

		Parkar_valve_Signal(0);
		Oxygen_Parameter.Servo_Position=45;
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Servo_Position_Feedback = SERVO_FULL_OPEND;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		vTaskSuspend(Oxygen_Blending_Handler);
		vTaskSuspend(Alert_Handler);
	    vTaskSuspend(One_Time_Handler);
		Blower_Signal(0);
		vol.Volume                                        =  RESET;
		vol.Volume_max                                    =  RESET;
		CPAP.APNEA_COUNTER                                = (1000*CPAP.APNEA_TIME) ;
		CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL      = DAC_VAL(CPAP.CPAP_Val );
		BREATH_STATE                                      = EXPIRATION_CYCLE;
		Patient_Trigger                                   = NO_PATIENT_TRIGGER;
		CPAP_Control.CPAP_Settle_Time                     = 2000;
		CPAP_Average.CPAP_AVG_VAL                         = 0;
		vTaskResume(CPAP_Handler);
		if(Current_Backup_Mode == Backup_PCCMV_Mode)
		{
		   vTaskSuspend(BACKUP_PC_Handler);
		}
}

static void Set_Blower_Val()
{
	if(PIP_Average_Parameter.PIP_AVG_VAL  == BACKUP_PC_CMV.PIP_Val  )
	{
		Change_DAC_Values(BACKUP_PC_CMV.PIP_Val ,Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL);
	}
}



static void Smooth_Ramp_Pressure()
{
	if(Smooth_Ramp_Time   == Smooth_Ramp_Time_Val_Pending)
	{
		if( (BACKUP_PC_CMV.PEEP_Val == PEEP_Maintain_Parameter.PEEP_AVG_VAL)  && (BACKUP_PC_CMV.PEEP_Val >= 9) && (BACKUP_PC_CMV.PEEP_Val <= 14) && (BACKUP_PC_CMV.PIP_Val >=30) )
		{
			if(PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time  < 2)
			{
				if(Smooth_Ramp_Time_Val_Count > 2 )
				{
					Smooth_Ramp_Time_Val = BACKUP_PC_CMV.PEEP_Val;
					Smooth_Ramp_Time     = Smooth_Ramp_Time_Val_Finished;
				}
				else
				{
					Smooth_Ramp_Time_Val_Count++;
				}
			}
		}
    }
}




static void Trigger_Time_Monitor()
{
	uint16_t CALC_TRIG_VAL;
	CALC_TRIG_VAL 	    =  (BACKUP_PC_CMV.EXPIRATION_TIME   - PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time );
	if(CALC_TRIG_VAL <0)
	{
		CALC_TRIG_VAL = (CALC_TRIG_VAL * (-1));
	}

	Common_Mode_Parameter.TOLERANCE_EWAVE =  Common_Mode_Parameter.TRIG_TIME;
	if(CALC_TRIG_VAL < Common_Mode_Parameter.TRIG_TIME )
	{
		Common_Mode_Parameter.TOLERANCE_EWAVE =  (Common_Mode_Parameter.TRIG_TIME - 200) ;
	}
	Common_Mode_Parameter.TRIG_WINDOW        = Common_Mode_Parameter.TOLERANCE_EWAVE;
}
