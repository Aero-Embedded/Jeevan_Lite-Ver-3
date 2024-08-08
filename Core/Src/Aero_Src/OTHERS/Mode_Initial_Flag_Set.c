/*
 * Mode_Initial_Flag_Set.c
 *
 *  Created on: Jan 21, 2023
 *      Author: asus
 */


#include "Mode_Initial_Flag_Set.h"




void Mode_Initial_Flag_Set_Task (void *argument)
{
	while(1)
	{
		One_Minite_Or_60000_Ms  = 60000;
		Breath                  = 0;
		Breath_Count            = Leave_Count_The_Breath;
		switch (Mode_Initial_Flag_Set)
		{
			case OPEN:
				switch(Mode)
				{
					case Pccmv:
						PC_CMV_Initial_Flags_SET();
					break;
					case Vccmv:
						VC_CMV_Initial_Flags_SET();
					break;
					case Pcsimv:
						PC_SIMV_Initial_Flags_SET();
					break;
					case Vcsimv:
						VC_SIMV_Initial_Flags_SET();
					break;
					case Psv:
						PSV_Initial_Flags_SET();
					break;
					case Cpap:
						CPAP_Initial_Flags_SET();
					break;
					case BiPap:
						BIBAP_Initial_Flags_SET();
					break;
					case Aprv:
						APRV_Initial_Flags_SET();
					break;
					default:
					break;
				}
			break;
			default:
			break;
		}



		switch (Back_Up_Mode_Initial_Flag_Set)
		{
			case OPEN:
				switch(Current_Backup_Mode)
				{
					case Backup_PCCMV_Mode:
						Backup_PC_CMV_Initial_Flags_SET();
					break;
					default:
					break;
				}
			break;
			default:
			break;
		}
	}
}


void PC_CMV_Initial_Flags_SET()
{
	LED_STATUS = Idle_LED;
	APRV_current_breathe                                  = APRV_IDLE;
	APRV_Next_Breathe                                     = APRV_Idle;
	Common_Mode_Parameter._PIP_Val                        = PC_CMV.PIP_Val;
	Common_Mode_Parameter._PEEP_Val                       = PC_CMV.PEEP_Val;
	Common_Mode_Parameter._RESPIRATORY_RATE_Val           = PC_CMV.RESPIRATORY_RATE_Val;
	Common_Mode_Parameter._FIO2_Val                       = PC_CMV.FIO2_Val;
	Peep_Status                   				          = PEEP_NOT_ACHEIVED;
	Expiratory_Valve_Lock_Delay   			              = OPENED;
	PEEP_Maintain_Parameter.PEEP_Temp                     = (PC_CMV.PEEP_Val / 2);
	PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time    = 0;
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val           = 1;
	PEEP_Maintain_Parameter.PEEP_Event_Count              = 1;
    Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = DAC_VAL(PC_CMV.PIP_Val);
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
	    PC_CMV.RISE_TIME_MS_Val = 800;
	}
	else if((Common_Mode_Parameter._PEEP_Val < 9))
	{
		PC_CMV.RISE_TIME_MS_Val = 600;
	}
	Need_PID_Task_Delay                                   = NO;
	Tune_PID_Task_Delay                                   = T_OPEN;
	PIP_control_Task.Low_PIP_Count                        = RESET;
	PIP_control_Task.Last_Early_Acheived_Ramp_Time_Percentage  = RESET;
	Smooth_Ramp_Time_Val_Count                            = 0;
	Smooth_Ramp_Time_Val                                  = 15;
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
    Fio2_Blending_Method = PRESSURE_CONTROLLED;
    O2_Initial_Parameter(PC_CMV.FIO2_Val);
	Alert_Flags_Reset();
	Next_Half_Breath_Cycle                                = Generate_Expiration_Cycle;
	Run_Current_Breathe_State                             = Run_Expiration_Cycle;
	Back_Up_Mode_Initial_Flag_Set                         = CLOSE;
	Mode_Initial_Flag_Set                                 = CLOSE;
	Reset_Patient_Circuit_Diconnceted_Alert_Flags();
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(PSV_Handler);
	vTaskSuspend(PSV_PID_Handler);
	vTaskSuspend(CPAP_Handler);
	vTaskSuspend(BIPAP_Handler);
	vTaskSuspend(BIPAP_PID_Handler);
	vTaskSuspend(APRV_Handler);
	vTaskSuspend(APRV_ONE_TIME_Handler);
	vTaskResume(Alert_Handler);
	vTaskResume(One_Time_Handler);
	vTaskResume(Pc_Cmv_Handler);
	vTaskResume(PIP_Control_Task_Handler);
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}


void VC_CMV_Initial_Flags_SET()
{
	Reset_Blower_Status();
	LED_STATUS                                            = Idle_LED;
	APRV_current_breathe                                  = APRV_IDLE;
	APRV_Next_Breathe                                     = APRV_Idle;
	Total_Volume_Achieved_Time                            = 500;
	Common_Mode_Parameter._PEEP_Val                       = VC_CMV.PEEP_Val;
	Common_Mode_Parameter._RESPIRATORY_RATE_Val           = VC_CMV.RESPIRATORY_RATE_Val;
	Common_Mode_Parameter._FIO2_Val                       = VC_CMV.FIO2_Val;
	Common_Mode_Parameter._VOLUME_Val                     = VC_CMV.VOLUME_Val;
	Common_Mode_Parameter._FLOW_RATE                      = VC_CMV.FLOW_RATE_Val;
	Peep_Status                   				          = PEEP_NOT_ACHEIVED;
	Expiratory_Valve_Lock_Delay   			              = OPENED;
    PEEP_Maintain_Parameter.PEEP_Temp                     = 3;
    PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time    = 0;
    PEEP_Maintain_Parameter.PEEP_Cumulative_Val           = 1;
    PEEP_Maintain_Parameter.PEEP_Event_Count              = 1;


	LOW_COMPLIANCE_DAC                                         = LOW_Compliance_DAC_value(Common_Mode_Parameter._FLOW_RATE);
	Volume_Mode_Blower_control.PREVIOUS_INITIAL_BLOWER_DAC_VAL = LOW_COMPLIANCE_DAC;


    Fio2_Blending_Method = VOLUME_CONTROLLED;
    O2_Initial_Parameter(VC_CMV.FIO2_Val);
    INCREMENT_DAC_VAL_PER_ITERATION           = 3;
	Volume_control_Task.Volume_Stop_Value     = 10;
	Alert_Flags_Reset();
	Next_Half_Breath_Cycle                    = Generate_Expiration_Cycle;
	Run_Current_Breathe_State                 = Run_Expiration_Cycle;
	Back_Up_Mode_Initial_Flag_Set             = CLOSE;
	Mode_Initial_Flag_Set                     = CLOSE;
	Reset_Patient_Circuit_Diconnceted_Alert_Flags();
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);
	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(PSV_Handler);
	vTaskSuspend(PSV_PID_Handler);
	vTaskSuspend(CPAP_Handler);
	vTaskSuspend(BIPAP_Handler);
	vTaskSuspend(BIPAP_PID_Handler);
	vTaskSuspend(APRV_Handler);
	vTaskSuspend(APRV_ONE_TIME_Handler);
	vTaskResume(Alert_Handler);
	vTaskResume(One_Time_Handler);
	vTaskResume(Vc_Cmv_Handler);
	vTaskResume(Volume_Control_Task_Handler);
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}



void PC_SIMV_Initial_Flags_SET()
{
	TIME_Base_parameter._EXPIRATION_TIME                  = 0;
	LED_STATUS = Idle_LED;
	APRV_current_breathe                                  = APRV_IDLE;
	APRV_Next_Breathe                                     = APRV_Idle;
	Common_Mode_Parameter._PIP_Val                        = PC_SIMV.PIP_Val;
	Common_Mode_Parameter._PEEP_Val                       = PC_SIMV.PEEP_Val;
	Common_Mode_Parameter._RESPIRATORY_RATE_Val           = PC_SIMV.RESPIRATORY_RATE_Val;
	Common_Mode_Parameter._FIO2_Val                       = PC_SIMV.FIO2_Val;
	Peep_Status                   				          = PEEP_NOT_ACHEIVED;
	Expiratory_Valve_Lock_Delay   			              = OPENED;
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val           = 1;
	PEEP_Maintain_Parameter.PEEP_Event_Count              = 1;
	PEEP_Maintain_Parameter.PEEP_Temp                     = (PC_SIMV.PEEP_Val / 2);
	PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time    = 0;
	Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = DAC_VAL(PC_SIMV.PIP_Val);
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
		PC_SIMV.RISE_TIME_MS_Val = 800;
	}
	else if((Common_Mode_Parameter._PEEP_Val < 9))
	{
		PC_SIMV.RISE_TIME_MS_Val = 600;
	}
	Need_PID_Task_Delay                                   = NO;
	Tune_PID_Task_Delay                                   = T_OPEN;
	PIP_control_Task.Low_PIP_Count                        = RESET;
	PIP_control_Task.Last_Early_Acheived_Ramp_Time_Percentage  = RESET;
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

	Fio2_Blending_Method  = PRESSURE_CONTROLLED;
	O2_Initial_Parameter(PC_SIMV.FIO2_Val);
	Alert_Flags_Reset();
	Next_Half_Breath_Cycle                                = Generate_Expiration_Cycle;
	Run_Current_Breathe_State                             = Run_Expiration_Cycle;
	Back_Up_Mode_Initial_Flag_Set     = CLOSE;
	Mode_Initial_Flag_Set             = CLOSE;
	Reset_Patient_Circuit_Diconnceted_Alert_Flags();
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(PSV_Handler);
	vTaskSuspend(PSV_PID_Handler);
	vTaskSuspend(CPAP_Handler);
	vTaskSuspend(BIPAP_Handler);
	vTaskSuspend(BIPAP_PID_Handler);
	vTaskSuspend(APRV_Handler);
	vTaskSuspend(APRV_ONE_TIME_Handler);
	vTaskResume(Alert_Handler);
	vTaskResume(One_Time_Handler);
	if(Assist_Control == ASSIST_ON)
	{
		vTaskResume(Pc_Simv_Assist_ON_Handler);
		vTaskResume(PIP_Control_Task_Handler);
		vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	}
	else if(Assist_Control == ASSIST_OFF)
	{
		Pressure_Mode_blower_control.PEEP_BLOWER_DAC_VAL      = DAC_VAL(Common_Mode_Parameter._PEEP_Val);
		Pressure_Mode_blower_control.LAST_PEEP_BLOWER_DAC_VAL = Pressure_Mode_blower_control.PEEP_BLOWER_DAC_VAL;
		vTaskResume(Pc_Simv_Assist_OFF_Handler);
		vTaskResume(PIP_Control_Task_Handler);
		vTaskSuspend(Pc_Simv_Assist_ON_Handler);
		Trigger_Process = ONE_TIME_TRIGGER;
	}
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}


void VC_SIMV_Initial_Flags_SET()
{
	Reset_Blower_Status();
	TIME_Base_parameter._EXPIRATION_TIME                  = 0;
	LED_STATUS                                            = Idle_LED;
	APRV_current_breathe                                  = APRV_IDLE;
	APRV_Next_Breathe                                     = APRV_Idle;
	Total_Volume_Achieved_Time                            = 500;
	Common_Mode_Parameter._PEEP_Val                       = VC_SIMV.PEEP_Val;
	Common_Mode_Parameter._RESPIRATORY_RATE_Val           = VC_SIMV.RESPIRATORY_RATE_Val;
	Common_Mode_Parameter._FIO2_Val                       = VC_SIMV.FIO2_Val;
	Common_Mode_Parameter._VOLUME_Val                     = VC_SIMV.VOLUME_Val;
	Common_Mode_Parameter._FLOW_RATE                      = VC_SIMV.FLOW_RATE_Val;
	Peep_Status                   				          = PEEP_NOT_ACHEIVED;
	Expiratory_Valve_Lock_Delay   			              = OPENED;
	PEEP_Maintain_Parameter.PEEP_Cumulative_Val           = 1;
	PEEP_Maintain_Parameter.PEEP_Event_Count              = 1;
	PEEP_Maintain_Parameter.PEEP_Temp                     = 3;
	PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time    = 0;

	LOW_COMPLIANCE_DAC                                         = LOW_Compliance_DAC_value(Common_Mode_Parameter._FLOW_RATE);
	Volume_Mode_Blower_control.PREVIOUS_INITIAL_BLOWER_DAC_VAL = LOW_COMPLIANCE_DAC;


	Volume_control_Task.Volume_Stop_Value     = 10;
	INCREMENT_DAC_VAL_PER_ITERATION           = 3;
	Fio2_Blending_Method = VOLUME_CONTROLLED;
	O2_Initial_Parameter(VC_SIMV.FIO2_Val);
	Alert_Flags_Reset();
	Next_Half_Breath_Cycle                                = Generate_Expiration_Cycle;
	Run_Current_Breathe_State                             = Run_Expiration_Cycle;
	Back_Up_Mode_Initial_Flag_Set   = CLOSE;
	Mode_Initial_Flag_Set           = CLOSE;
	Reset_Patient_Circuit_Diconnceted_Alert_Flags();
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);
	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(PSV_Handler);
	vTaskSuspend(PSV_PID_Handler);
	vTaskSuspend(CPAP_Handler);
	vTaskSuspend(BIPAP_Handler);
	vTaskSuspend(BIPAP_PID_Handler);
	vTaskSuspend(APRV_Handler);
	vTaskSuspend(APRV_ONE_TIME_Handler);
	vTaskResume(Alert_Handler);
	vTaskResume(One_Time_Handler);
	if(Assist_Control == ASSIST_ON)
	{
		vTaskResume(Vc_Simv_Assist_ON_Handler);
		vTaskResume(Volume_Control_Task_Handler);
		vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	}
	else if(Assist_Control == ASSIST_OFF)
	{
		Volume_Mode_Blower_control.PEEP_BLOWER_DAC_VAL      = DAC_VAL(Common_Mode_Parameter._PEEP_Val);
		Volume_Mode_Blower_control.LAST_PEEP_BLOWER_DAC_VAL = Volume_Mode_Blower_control.PEEP_BLOWER_DAC_VAL;
		vTaskResume(Vc_Simv_Assist_OFF_Handler);
		vTaskResume(Volume_Control_Task_Handler);
		vTaskSuspend(Vc_Simv_Assist_ON_Handler);
		Trigger_Process = ONE_TIME_TRIGGER;
	}
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}



void PSV_Initial_Flags_SET()
{
	APRV_current_breathe                   = APRV_IDLE;
	APRV_Next_Breathe                      = APRV_Idle;
	PSV_Control.PSV_PEEP_Settle_Time       = 2000;
	Common_Mode_Parameter.TOLERANCE_EWAVE  = PSV.TOLERANCE_EWAVE ;
	Common_Mode_Parameter.TRIG_WINDOW      = PSV.TRIG_WINDOW;
	Common_Mode_Parameter.TRIG_LMT         = PSV.TRIG_LMT;
	Common_Mode_Parameter.TRIG_TIME        = PSV.TRIG_TIME;
	Parkar_valve_Signal(0);
	Oxygen_Parameter.Servo_Position        =45;
	Servo_Info(Oxygen_Parameter.Servo_Position);
	Servo_Position_Feedback                = SERVO_FULL_OPEND;
	Servo_Angle(Oxygen_Parameter.Servo_Position);
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(Alert_Handler);
	vTaskSuspend(One_Time_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(CPAP_Handler);
	vTaskSuspend(BIPAP_Handler);
	vTaskSuspend(BIPAP_PID_Handler);
	vTaskSuspend(APRV_Handler);
	vTaskSuspend(APRV_ONE_TIME_Handler);
	Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL = DAC_VAL(PSV.PRESSURE_SUPPORT_Val );
	PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL           = DAC_VAL(PSV.PEEP_Val );
	PSV_Mode_Blower_control.LAST_PEEP_BLOWER_DAC_VAL      = PSV_Mode_Blower_control.PEEP_BLOWER_DAC_VAL;
	PSV_Pip_average.PRESSURE_SUPPORT_AVG_VAL              = PSV.PRESSURE_SUPPORT_Val;
	PSV_Peep_Average.PEEP_AVG_VAL                         = PSV.PEEP_Val;
	Reset_Patient_Circuit_Diconnceted_Alert_Flags();
	vTaskResume(PSV_Handler);
	Back_Up_Mode_Initial_Flag_Set   = CLOSE;
	Mode_Initial_Flag_Set           = CLOSE;
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}



void CPAP_Initial_Flags_SET()
{
	APRV_current_breathe                   = APRV_IDLE;
	APRV_Next_Breathe                      = APRV_Idle;
	CPAP_Control.CPAP_Settle_Time          = 2000;
	Common_Mode_Parameter.TOLERANCE_EWAVE  = CPAP.TOLERANCE_EWAVE ;
	Common_Mode_Parameter.TRIG_WINDOW      = CPAP.TRIG_WINDOW;
	Common_Mode_Parameter.TRIG_LMT         = CPAP.TRIG_LMT;
	Common_Mode_Parameter.TRIG_TIME        = CPAP.TRIG_TIME;
	Patient_Trigger_Count                  = 0;
	Parkar_valve_Signal(0);
	Oxygen_Parameter.Servo_Position=45;
	Servo_Info(Oxygen_Parameter.Servo_Position);
	Servo_Position_Feedback = SERVO_FULL_OPEND;
	Servo_Angle(Oxygen_Parameter.Servo_Position);
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(Alert_Handler);
	vTaskSuspend(One_Time_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(PSV_Handler);
	vTaskSuspend(PSV_PID_Handler);
	vTaskSuspend(BIPAP_Handler);
	vTaskSuspend(BIPAP_PID_Handler);
	vTaskSuspend(APRV_Handler);
	vTaskSuspend(APRV_ONE_TIME_Handler);
	CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL           = DAC_VAL(CPAP.CPAP_Val );
	CPAP_Mode_Blower_control.LAST_CPAP_BLOWER_DAC_VAL      = CPAP_Mode_Blower_control.CPAP_BLOWER_DAC_VAL;
	CPAP_Average.CPAP_AVG_VAL                              = CPAP.CPAP_Val;
	Reset_Patient_Circuit_Diconnceted_Alert_Flags();
	vTaskResume(CPAP_Handler);
	Back_Up_Mode_Initial_Flag_Set   = CLOSE;
	Mode_Initial_Flag_Set           = CLOSE;
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}



void BIBAP_Initial_Flags_SET()
{
	APRV_current_breathe                   = APRV_IDLE;
	APRV_Next_Breathe                      = APRV_Idle;
	BIPAP_Control.BIPAP_EPAP_Settle_Time   = 2000;
	Common_Mode_Parameter.TOLERANCE_EWAVE  = BIPAP.TOLERANCE_EWAVE ;
	Common_Mode_Parameter.TRIG_WINDOW      = BIPAP.TRIG_WINDOW;
	Common_Mode_Parameter.TRIG_LMT         = BIPAP.TRIG_LMT;
	Common_Mode_Parameter.TRIG_TIME        = BIPAP.TRIG_TIME;
	Parkar_valve_Signal(0);
	Oxygen_Parameter.Servo_Position=45;
	Servo_Info(Oxygen_Parameter.Servo_Position);
	Servo_Position_Feedback = SERVO_FULL_OPEND;
	Servo_Angle(Oxygen_Parameter.Servo_Position);
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(Alert_Handler);
	vTaskSuspend(One_Time_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(PSV_Handler);
	vTaskSuspend(PSV_PID_Handler);
	vTaskSuspend(CPAP_Handler);
	vTaskSuspend(APRV_Handler);
	vTaskSuspend(APRV_ONE_TIME_Handler);
	Pressure_Mode_blower_control.BLOWER_ENDING_DAC_SIGNAL   = DAC_VAL(BIPAP.IPAP_Val );
	BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL           = DAC_VAL(BIPAP.EPAP_Val );
	BIPAP_Mode_Blower_control.LAST_EPAP_BLOWER_DAC_VAL      = BIPAP_Mode_Blower_control.EPAP_BLOWER_DAC_VAL;
	BIPAP_Pip_average.IPAP_AVG_VAL                          = BIPAP.IPAP_Val;
	BIPAP_Peep_Average.EPAP_AVG_VAL                         = BIPAP.EPAP_Val;
	Reset_Patient_Circuit_Diconnceted_Alert_Flags();
	vTaskResume(BIPAP_Handler);
	Back_Up_Mode_Initial_Flag_Set   = CLOSE;
	Mode_Initial_Flag_Set           = CLOSE;
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}



void APRV_Initial_Flags_SET()
{
	LED_STATUS = Idle_LED;
	Common_Mode_Parameter._PIP_Val                     = APRV.P_HIGH;
	Common_Mode_Parameter._PEEP_Val                    = APRV.P_LOW;
	Common_Mode_Parameter._FIO2_Val                    = APRV.FIO2_VAL;
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(Alert_Handler);
	vTaskSuspend(One_Time_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(PSV_Handler);
	vTaskSuspend(PSV_PID_Handler);
	vTaskSuspend(CPAP_Handler);
	APRV_PIP_average.PIP_Cumulative_Val                 = 1;
	APRV_PIP_average.PIP_Event_Count                    = 1;
	APRV_PEEP_average.PEEP_Cumulative_Val               = 1;
	APRV_PEEP_average.PEEP_Event_Count                  = 1;

	APRV_Next_Breathe                                   = APRV_EXPIRATION;
	APRV_current_breathe                                = APRV_EXP;

	APRV.T_LOW_VAL_COUNTER                              = APRV.T_LOW_VAL;



	PEEP_Maintain_Parameter.PEEP_Temp                   = APRV.P_LOW;
	PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time  =1;
	APRV_mode_blower_control.BLOWER_DAC_VAL = DAC_VAL(APRV.P_HIGH );



	Fio2_Blending_Method  = PRESSURE_CONTROLLED;
	O2_Initial_Parameter(APRV.FIO2_VAL);

	vTaskResume(APRV_ONE_TIME_Handler);
	vTaskResume(APRV_Handler);
	Back_Up_Mode_Initial_Flag_Set   = CLOSE;
	Mode_Initial_Flag_Set           = CLOSE;
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}


void Backup_PC_CMV_Initial_Flags_SET()
{
	Common_Mode_Parameter._PIP_Val                        = BACKUP_PC_CMV.PIP_Val;
	Common_Mode_Parameter._PEEP_Val                       = BACKUP_PC_CMV.PEEP_Val;
	Common_Mode_Parameter._RESPIRATORY_RATE_Val           = BACKUP_PC_CMV.RESPIRATORY_RATE_Val;
	Common_Mode_Parameter._FIO2_Val                       = BACKUP_PC_CMV.FIO2_Val;
	Reset_PC_CMV_BACKUP_Old_Flags_Reset();
    if(  (OLD.Last_RR_Val_PC_CMV_BACKUP != BACKUP_PC_CMV.RESPIRATORY_RATE_Val) || (OLD.Last_PIP_Val_PC_CMV_BACKUP    != BACKUP_PC_CMV.PIP_Val)
    		                                                                   || (OLD.Last_Rise_Time_PC_CMV_BACKUP  != BACKUP_PC_CMV.Rise_Time )
																		       || (OLD.Last_I_Val_PC_CMV_BACKUP      != BACKUP_PC_CMV.Inspiration_Time_Ratio)
																		       || (OLD.Last_E_Val_PC_CMV_BACKUP      != BACKUP_PC_CMV.Expiration_Time_Ratio)
																			   || (OLD.Last_PEEP_Val_PC_CMV_BACKUP   != BACKUP_PC_CMV.PEEP_Val)
																			   || (OLD.Last_O2_Val_PC_CMV_BACKUP     != BACKUP_PC_CMV.FIO2_Val ))
    {
    	BACKUP_PC_CMV.RISE_TIME_MS_Val       = ((float)BACKUP_PC_CMV.Rise_Time/100.00)*(BACKUP_PC_CMV.INSPIRATION_TIME) ;
    	if(BACKUP_PC_CMV.INSPIRATION_TIME >600)
    	{
    		if(BACKUP_PC_CMV.RISE_TIME_MS_Val < 600)
    			BACKUP_PC_CMV.RISE_TIME_MS_Val = 600;
    	}

    	PIP_control_Task.PIP_Acheived_Time_Ms          =  (BACKUP_PC_CMV.RISE_TIME_MS_Val * 2);
    	OLD.Last_RR_Val_PC_CMV_BACKUP                  = BACKUP_PC_CMV.RESPIRATORY_RATE_Val;
    	OLD.Last_PIP_Val_PC_CMV_BACKUP                 = BACKUP_PC_CMV.PIP_Val;
    	OLD.Last_Rise_Time_PC_CMV_BACKUP               = BACKUP_PC_CMV.Rise_Time;
    	OLD.Last_I_Val_PC_CMV_BACKUP                   = BACKUP_PC_CMV.Inspiration_Time_Ratio;
    	OLD.Last_E_Val_PC_CMV_BACKUP                   = BACKUP_PC_CMV.Expiration_Time_Ratio;
    	OLD.Last_PEEP_Val_PC_CMV_BACKUP                = BACKUP_PC_CMV.PEEP_Val;
    	OLD.Last_O2_Val_PC_CMV_BACKUP                  = BACKUP_PC_CMV.FIO2_Val;
    	OLD.LAST_BLOWER_ENDING_DAC_SIGNAL              =  DAC_VAL(BACKUP_PC_CMV.PIP_Val);
    	if( (Common_Mode_Parameter._PIP_Val < 30) && (Common_Mode_Parameter._PEEP_Val < 9) )
    	{
    		OLD.LAST_PIP_Control_Task_Delay            = 10;
    		OLD.LAST_Increase_Each_DAC_SET_CONST       = 50;
    	}
    	else
    	{
    		OLD.LAST_PIP_Control_Task_Delay           = 2;
    		OLD.LAST_Increase_Each_DAC_SET_CONST      = 140;
    	}

    	if( (Common_Mode_Parameter._PIP_Val < 30) && (Common_Mode_Parameter._PEEP_Val < 9) )
    	{
    		BACKUP_PC_CMV.RISE_TIME_MS_Val = 800;
    	}
    	else if((Common_Mode_Parameter._PEEP_Val < 9))
    	{
    		BACKUP_PC_CMV.RISE_TIME_MS_Val = 600;
    	}
    	OLD.LAST_Need_PID_Task_Delay                   =  NO;
    	OLD.LAST_Tune_PID_Task_Delay                   =  T_OPEN;
    	OLD.LAST_Low_PIP_Count                         =  RESET;
    	OLD.LAST_Early_Acheived_Ramp_Time_Percentage   =  RESET;
    	OLD.Last_Ramp_Time_PC_CMV_BACKUP               =  BACKUP_PC_CMV.RISE_TIME_MS_Val;
		PIP_Average_Parameter.PIP_Cumulative_Val       = 1;
		PIP_Average_Parameter.PIP_Event_Count          = 1;
		PEEP_Maintain_Parameter.PEEP_Cumulative_Val    = 1;
		PEEP_Maintain_Parameter.PEEP_Event_Count       = 1;
	    PEEP_Maintain_Parameter.PEEP_AVG_VAL           = 1;
	    PEEP_Maintain_Parameter.PEEP_Temp              = (BACKUP_PC_CMV.PEEP_Val / 2);
	    PEEP_Maintain_Parameter.Expiratory_Valve_Open_Time  = 0;
		Smooth_Ramp_Time_Val_Count                     = 0;
		Smooth_Ramp_Time_Val                           = 15;
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
    	Fio2_Blending_Method                           = PRESSURE_CONTROLLED;
    	O2_Initial_Parameter(BACKUP_PC_CMV.FIO2_Val);
		Backup_PC_CMV_SET_VALUE = CHANGE_HAPPEN;

    }
    else
    {
    	OLD.LAST_BLOWER_ENDING_DAC_SIGNAL              =  OLD.LAST_BLOWER_ENDING_DAC_SIGNAL;
    	OLD.LAST_PIP_Control_Task_Delay                =  OLD.LAST_PIP_Control_Task_Delay;
    	OLD.LAST_Increase_Each_DAC_SET_CONST           =  OLD.LAST_Increase_Each_DAC_SET_CONST;
    	OLD.LAST_Need_PID_Task_Delay                   =  OLD.LAST_Need_PID_Task_Delay;
    	OLD.LAST_Tune_PID_Task_Delay                   =  OLD.LAST_Tune_PID_Task_Delay;
    	OLD.LAST_Low_PIP_Count                         =  OLD.LAST_Low_PIP_Count;
    	OLD.LAST_Early_Acheived_Ramp_Time_Percentage   =  OLD.LAST_Early_Acheived_Ramp_Time_Percentage;
    	OLD.Last_Ramp_Time_PC_CMV_BACKUP               =  OLD.Last_Ramp_Time_PC_CMV_BACKUP;
    	Backup_PC_CMV_SET_VALUE = NO_CHANGES;
    }
    Alert_Flags_Reset();
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(Alert_Handler);
	vTaskSuspend(One_Time_Handler);
	vTaskSuspend(BACKUP_PC_Handler);

	Back_Up_Mode_Initial_Flag_Set   = CLOSE;
	vTaskSuspend(Mode_initial_Flag_Set_Handler);
}





void Reset_PC_CMV_Old_Flags_Reset()
{
	OLD.Last_PEEP_Val_PC_CMV_BACKUP  = 0;
	OLD.Last_PIP_Val_PC_CMV_BACKUP   = 0;
	OLD.Last_Rise_Time_PC_CMV_BACKUP = 0;
	OLD.Last_O2_Val_PC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_PC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_PC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_PC_CMV_BACKUP     = 0;
	OLD.Last_PEEP_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_FLOW_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_O2_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_VC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_VC_CMV_BACKUP     = 0;
}



void Reset_VC_CMV_Old_Flags_Reset()
{
	OLD.Last_PEEP_Val_PC_CMV_BACKUP  = 0;
	OLD.Last_PIP_Val_PC_CMV_BACKUP   = 0;
	OLD.Last_Rise_Time_PC_CMV_BACKUP = 0;
	OLD.Last_O2_Val_PC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_PC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_PC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_PC_CMV_BACKUP     = 0;
	OLD.Last_PEEP_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_FLOW_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_O2_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_VC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_VC_CMV_BACKUP     = 0;
}



void Reset_PC_SIMV_Old_Flags_Reset()
{
	OLD.Last_PEEP_Val_PC_CMV_BACKUP  = 0;
	OLD.Last_PIP_Val_PC_CMV_BACKUP   = 0;
	OLD.Last_Rise_Time_PC_CMV_BACKUP = 0;
	OLD.Last_O2_Val_PC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_PC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_PC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_PC_CMV_BACKUP     = 0;
	OLD.Last_PEEP_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_FLOW_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_O2_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_VC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_VC_CMV_BACKUP     = 0;
}



void Reset_VC_SIMV_Old_Flags_Reset()
{
	OLD.Last_PEEP_Val_PC_CMV_BACKUP  = 0;
	OLD.Last_PIP_Val_PC_CMV_BACKUP   = 0;
	OLD.Last_Rise_Time_PC_CMV_BACKUP = 0;
	OLD.Last_O2_Val_PC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_PC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_PC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_PC_CMV_BACKUP     = 0;
	OLD.Last_PEEP_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_FLOW_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_O2_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_VC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_VC_CMV_BACKUP     = 0;
	OLD.Last_Volume_Val_VC_CMV_BACKUP= 0;
}


void Reset_PC_CMV_BACKUP_Old_Flags_Reset()
{
	OLD.Last_PEEP_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_FLOW_Val_VC_CMV_BACKUP  = 0;
	OLD.Last_O2_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_RR_Val_VC_CMV_BACKUP    = 0;
	OLD.Last_I_Val_VC_CMV_BACKUP     = 0;
	OLD.Last_E_Val_VC_CMV_BACKUP     = 0;
	OLD.Last_Volume_Val_VC_CMV_BACKUP= 0;
}



void Reset_Patient_Circuit_Diconnceted_Alert_Flags()
{
	PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK             = CHECK;
	Alert_Count.Patient_Circuit_disconnected_Alert_Count = 0;
	Alert_Status                                         = NO_ALERT;
	LED_Alert();
	CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_PATIENT_CIRCUIT_DISCONNECTED);
}
