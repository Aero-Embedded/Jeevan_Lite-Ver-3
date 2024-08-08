/*
 * Alert.c
 *
 *  Created on: Mar 30, 2022
 *      Author: asus
 */


#include "Alert.h"


uint8_t Achieved_Respiratory_Count=0;
uint8_t Achieved_One_RR=0;
int Total_Tidal_volume,Maximum_Tidal_Volume_Achieved,Achieved_Minite_Volume=0;
int RR_E_TIME_ACHIEVED=0;
int RR_I_TIME_ACHIEVED=0;
int Insp_Volume,Exp_Volume;
int Leak;
uint8_t Oxygen_Supply_Failed = NO;
uint8_t Patient_Ckt_Discon = 0;
uint8_t Proximal_Flow_Sensor_Reverse = 0;
uint8_t Toggle=0,old_State=1,Newstate=0;


/*
 * This Function is used for Receiving Alert Range Parameter values
 */

void Split_Alert_Parameter_Packets(ALERT_RANGE_PACKET *Receive_Graph_Packet)
{
	Alert_Range._RANGE_PIP_MIN_Val      =	(Receive_Graph_Packet->_RANGE_PIP_MIN);
	Alert_Range._RANGE_PIP_MAX_Val	    =	(Receive_Graph_Packet->_RANGE_PIP_MAX);
	Alert_Range._RANGE_VT_MIN_Val	    = 	(Receive_Graph_Packet->_RANGE_VT_MIN);
	Alert_Range._RANGE_VT_MAX_Val	    = 	(Receive_Graph_Packet->_RANGE_VT_MAX);
	Alert_Range._RANGE_RR_MIN_Val       =	(Receive_Graph_Packet->_RANGE_RR_MIN);
	Alert_Range._RANGE_RR_MAX_Val       =	(Receive_Graph_Packet->_RANGE_RR_MAX);
	Alert_Range._RANGE_MINT_VOL_MIN_Val =	(Receive_Graph_Packet->_RANGE_MINT_VOL_MIN);
	Alert_Range._RANGE_MINT_VOL_MAX_Val =	(Receive_Graph_Packet->_RANGE_MINT_VOL_MAX);
	Alert_Range._RANGE_SPO2_MIN_Val     =	(Receive_Graph_Packet->_RANGE_SPO2_MIN);
	Alert_Range._RANGE_PULSE_MAX_Val     =	(Receive_Graph_Packet->_RANGE_PULSE_MAX);
	Alert_Range._RANGE_PULSE_MIN_Val    =	(Receive_Graph_Packet->_RANGE_PULSE_MIN);
}


void SEND_ALERT_PACKET()
{

	DEVICE_ALERT_DATA_SEND._header  = Send_BlueTooth_Alert_Header;
	DEVICE_ALERT_DATA_SEND._length  = 7;
	DEVICE_ALERT_DATA_SEND._CRC8    = chksum8((unsigned char*)&DEVICE_ALERT_DATA_SEND.FIRST_FRAME_UN.FIRST_BYTES,(DEVICE_ALERT_DATA_SEND_LENGTH -1));
#ifdef	UART_COMMUNICATION_1
	HAL_UART_Transmit(&huart6,(uint8_t*)&DEVICE_ALERT_DATA_SEND,sizeof(DEVICE_ALERT_DATA_SEND),300);
#endif
#ifdef	USB_COMMUNICATION_1
	CDC_Transmit_FS((uint8_t*)&DEVICE_ALERT_DATA_SEND,sizeof(DEVICE_ALERT_DATA_SEND));
#endif


}



void Clear_All_Alert_Bits()
{
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_PLUGGED_IN);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_BATTERY_MODE);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PIP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PIP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PEEP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PEEP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_MINUTE_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_MINUTE_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_TIDAL_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_TIDAL_VOLUME);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_HIGH_RR);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_LOW_RR);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_OXYGEN_SUPPLY_FAILED);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_PATIENT_CIRCUIT_DISCONNECTED);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_FLOW_SENSOR_WRONG_DIRECTION);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_HIGH_FIO2);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_LOW_FIO2);
	CLEAR_ALERT_BIT(SECOND_FRAME_UN, _ALERT_LEAK);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_20_PER_BATTERY_DRAIN);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_ON);
    CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_OFF);
}



void Alert_Task (void *argument)
{
	while(1)
	{
		if( (Run_Current_Breathe_State == Run_Inspiration_Cycle)  )
		{
			if(Alert_Check == 1 && (! ((Assist_Control == ASSIST_OFF) && (Patient_Trigger == PATIENT_TRIGGER_HAPPEN) )) )
			{
				Patient_Circuit_Disconnected_Alert();
			}
			Proximal_Flow_Sensor_Reverse_Direction();
		}
		vTaskDelay(Two_Millisecond_Delay);
	}
}



void Alert_Flags_Reset()
{
	Alert_Count.PIP_Alert_Count          = 0;
	Alert_Count.PEEP_Alert_Count         = 0;
	Alert_Count.Tidal_Volume_Alert_Count = 0;
	Achieved_Respiratory_Count           = 0;
	Total_Tidal_volume                   = 0;
	Alert_Count.FIO2_Supply_Alert_Count  = 0;
	Alert_Count.FIO2_Alert_Count         = 0;
	Alert_Count.Leak_Alert_Count         = 0;
	Alert_Count.Patient_Circuit_disconnected_Alert_Count=0;
	Alert_Check=5;
}


void Check_Alert()
{
	Convert_Battery_Percentage();
	if(Patient_Ckt_Discon == 1)
	{
		Hide_Alert_1();
		Patient_Ckt_Discon = 0;
	}
    else if(Proximal_Flow_Sensor_Reverse == 1)
	{
		Hide_Alert_2();
		Proximal_Flow_Sensor_Reverse = 0;
	}
	else
	{
		Pip_Alert(PIP_Average_Parameter.Maximum_PIP_Acheived );
		Peep_Alert(Common_Mode_Parameter._PEEP_Val);
		Tidal_Volume_Alert(vol.Volume_max);
		if(Achieved_Respiratory_Count == Common_Mode_Parameter._RESPIRATORY_RATE_Val)
		{
			Achieved_Minite_Volume         = ((float)Total_Tidal_volume / 1000);
			Total_Tidal_volume             = 0;
			Achieved_Respiratory_Count     = 0;
			Minite_Volume_Alert(Achieved_Minite_Volume);
		}
		if(Alert_Check == 1)
		{
			Respiratory_Rate_Alert();
		}
		if(Common_Mode_Parameter._FIO2_Val>21)
		{
			Oxygen_Supply_Alert(Common_Mode_Parameter._FIO2_Val,Oxygen_Parameter.Achieved_Oxygen);
			Oxygen_Blending_Alert(Common_Mode_Parameter._FIO2_Val,Oxygen_Parameter.Achieved_Oxygen);
		}
		Leak_Alert();
	}
	AC_Supply_or_Battery_Supply_Indication();
	if(old_State != Newstate)
	{
		if(Toggle == 1)
		{
		    SET_ALERT_BIT(FIRST_FRAME_UN, _ALERT_PLUGGED_IN);
			CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_BATTERY_MODE);
		}
		else
		{
			CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_PLUGGED_IN);
			SET_ALERT_BIT(FIRST_FRAME_UN, _ALERT_BATTERY_MODE);
		}
		Newstate = old_State;
	}


}

void Alert_Inspiration_Time_Parameter()
{
	Send_Bluetooth_Data_Farme = SEND_ALERT_FRAME;
	Achieved_Respiratory_Count++;
	Total_Tidal_volume += vol.Volume_max;
	RR_E_TIME_ACHIEVED = TIME_Base_parameter.EXPIRATION_TIME_ACHEIVED ;
	if(Alert_Check>1)
		Alert_Check--;
	Exp_Volume=vol.Volume;
    Exp_Volume=Insp_Volume-Exp_Volume;
	Leak=Insp_Volume-Exp_Volume;
}



void AC_Supply_or_Battery_Supply_Indication()
{
	if((HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2))== 0)
	{
		Toggle = 1;
		old_State=Toggle;
	}
	else
	{
		Toggle = 0;
		old_State = Toggle;
	}
}



void Alert_Expiration_Time_Parameter()
{
	RR_I_TIME_ACHIEVED = TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED ;
	Insp_Volume=vol.Volume;
	Alert_Count.Patient_Circuit_disconnected_Alert_Count=0;
	Alert_Count.Proximal_Flow_Sensor_reversed_Alert_Count=0;
	PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK = CHECK;
	PROXIMAL_SENSOR_REVERSE_ALERT_CHECK = CHECK;

}


void Pip_Alert(uint8_t PIP)
{

	if ( (PIP >= Alert_Range._RANGE_PIP_MIN_Val) && (PIP <= Alert_Range._RANGE_PIP_MAX_Val))
	{
		Alert_Count.PIP_Alert_Count = 0;
	}
	else
	{
		Alert_Count.PIP_Alert_Count++;
		if (Alert_Count.PIP_Alert_Count > 3)
		{
			Alert_Status = ALERT_HAPPEN;

				if (Alert_Range._RANGE_PIP_MIN_Val > PIP)
				{
					SET_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PIP);
					CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PIP);

					Alert_Count.PIP_Alert_Count = 0;
				}
				else if (Alert_Range._RANGE_PIP_MAX_Val < PIP)
				{
					SET_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PIP);
					CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PIP);
					Alert_Count.PIP_Alert_Count = 0;
				}
		}
	}
}


void Peep_Alert(uint8_t PEEP)
{
	uint8_t PEEP_HIGH = (PEEP+3);
	uint8_t PEEP_LOW = (PEEP-3);
	if ((PEEP_Maintain_Parameter.PEEP_AVG_VAL <= PEEP_HIGH)&& (PEEP_Maintain_Parameter.PEEP_AVG_VAL >= PEEP_LOW))
	{
		Alert_Count.PEEP_Alert_Count = 0;
	}
	else
	{
		Alert_Count.PEEP_Alert_Count++;
			if(Alert_Count.PEEP_Alert_Count>3)
			{
				Alert_Status = ALERT_HAPPEN;

				if (PEEP_HIGH > PEEP_Maintain_Parameter.PEEP_AVG_VAL)
				{
					SET_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PEEP);
					CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PEEP);

					Alert_Count.PEEP_Alert_Count = 0;
				}
				else if (PEEP_LOW< PEEP_Maintain_Parameter.PEEP_AVG_VAL)
				{
					SET_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PEEP);
					CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PEEP);
					Alert_Count.PEEP_Alert_Count = 0;
				}
			}
	}
}




void Tidal_Volume_Alert(int Maximum_Volume_Achieved)
{
	if( (Maximum_Volume_Achieved >= Alert_Range._RANGE_VT_MIN_Val)  &&  (Maximum_Volume_Achieved <= Alert_Range._RANGE_VT_MAX_Val))
	{
		Alert_Count.Tidal_Volume_Alert_Count=0;
	}
	else
	{
		Alert_Count.Tidal_Volume_Alert_Count++;
		if(Alert_Count.Tidal_Volume_Alert_Count>3)
		{
			Alert_Status = ALERT_HAPPEN;
			if(Alert_Range._RANGE_VT_MIN_Val>Maximum_Volume_Achieved)
			{
				SET_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_TIDAL_VOLUME);
				CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_TIDAL_VOLUME);

				Alert_Count.Tidal_Volume_Alert_Count=0;
			}
			else if(Alert_Range._RANGE_VT_MAX_Val<Maximum_Volume_Achieved)
			{
				SET_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_TIDAL_VOLUME);
				CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_TIDAL_VOLUME);
				Alert_Count.Tidal_Volume_Alert_Count=0;
			}
		}

	  }
}





void Minite_Volume_Alert(int Achieved_Minite_Volume)
{
	if ((Achieved_Minite_Volume >= Alert_Range._RANGE_MINT_VOL_MIN_Val) && (Achieved_Minite_Volume <= Alert_Range._RANGE_MINT_VOL_MAX_Val))
	{

	}
	else
	{

		Alert_Status = ALERT_HAPPEN;

			if (Alert_Range._RANGE_MINT_VOL_MIN_Val > Achieved_Minite_Volume)
			{
				SET_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_MINUTE_VOLUME);
				CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_MINUTE_VOLUME);


			}
			else if (Alert_Range._RANGE_MINT_VOL_MAX_Val < Achieved_Minite_Volume)
			{
				SET_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_MINUTE_VOLUME);
				CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_MINUTE_VOLUME);
			}
	}

}


void Respiratory_Rate_Alert()
{
	Achieved_One_RR=  (60000/((float)(RR_E_TIME_ACHIEVED + RR_I_TIME_ACHIEVED)));
	if(( Achieved_One_RR >= Alert_Range._RANGE_RR_MIN_Val) && (Achieved_One_RR <=Alert_Range._RANGE_RR_MAX_Val))
	{

	}
	else
	{
		Alert_Status = ALERT_HAPPEN;
		if(Alert_Range._RANGE_RR_MIN_Val>Achieved_One_RR)
		{
			SET_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_LOW_RR);
			CLEAR_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_HIGH_RR);

		}
		else if(Alert_Range._RANGE_RR_MAX_Val<Achieved_One_RR)
		{
			SET_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_HIGH_RR);
			CLEAR_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_LOW_RR);
		}
	}
}





void Oxygen_Supply_Alert(uint8_t FIO2_Val,uint8_t Achieved_Oxygen)
{
	if(Achieved_Oxygen<=22)
	{
		Alert_Count.FIO2_Supply_Alert_Count++;
		if(Alert_Count.FIO2_Supply_Alert_Count>5)
		{
			Alert_Status = ALERT_HAPPEN;
			  SET_ALERT_BIT(FIRST_FRAME_UN,_ALERT_OXYGEN_SUPPLY_FAILED);
			  Alert_Count.FIO2_Supply_Alert_Count=0;
			  Oxygen_Supply_Failed = YES;
		}
	 }
	else
	{
		Alert_Count.FIO2_Supply_Alert_Count=0;
		Oxygen_Supply_Failed = NO;
	}

}

void Oxygen_Blending_Alert(uint8_t FIO2_Val,uint8_t Achieved_Oxygen)
{
	if( Oxygen_Supply_Failed == NO)
	{
			uint8_t FIO2_HIGH = (FIO2_Val+5);
			uint8_t FIO2_LOW = (FIO2_Val-5);
			 if( (Achieved_Oxygen <= FIO2_HIGH) && (Achieved_Oxygen >= FIO2_LOW))
			 {

				Alert_Count.FIO2_Alert_Count=0;
			 }
			 else
			 {

				Alert_Count.FIO2_Alert_Count++;
					if(Alert_Count.FIO2_Alert_Count>3)
					{
						Alert_Status = ALERT_HAPPEN;

						if (Achieved_Oxygen > FIO2_HIGH)
						{
							SET_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_HIGH_FIO2);
							CLEAR_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_LOW_FIO2);
							Alert_Count.FIO2_Alert_Count=0;
						}
						else if (Achieved_Oxygen < FIO2_LOW)
						{
							SET_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_LOW_FIO2);
							CLEAR_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_HIGH_FIO2);
							Alert_Count.FIO2_Alert_Count=0;
						}
					}
			 }
	}

}


void Leak_Alert()
{
	if(0>Leak)
	{
		Alert_Count.Leak_Alert_Count++;
		 if(Alert_Count.Leak_Alert_Count>3)
		 {
			Alert_Status = ALERT_HAPPEN;
		    SET_ALERT_BIT(SECOND_FRAME_UN, _ALERT_LEAK);
		    Alert_Count.Leak_Alert_Count=0;
		  }
	}
	else
	{
		Alert_Count.Leak_Alert_Count=0;
	}
}




void Patient_Circuit_Disconnected_Alert()
{
	if(  (vol.Volume_Val<=0) && (Flow_Sensor_cal._Flow_Val==0 || Flow_Sensor_cal._Flow_Val==-1 || Flow_Sensor_cal._Flow_Val==-2))
	{
			if((DAC1->DHR12R1)>=400)
			{
				if(PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK == CHECK)
				{
					Alert_Count.Patient_Circuit_disconnected_Alert_Count++;
						if(Alert_Count.Patient_Circuit_disconnected_Alert_Count >= 200)
						{
							Alert_Status = ALERT_HAPPEN;
							SET_ALERT_BIT(FIRST_FRAME_UN,_ALERT_PATIENT_CIRCUIT_DISCONNECTED);
							PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK = DO_NOT_CHECK;
							Patient_Ckt_Discon = 1;
						}
				}
			}
	}
	else
	{

		if( (Common_Mode_Parameter._PEEP_Val  ) >= (Pressure_sensor.Pressure_Val ) )
		{
			if(PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK == CHECK)
			{
				Alert_Count.Patient_Circuit_disconnected_Alert_Count++;
					if(Alert_Count.Patient_Circuit_disconnected_Alert_Count >= 200)
					{
						Alert_Status = ALERT_HAPPEN;
						SET_ALERT_BIT(FIRST_FRAME_UN,_ALERT_PATIENT_CIRCUIT_DISCONNECTED);
            			PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK = DO_NOT_CHECK;
						Patient_Ckt_Discon = 1;
					}
			}
		}
	}
}



void Proximal_Flow_Sensor_Reverse_Direction()
{
	if(TIME_Base_parameter.INSPIRATION_TIME_ACHEIVED < 500)
	{
		if((vol.Volume_Val<(-10)))
		{
			if(PROXIMAL_SENSOR_REVERSE_ALERT_CHECK == CHECK)
			{
				Alert_Count.Proximal_Flow_Sensor_reversed_Alert_Count++;
					 if( Alert_Count.Proximal_Flow_Sensor_reversed_Alert_Count>=150)
					 {
						  Alert_Status = ALERT_HAPPEN;
						  SET_ALERT_BIT(FIRST_FRAME_UN,_ALERT_FLOW_SENSOR_WRONG_DIRECTION);
						  PROXIMAL_SENSOR_REVERSE_ALERT_CHECK =  DO_NOT_CHECK;
						  Proximal_Flow_Sensor_Reverse = 1;

					 }
			}

		}
	}
}




void LED_Alert()
{
	if(Alert_Status == ALERT_HAPPEN)
	{
		Alert_Status_Led_Strip = ALERT_HAPPEN;
		Red_Led_ON();
		Blue_Led_OFF();
		Green_Led_OFF();

	}
	else if(Alert_Status == NO_ALERT)
	{
		Green_Led_ON();
		Blue_Led_OFF();
		Red_Led_OFF();
	}
}


void Hide_Alert_1()
{

	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_PLUGGED_IN);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_BATTERY_MODE);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PIP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PIP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PEEP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PEEP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_MINUTE_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_MINUTE_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_TIDAL_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_TIDAL_VOLUME);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_HIGH_RR);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_LOW_RR);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_OXYGEN_SUPPLY_FAILED);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_FLOW_SENSOR_WRONG_DIRECTION);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_HIGH_FIO2);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_LOW_FIO2);
	CLEAR_ALERT_BIT(SECOND_FRAME_UN, _ALERT_LEAK);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_20_PER_BATTERY_DRAIN);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_ON);
    CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_OFF);

}


void Hide_Alert_2()
{
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_PLUGGED_IN);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_BATTERY_MODE);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PIP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PIP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_HIGH_PEEP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN, _ALERT_LOW_PEEP);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_MINUTE_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_MINUTE_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_HIGH_TIDAL_VOLUME);
	CLEAR_ALERT_BIT(THIRD_FRAME_UN,_ALERT_LOW_TIDAL_VOLUME);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_HIGH_RR);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN,_ALERT_LOW_RR);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_OXYGEN_SUPPLY_FAILED);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_PATIENT_CIRCUIT_DISCONNECTED);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_HIGH_FIO2);
	CLEAR_ALERT_BIT(FOURTH_FRAME_UN, _ALERT_LOW_FIO2);
	CLEAR_ALERT_BIT(SECOND_FRAME_UN, _ALERT_LEAK);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_20_PER_BATTERY_DRAIN);
	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_ON);
    CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_OFF);
}
