/*
 * Oxygen_Blending.c
 *
 *  Created on: Feb 8, 2023
 *      Author: asus
 */


#include "Oxygen_Blending.h"


float O2_Sensor_Voltage;
float O2_Sensor_Maximum_mv=9.2;
uint16_t O2_DAC_VALUE=0;


uint32_t O2_Flow_Event_Count;
uint32_t O2_Flow_Cumulative_Val;




void Oxygen_Blending_Task(void *argument)
{
	while(1)
	{
			Oxygen_Blending(Common_Mode_Parameter._FIO2_Val);
			vTaskDelay(Two_Millisecond_Delay);

	}
}


void Oxygen_Blending(uint8_t FIO2_Val)
{
	if(Run_Current_Breathe_State == Run_Inspiration_Cycle)
	{
		if(Fio2_Blending_Method == PRESSURE_CONTROLLED)
		{
			if(Pressure_sensor.Pressure_Val >= Common_Mode_Parameter._PIP_Val )
			{
				Parkar_valve_Signal(0);
				Oxygen_Blending_Status=OXYGEN_ACHIEVED;
			}

		}
		else if(Fio2_Blending_Method == VOLUME_CONTROLLED)
		{
			if(vol.Volume >= (Common_Mode_Parameter._VOLUME_Val-Volume_control_Task.Volume_Stop_Value))
			{
				Parkar_valve_Signal(0);
				Oxygen_Blending_Status=OXYGEN_ACHIEVED;
			}
		}



	}
	else if(Run_Current_Breathe_State == Run_Expiration_Cycle)
	{
		 Parkar_valve_Signal(0);
	}
	if(Oxygen_Blending_Status == OXYGEN_NOT_ACHIEVED)
	{
		O2_Flow_Event_Count ++;
		O2_Flow_Cumulative_Val =( O2_Flow_Cumulative_Val +((int) O2_Flow_Sensor_Voltage) );
	}
}



void Adjust_Servo_And_Calculate_O2(uint8_t FIO2_Val)
{
			if(FIO2_Val > 21)
			{

				O2_Flow_Val = ((float)O2_Flow_Cumulative_Val/O2_Flow_Event_Count);
				O2_Flow_Event_Count =1;
				O2_Flow_Cumulative_Val=1;

				Oxygen_Parameter.OXYGEN_AVG_VAL = ((float)Oxygen_Parameter.OXYGEN_Cumulative_Val/(float)Oxygen_Parameter.OXYGEN_Event_Count);
				Oxygen_Parameter.Achieved_Oxygen= (uint8_t)(Oxygen_Parameter.OXYGEN_AVG_VAL / O2_Sensor_Maximum_mv);

				if(Oxygen_Parameter.Achieved_Oxygen > 100)
					Oxygen_Parameter.Achieved_Oxygen=100;

				if(Oxygen_Parameter.Achieved_Oxygen < 21)
					Oxygen_Parameter.Achieved_Oxygen=21;

				Oxygen_Parameter.OXYGEN_Event_Count     = 10;
				Oxygen_Parameter.OXYGEN_Cumulative_Val  = 10;

					if(Oxygen_Parameter.Achieved_Oxygen < (FIO2_Val-3))
					{
						Oxygen_Parameter.Servo_Move_Forward++;
							if(Oxygen_Parameter.Servo_Move_Forward > 3)
							{
								Oxygen_Parameter.Servo_Position=Oxygen_Parameter.Servo_Position+5;
								Servo_Info(Oxygen_Parameter.Servo_Position);


								Oxygen_Parameter.Servo_Move_Forward=0;
							}

						if(Common_Mode_Parameter._FIO2_Val != 100)
						{
							if(Oxygen_Parameter.Servo_Position >= SERVO_FULL_CLOSED )
							{
								Oxygen_Parameter.Servo_Position=90;
								Servo_Info(Oxygen_Parameter.Servo_Position);
								Servo_Position_Feedback = SERVO_FULL_CLOSED;
							}
						}
					}
					else if(Oxygen_Parameter.Achieved_Oxygen>(FIO2_Val+3))
					{
						Oxygen_Parameter.Servo_Move_Backward++;
							if(Oxygen_Parameter.Servo_Move_Backward > 3)
							{
								Oxygen_Parameter.Servo_Position=Oxygen_Parameter.Servo_Position-5;
								Servo_Info(Oxygen_Parameter.Servo_Position);
								Oxygen_Parameter.Servo_Move_Backward=0;
							}
							if(Common_Mode_Parameter._FIO2_Val != 100)
							{
								if(Oxygen_Parameter.Servo_Position <= SERVO_FULL_OPEND)
								{
									Oxygen_Parameter.Servo_Position=45;
									Servo_Info(Oxygen_Parameter.Servo_Position);
									Servo_Position_Feedback = SERVO_FULL_OPEND;
								}
							}
					}
			}
			else
			{
				Oxygen_Parameter.Servo_Position=45;
				Servo_Info(Oxygen_Parameter.Servo_Position);
				Oxygen_Parameter.O2_DAC=0;
				Parkar_valve_Signal(0);
			}
			Servo_Angle(Oxygen_Parameter.Servo_Position);
}

void Get_Oxygen_Value(uint8_t FIO2_Val)
{
	if(Run_Current_Breathe_State == Run_Inspiration_Cycle)
	{
		if (FIO2_Val > 21)
		{
			O2_Sensor_Voltage = ( (O2_Sensor_Pin_Voltage * 3300) / 4095);
			Oxygen_Parameter.OXYGEN_Event_Count ++;
			Oxygen_Parameter.OXYGEN_Cumulative_Val =( Oxygen_Parameter.OXYGEN_Cumulative_Val +((int) O2_Sensor_Voltage) );
		}
	}
	O2_Flow_Sensor_Voltage = ( ((Adc.AdcData[5]) * 3300) / 4095);
}

void O2_Initial_Parameter(uint8_t FIO2_Val)
{
	        Oxygen_Parameter.OXYGEN_Event_Count     = 1;
			Oxygen_Parameter.OXYGEN_Cumulative_Val  = 1;

			if(Fio2_Blending_Method   == PRESSURE_CONTROLLED)
			{
			    Oxygen_Parameter.O2_DAC=Set_Initial_O2_DAC_VAL_Pressure_Modes(FIO2_Val);
			    Servo_Position_Pressure_Controlled_Mode(FIO2_Val);
			}
			else if(Fio2_Blending_Method   == VOLUME_CONTROLLED)
			{
			    Oxygen_Parameter.O2_DAC=Set_Initial_O2_DAC_VAL_Volume_Modes(FIO2_Val);
			    Servo_Position_Volume_Controlled_Mode(FIO2_Val);
			}


}



void Servo_Info(uint8_t Servo_Position)
{
	Oxygen_Parameter.Servo_Duty_Cycle  = ( ((Servo_Position-45) * 0.07)  + 5);
	Oxygen_Parameter.Servo_Millisecond = ( ((Servo_Position-45) * 0.015) + 1);
}




void O2_DAC_Control(uint8_t FIO2_Val)
{
	if(FIO2_Val  > 21)
	{
		if(Alert_Check <= 4)
		{
			   if (Oxygen_Parameter.Achieved_Oxygen < (FIO2_Val  -2 ))
				{
					Low_Oxygen_Achieved(FIO2_Val);
				}
				else if (Oxygen_Parameter.Achieved_Oxygen > (FIO2_Val+2))
				{
					High_Oxygen_Achieved(FIO2_Val);
				}
				else
					Parkar_valve_Signal(Oxygen_Parameter.O2_DAC);
				Oxygen_Parameter.Parker_Signal_Voltage = ( (Oxygen_Parameter.O2_DAC * 3.3) /4095);
				Oxygen_Blending_Status = OXYGEN_NOT_ACHIEVED;
				Parkar_valve_Signal(Oxygen_Parameter.O2_DAC);
		}
	}
	else
	{
		Parkar_valve_Signal(0);
	}
}

void Low_Oxygen_Achieved(uint8_t FIO2_Val)
{
	uint8_t Low_Achieved_Oxygen_In_Percentage;
	Low_Achieved_Oxygen_In_Percentage= (uint8_t) ( ( (float)Oxygen_Parameter.Achieved_Oxygen  /(float)FIO2_Val) * 100);
	if( (Low_Achieved_Oxygen_In_Percentage <= 70) )
	{
		Oxygen_Parameter.O2_DAC = Oxygen_Parameter.O2_DAC + 60;
	}
	if( (Low_Achieved_Oxygen_In_Percentage <= 80) && (Low_Achieved_Oxygen_In_Percentage >70) )
	{
		Oxygen_Parameter.O2_DAC = Oxygen_Parameter.O2_DAC + 40;
	}
	else if(Low_Achieved_Oxygen_In_Percentage>80)
	{
		Oxygen_Parameter.O2_DAC = Oxygen_Parameter.O2_DAC + 20;
	}


	if(Oxygen_Parameter.O2_DAC>=2000)
	{
		Oxygen_Parameter.O2_DAC=2000;
	}
	Parkar_valve_Signal(Oxygen_Parameter.O2_DAC);
}


void High_Oxygen_Achieved(uint8_t FIO2_Val)
{
	uint8_t High_Achieved_Oxygen_In_Percentage;
	High_Achieved_Oxygen_In_Percentage = (uint8_t) (( (float) Oxygen_Parameter.Achieved_Oxygen  /(float)FIO2_Val) * 100);

	if(High_Achieved_Oxygen_In_Percentage >= 130)
	{
		Oxygen_Parameter.O2_DAC = Oxygen_Parameter.O2_DAC - 60;
	}
	if( (High_Achieved_Oxygen_In_Percentage >= 120) && (High_Achieved_Oxygen_In_Percentage < 130) )
	{
		Oxygen_Parameter.O2_DAC = Oxygen_Parameter.O2_DAC - 40;
	}
	else if(High_Achieved_Oxygen_In_Percentage < 120)
	{
		Oxygen_Parameter.O2_DAC = Oxygen_Parameter.O2_DAC - 20;
	}


	if(Oxygen_Parameter.O2_DAC<=200)
	{
		Oxygen_Parameter.O2_DAC=200;
	}
	Parkar_valve_Signal(Oxygen_Parameter.O2_DAC);
}




uint16_t Set_Initial_O2_DAC_VAL_Pressure_Modes(uint8_t FIO2_Val)
{
		switch(FIO2_Val)
		{
			case 25:
				 O2_DAC_VALUE  = 250;
			break;
			case 30:
				 O2_DAC_VALUE  = 280;
			break;
			case 35:
				 O2_DAC_VALUE  = 330;
			break;
			case 40:
				 O2_DAC_VALUE  = 380;
			break;
			case 45:
				 O2_DAC_VALUE  = 410;
			break;
			case 50:
				 O2_DAC_VALUE  = 450;
			break;
			case 55:
				 O2_DAC_VALUE  = 500;
			break;
			case 60:
				 O2_DAC_VALUE  = 550;
			break;
			case 65:
				 O2_DAC_VALUE  = 600;
			break;
			case 70:
				 O2_DAC_VALUE  = 650;
			break;
			case 75:
				 O2_DAC_VALUE  = 700;
			break;
			case 80:
				 O2_DAC_VALUE  = 750;
			break;
			case 85:
				 O2_DAC_VALUE  = 800;
			break;
			case 90:
				 O2_DAC_VALUE  = 850;
			break;
			case 95:
				 O2_DAC_VALUE  = 900;
			break;
			case 100:
				 O2_DAC_VALUE  = 1800;
			break;
		}
		return O2_DAC_VALUE;
}





uint16_t Set_Initial_O2_DAC_VAL_Volume_Modes(uint8_t FIO2_Val)
{
	switch(FIO2_Val)
	{
		case 25:
			 O2_DAC_VALUE  = 280;
		break;
		case 30:
			 O2_DAC_VALUE  = 300;
		break;
		case 35:
			 O2_DAC_VALUE  = 400;
		break;
		case 40:
			 O2_DAC_VALUE  = 430;
		break;
		case 45:
			 O2_DAC_VALUE  = 450;
		break;
		case 50:
			 O2_DAC_VALUE  = 480;
		break;
		case 55:
			 O2_DAC_VALUE  = 510;
		break;
		case 60:
			 O2_DAC_VALUE  = 540;
		break;
		case 65:
			 O2_DAC_VALUE  = 560;
		break;
		case 70:
			 O2_DAC_VALUE  = 590;
		break;
		case 75:
			 O2_DAC_VALUE  = 620;
		break;
		case 80:
			 O2_DAC_VALUE  = 650;
		break;
		case 85:
			 O2_DAC_VALUE  = 680;
		break;
		case 90:
			 O2_DAC_VALUE  = 800;
		break;
		case 95:
			 O2_DAC_VALUE  = 850;
		break;
		case 100:
			 O2_DAC_VALUE  = 1800;
		break;
	}

		return O2_DAC_VALUE;
}



void Servo_Position_Pressure_Controlled_Mode(uint8_t FIO2_Val)
{
	if(FIO2_Val==100)
	{
		Oxygen_Parameter.Servo_Position=110;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		Servo_Info(Oxygen_Parameter.Servo_Position);
		vTaskResume(Oxygen_Blending_Handler);

	}
	else if( (FIO2_Val>=70) && (FIO2_Val<95) )
	{
		Oxygen_Parameter.Servo_Position=75;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		Servo_Info(Oxygen_Parameter.Servo_Position);
		vTaskResume(Oxygen_Blending_Handler);
	}
	else if((FIO2_Val>21) && (FIO2_Val<70) )
	{
		Oxygen_Parameter.Servo_Position=45;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		Servo_Info(Oxygen_Parameter.Servo_Position);
		vTaskResume(Oxygen_Blending_Handler);
	}
	else if(FIO2_Val<=21)
	{
		Oxygen_Parameter.Servo_Position=45;
		Oxygen_Parameter.Achieved_Oxygen=21;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Parkar_valve_Signal(0);
		vTaskSuspend(Oxygen_Blending_Handler);
	}
}


void Servo_Position_Volume_Controlled_Mode(uint8_t FIO2_Val)
{
	if(FIO2_Val==100)
	{
		Oxygen_Parameter.Servo_Position=110;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		Servo_Info(Oxygen_Parameter.Servo_Position);
		vTaskResume(Oxygen_Blending_Handler);

	}
	else if( (FIO2_Val>=70) && (FIO2_Val<95) )
	{
		Oxygen_Parameter.Servo_Position=75;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		Servo_Info(Oxygen_Parameter.Servo_Position);
		vTaskResume(Oxygen_Blending_Handler);
	}
	else if((FIO2_Val>21) && (FIO2_Val<70) )
	{
		Oxygen_Parameter.Servo_Position=45;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		Servo_Info(Oxygen_Parameter.Servo_Position);
		vTaskResume(Oxygen_Blending_Handler);
	}
	else if(FIO2_Val<=21)
	{
		Oxygen_Parameter.Servo_Position=45;
		Oxygen_Parameter.Achieved_Oxygen=21;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Parkar_valve_Signal(0);
		vTaskSuspend(Oxygen_Blending_Handler);
	}
}
