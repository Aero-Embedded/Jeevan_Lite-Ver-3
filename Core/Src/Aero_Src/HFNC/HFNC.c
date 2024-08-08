/*
 * HFNC.c
 *
 *  Created on: Mar 15, 2023
 *      Author: asus
 */

#include "HFNC.h"

int O2_supply_error_count;

uint16_t HFNC_DAC_VALUE_TEMP;

static void Check_O2_Supply();
static uint16_t HFNC_TUNE_DAC_VALUE(uint16_t *,uint8_t);

uint8_t FLOW_AVG;


float Cummulative_Flow_Value;
int Flow_count;
uint16_t LAST_HFNC_DAC_VALUE = 1800;


void HFNC_Task (void *argument)
{
	while (1)
	{
		Parkar_valve_Signal( HFNC_DAC_VALUE);
		ExpValve_CLOSE();
		Check_O2_Supply();
		Flow_count++;
		Cummulative_Flow_Value += Flow_Sensor_cal._Flow_Val;

		if(Flow_count >= 3000)
		{
			FLOW_AVG               = ((float)Cummulative_Flow_Value / (float)Flow_count);
			LAST_HFNC_DAC_VALUE    = HFNC_TUNE_DAC_VALUE(&LAST_HFNC_DAC_VALUE, HFNC._Flow_Rate);
			HFNC_DAC_VALUE         = LAST_HFNC_DAC_VALUE;
			Cummulative_Flow_Value = 0;
			Flow_count             = 0;
			Flow_count             = 0;

			Green_Led_ON();
			Blue_Led_OFF();
			Red_Led_OFF();
		}

		vTaskDelay(Two_Millisecond_Delay);

	}

}




void HFNC_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet)
{
	HFNC._Flow_Rate=(Receive_Graph_Packet ->Flow_Limit_FiO2_Flow_Limit);

	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);

	vTaskSuspend(One_Time_Handler);
	vTaskSuspend(Pc_Cmv_Handler);
	vTaskSuspend(PIP_Control_Task_Handler);

	vTaskSuspend(Pc_Simv_Assist_OFF_Handler);
	vTaskSuspend(Pc_Simv_Assist_ON_Handler);

	vTaskSuspend(Vc_Simv_Assist_ON_Handler);
	vTaskSuspend(Vc_Simv_Assist_OFF_Handler);

	vTaskSuspend(BACKUP_PC_Handler);

	vTaskSuspend(PSV_Handler);
	vTaskSuspend(PSV_PID_Handler);

	vTaskSuspend(CPAP_Handler);

	vTaskSuspend(BIPAP_Handler);
	vTaskSuspend(BIPAP_PID_Handler);

	vTaskSuspend(APRV_Handler);
	vTaskSuspend(APRV_ONE_TIME_Handler);

	vTaskSuspend(Nebuliser_Handler);

	vTaskSuspend(Suction_Handler);
	vTaskSuspend(HFNC_Handler);

	ExpValve_OPEN();
	Blower_Signal( 0);
	Parkar_valve_Signal(0);
	Nebuliser_OFF();

	HFNC_DAC_VALUE=0;


#ifdef PID_ON_1
  	 uwTick=0;
  	 HFNC_Dac1=1800;
  	 HFNC_Dac2=3100;
	 kp=10;
	 ki=9;
	 kd=1;
	 Time=1000;
	 Speed=1000;
	 TempSetpoint = HFNC._Flow_Rate;
	 PID(&TPID, &Temp_Flow, &PIDOut, &TempSetpoint, kp, ki, kd, _PID_P_ON_E, _PID_CD_DIRECT);
	 PID_SetMode(&TPID, _PID_MODE_AUTOMATIC);
	 PID_SetSampleTime(&TPID, Time);
	 PID_SetOutputLimits(&TPID, HFNC_Dac1, HFNC_Dac2);
#endif
	Parkar_valve_Signal(0);
	Oxygen_Parameter.Servo_Position=110;
	Servo_Info(Oxygen_Parameter.Servo_Position);
	Servo_Position_Feedback = SERVO_FULL_CLOSED;
	Servo_Angle(Oxygen_Parameter.Servo_Position);

	Flow_count = 0;
	HFNC_DAC_VALUE = HFNC_DAC_Value(HFNC._Flow_Rate);
	LAST_HFNC_DAC_VALUE = HFNC_DAC_VALUE;

	vTaskResume(HFNC_Handler);


}



void Check_O2_Supply()
{
		if(Flow_Sensor_cal._Flow_Val == 0)
		{
			O2_supply_error_count++;

			if(O2_supply_error_count >= 5000)
			{
				Clear_All_Alert_Bits();

				SET_ALERT_BIT(FIRST_FRAME_UN,_ALERT_OXYGEN_SUPPLY_FAILED);
				Send_Bluetooth_Data_Farme = SEND_ALERT_FRAME;

				Red_Led_ON();
				Blue_Led_OFF();
				Green_Led_OFF();

				Flow_count            = 0;
				O2_supply_error_count = 0;
			}
		}
}






static uint16_t HFNC_TUNE_DAC_VALUE(uint16_t *Last_Blower_DAC_Val,uint8_t Set_Flow)
{
	if(  ( FLOW_AVG)  > ( Set_Flow ) && (FLOW_AVG  < (Set_Flow + 2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-1);
	}
	else if(  ( FLOW_AVG)  >= ( Set_Flow + 2 ) && (FLOW_AVG  < (Set_Flow + 4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-2);
	}
	else if(  ( FLOW_AVG)  >= ( Set_Flow + 4 )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val-4);
	}
	else if( (FLOW_AVG  < Set_Flow) && (FLOW_AVG  > (Set_Flow -2) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+1);
	}
	else if(  (FLOW_AVG  <= (Set_Flow-2) ) && (FLOW_AVG  >  (Set_Flow-4) ) )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+2);
	}
	else if(  (FLOW_AVG  <= (Set_Flow-4) )  )
	{
		*Last_Blower_DAC_Val = (*Last_Blower_DAC_Val+4);
	}
	else
	{
		*Last_Blower_DAC_Val = *Last_Blower_DAC_Val;
	}
	return *Last_Blower_DAC_Val;
}



uint16_t HFNC_DAC_Value(uint8_t Set_VAL)
{

		switch(Set_VAL)
		{
		    case 5:
		    	HFNC_DAC_VALUE_TEMP  =  1850;
		    break;
		    case 6:
		    	 HFNC_DAC_VALUE_TEMP  = 1860;
		    break;
		    case 7:
				 HFNC_DAC_VALUE_TEMP  = 1870;
			break;
		    case 8:
				 HFNC_DAC_VALUE_TEMP  = 1880;
			break;
		    case 9:
				 HFNC_DAC_VALUE_TEMP  = 1890;
			break;
		    case 10:
				 HFNC_DAC_VALUE_TEMP  = 1900;
			break;
			case 11:
				 HFNC_DAC_VALUE_TEMP   = 1905;
			break;
			case 12:
				 HFNC_DAC_VALUE_TEMP  = 1910;
			break;
			case 13:
				 HFNC_DAC_VALUE_TEMP  = 1915;
			break;
			case 14:
				 HFNC_DAC_VALUE_TEMP  = 1920;
			break;
			case 15:
				 HFNC_DAC_VALUE_TEMP  = 1920;
			break;
			case 16:
				 HFNC_DAC_VALUE_TEMP  = 1925;
			break;
			case 17:
				 HFNC_DAC_VALUE_TEMP   = 1925;
			break;
			case 18:
				 HFNC_DAC_VALUE_TEMP  = 1925;
			break;
			case 19:
				 HFNC_DAC_VALUE_TEMP  = 1925;
			break;
			case 20:
				 HFNC_DAC_VALUE_TEMP  = 1930;
			break;
			case 21:
				 HFNC_DAC_VALUE_TEMP  = 1935;
			break;
			case 22:
				 HFNC_DAC_VALUE_TEMP  = 1940;
			break;
			case 23:
				 HFNC_DAC_VALUE_TEMP   = 1945;
			break;
			case 24:
				 HFNC_DAC_VALUE_TEMP  = 1950;
			break;
			case 25:
				 HFNC_DAC_VALUE_TEMP  = 1955;
			break;
			case 26:
				 HFNC_DAC_VALUE_TEMP  = 1955;
			break;
			case 27:
				 HFNC_DAC_VALUE_TEMP  = 1955;
			break;
			case 28:
				 HFNC_DAC_VALUE_TEMP  = 1955;
			break;
		    case 29:
				 HFNC_DAC_VALUE_TEMP   = 1955;
			break;
			case 30:
				 HFNC_DAC_VALUE_TEMP  = 1960;
			break;
			case 31:
				 HFNC_DAC_VALUE_TEMP  = 1965;
			break;
			case 32:
				 HFNC_DAC_VALUE_TEMP  = 1970;
			break;
			case 33:
				 HFNC_DAC_VALUE_TEMP  = 1975;
			break;
			case 34:
				 HFNC_DAC_VALUE_TEMP  = 1980;
			break;
			case 35:
				 HFNC_DAC_VALUE_TEMP   = 1985;
			break;
			case 36:
				 HFNC_DAC_VALUE_TEMP  = 1985;
			break;
			case 37:
				 HFNC_DAC_VALUE_TEMP  = 1985;
			break;
			case 38:
				 HFNC_DAC_VALUE_TEMP  = 1985;
			break;
			case 39:
				 HFNC_DAC_VALUE_TEMP  = 1985;
			break;
			case 40:
				 HFNC_DAC_VALUE_TEMP  = 2000;
			break;
			case 41:
				 HFNC_DAC_VALUE_TEMP   = 2005;
			break;
			case 42:
				 HFNC_DAC_VALUE_TEMP  = 2010;
			break;
			case 43:
				 HFNC_DAC_VALUE_TEMP  = 2015;
			break;
			case 44:
				 HFNC_DAC_VALUE_TEMP  = 2020;
			break;
			case 45:
				 HFNC_DAC_VALUE_TEMP  = 2025;
			break;
			case 46:
				 HFNC_DAC_VALUE_TEMP  = 2025;
			break;
			case 47:
				 HFNC_DAC_VALUE_TEMP   = 2025;
			break;
			case 48:
				 HFNC_DAC_VALUE_TEMP  = 2025;
			break;
			case 49:
				 HFNC_DAC_VALUE_TEMP  = 2025;
			break;
			case 50:
				 HFNC_DAC_VALUE_TEMP  = 2030;
			break;
			case 51:
				 HFNC_DAC_VALUE_TEMP  = 2035;
			break;
			case 52:
				 HFNC_DAC_VALUE_TEMP  = 2040;
			break;
		    case 53:
				 HFNC_DAC_VALUE_TEMP   = 2045;
			break;
			case 54:
				 HFNC_DAC_VALUE_TEMP  = 2050;
			break;
			case 55:
				 HFNC_DAC_VALUE_TEMP  = 2055;
			break;
			case 56:
				 HFNC_DAC_VALUE_TEMP  = 2055;
			break;
			case 57:
				 HFNC_DAC_VALUE_TEMP  = 2055;
			break;
			case 58:
				 HFNC_DAC_VALUE_TEMP  = 2055;
			break;
			case 59:
				 HFNC_DAC_VALUE_TEMP  = 2055;
			break;
			case 60:
				 HFNC_DAC_VALUE_TEMP  = 2060;
			break;
		}

		return HFNC_DAC_VALUE_TEMP;
	}
