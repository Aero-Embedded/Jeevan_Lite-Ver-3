/*
 * Calibration.c
 *
 *  Created on: Mar 16, 2023
 *      Author: asus
 */


#include "Calibration.h"

int Count=0;
float Temp_Pressure_Val_1;
float Temp_Pressure_Val_2;
float Total_temp_Pressure_Val;

extern uint16_t Runtime_Pressure_Val_filterd;

extern float O2_Sensor_Maximum_mv;
extern float O2_Sensor_Voltage;
extern int Blower_RPM;
int O2_calib_Count=0;
float O2_Sensor_Maximum_mv_Temp=9.2;
uint8_t Loop;

void Split_Calibration_Packets(CALIBRATION_PACKET * Receive_Graph_Packet)
{
	CALIBRARTION.BLOWER                  =  (Receive_Graph_Packet->BLOWER_CALIBRATION);
	CALIBRARTION.PRESSURE_SENSOR         =  (Receive_Graph_Packet->PRESSURE_SENSOR_CALIBRATION);
	CALIBRARTION.FLOW_SENSOR_7002        =  (Receive_Graph_Packet->FLOW_SENSOR_7002_CALIBRATION);
	CALIBRARTION.VALVE_LEAK_TEST         =  (Receive_Graph_Packet->VALVE_LEAK_TEST_CALIBRATION);
	CALIBRARTION.O2_CHECK                =  (Receive_Graph_Packet->O2_CHECK_CALIBRATION);
	CALIBRARTION.ALARAM_TEST             =  (Receive_Graph_Packet->LED_ALARAM_CALIBRATION);
	CALIBRARTION.BATTERY_TEST            =  (Receive_Graph_Packet->BATTERY_CALIBRATION);
	CALIBRARTION.SENSOR_OFFSET_TEST      =  (Receive_Graph_Packet->SENSOR_OFFSET_CALIBRATION);
	CALIBRARTION.O2_SENSOR_100_PER_TEST  =  (Receive_Graph_Packet->O2_SENSOR_100_PER_CALIBRATE);


	Max_Flow = 0;
	Min_Flow = 0;

	vTaskResume(Calibration_Handler);
}



uint8_t chksum8_cal(const unsigned char *buff, size_t len)
{
    unsigned int sum;
    for ( sum = 0 ; len != 0 ; len-- )
        sum += *(buff++);
    return (uint8_t)sum;
}


void Send_Calibrated_Packet()
{

	SEND_CALIBRATION_PACKET._header = Send_BlueTooth_Calibration_Header;
	SEND_CALIBRATION_PACKET._length = 7;
	SEND_CALIBRATION_PACKET._CRC8   = chksum8_cal((unsigned char*)&SEND_CALIBRATION_PACKET.PASS_FAIL,(SEND_CALIBRATION_PACKET._length-1));
#ifdef	UART_COMMUNICATION_1
	HAL_UART_Transmit_IT(&huart6,(uint8_t*)&SEND_CALIBRATION_PACKET,sizeof(SEND_CALIBRATION_PACKET));
#endif
#ifdef	USB_COMMUNICATION_1
	CDC_Transmit_FS((uint8_t*)&SEND_CALIBRATION_PACKET,sizeof(SEND_CALIBRATION_PACKET));
#endif
	Send_Bluetooth_Data_Farme = SEND_GRAPH_FRAME;

}

void CALIBRATION_Task(void *argument)
{

	while(1)
	{

		if(CALIBRARTION.BLOWER  == START )
		{
			Blower_Calibration();
		}
		else if(CALIBRARTION.PRESSURE_SENSOR == START)
		{
			Pressure_Sensor_Calibration();
		}
		else if(CALIBRARTION.FLOW_SENSOR_7002 == START)
		{
			Flow_Sensor_7002_Calibration();
		}
		else if(CALIBRARTION.VALVE_LEAK_TEST == START)
		{
			Expiratory_Valve_Leak_Calibration();
		}
		else if(CALIBRARTION.O2_CHECK == START)
		{
			Oxygen_blending_Calibration();
		}
		else if(CALIBRARTION.ALARAM_TEST == START)
		{
			Led_Buzzer_Calibration();
		}
		else if(CALIBRARTION.BATTERY_TEST == START)
		{
			Battery_Calibration();
		}
		else if(CALIBRARTION.SENSOR_OFFSET_TEST == START)
		{
			Sensor_Voltage_Transmit();
		}
		else if(CALIBRARTION.O2_SENSOR_100_PER_TEST == START)
		{
			 Oxygen_Parameter.OXYGEN_Event_Count     = 1;
			 Oxygen_Parameter.OXYGEN_Cumulative_Val  = 1;
			 vTaskResume(O2_Calibration_Handler);
		}


		vTaskDelay(2);
	}
}



void Blower_Calibration()
{

	Count++;
    Blower_ON();
    ExpValve_CLOSE();
	Blower_Signal(4095);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);             // start blower Rpm
	SEND_CALIBRATION_PACKET.RESULT              = Pressure_sensor.Pressure_Val;
	 if(Count > 1500)
	 {
		 /*if(Blower_RPM <55000)
		 {
				SEND_CALIBRATION_PACKET.RESULT              = Pressure_sensor.Pressure_Val ;
				Blower_Signal(0);
				ExpValve_OPEN();
				vTaskDelay(2000);
				SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_FAIL ;
				Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
				vTaskDelay(Twenty_MilliSecond_Delay);
				Count                                       = RESET;
				CALIBRARTION.BLOWER                         = STOP;
				vTaskSuspend(Calibration_Handler);
		 }*/
		 if ((Pressure_sensor.Pressure_Val >=60)  )
		 {
				SEND_CALIBRATION_PACKET.RESULT              = Pressure_sensor.Pressure_Val ;
				Blower_Signal(0);
				ExpValve_OPEN();
				vTaskDelay(2000);
				SEND_CALIBRATION_PACKET.PASS_FAIL              = CALIBRATION_PASS ;
				Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
				vTaskDelay(Twenty_MilliSecond_Delay);
				Count                                       = RESET;
				CALIBRARTION.BLOWER                         = STOP;
				vTaskSuspend(Calibration_Handler);

		 }
		 /*else if ((Max_Flow >=60) || ( Blower_RPM > 55000) )
		 {
				SEND_CALIBRATION_PACKET.RESULT              = Pressure_sensor.Pressure_Val ;
				Blower_Signal(0);
				ExpValve_OPEN();
				vTaskDelay(2000);
				SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_PASS ;
				Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
				vTaskDelay(Twenty_MilliSecond_Delay);
				Count                                       = RESET;
				CALIBRARTION.BLOWER                         = STOP;
				vTaskSuspend(Calibration_Handler);

		 }*/
		 else
		 {
				SEND_CALIBRATION_PACKET.RESULT              = Pressure_sensor.Pressure_Val ;
				Blower_Signal(0);
				ExpValve_OPEN();
				vTaskDelay(2000);
				SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_FAIL ;
				Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
				vTaskDelay(Twenty_MilliSecond_Delay);
				Count                                       = RESET;
				CALIBRARTION.BLOWER                         = STOP;
				vTaskSuspend(Calibration_Handler);
		 }

	  }
}



void Pressure_Sensor_Calibration()
{
	if(Pressure_sensor.Pressure_Val<60)
	{
		Count++;
		Blower_ON();
		ExpValve_CLOSE();
		Blower_Signal(4095);
		SEND_CALIBRATION_PACKET.RESULT              = Pressure_sensor.Pressure_Val;
	}
	else
	{
		if ((Pressure_sensor.Pressure_Val >=60) )
		{
		       HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);             // stop blower Rpm
				SEND_CALIBRATION_PACKET.RESULT              = Pressure_sensor.Pressure_Val ;
				Blower_Signal(0);
				ExpValve_OPEN();
				vTaskDelay(2000);
				SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_PASS ;
				Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
				vTaskDelay(Twenty_MilliSecond_Delay);
				Count                                       = RESET;
				CALIBRARTION.PRESSURE_SENSOR                = STOP;
				vTaskSuspend(Calibration_Handler);
		}
		/*else
		{
		    HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);             // stop blower Rpm
			SEND_CALIBRATION_PACKET.RESULT             = Pressure_sensor.Pressure_Val ;
			Blower_Signal(0);
			ExpValve_OPEN();
			vTaskDelay(2000);
			SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_FAIL ;
			Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
			vTaskDelay(Twenty_MilliSecond_Delay);
			Count                                       = RESET;
			CALIBRARTION.PRESSURE_SENSOR                = STOP;
			vTaskSuspend(Calibration_Handler);
		}*/
	 }
	 if(Count > 1500)
	 {
		HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);             // stop blower Rpm
		SEND_CALIBRATION_PACKET.RESULT              = Pressure_sensor.Pressure_Val ;
		Blower_Signal(0);
		ExpValve_OPEN();
		vTaskDelay(2000);
		SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_FAIL ;
		Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
		vTaskDelay(Twenty_MilliSecond_Delay);
		Count                                       = RESET;
		CALIBRARTION.PRESSURE_SENSOR                = STOP;
		vTaskSuspend(Calibration_Handler);
	  }
}


void Flow_Sensor_7002_Calibration()
{
	  if (Flow_Sensor_cal._Flow_Val < 20  )
	  {
		  Count++;
		  Blower_ON();
		  ExpValve_OPEN();
		  Blower_Signal(2000);
		  SEND_CALIBRATION_PACKET.RESULT              = Flow_Sensor_cal._Flow_Val ;
	  }
	  else
	  {
		   SEND_CALIBRATION_PACKET.RESULT              = Flow_Sensor_cal._Flow_Val ;
		   Blower_Signal(0);
		   ExpValve_OPEN();
		   vTaskDelay(2000);
		   SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_PASS ;
		   Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
		   vTaskDelay(Twenty_MilliSecond_Delay);
		   Count                                       = RESET;
		   CALIBRARTION.FLOW_SENSOR_7002               = STOP;
		   vTaskSuspend(Calibration_Handler);
	   }
	   if (Count > 4000)
	   {
			SEND_CALIBRATION_PACKET.RESULT                = Flow_Sensor_cal._Flow_Val ;
			Blower_Signal(0);
			ExpValve_OPEN();
			vTaskDelay(2000);
			SEND_CALIBRATION_PACKET.PASS_FAIL  = CALIBRATION_FAIL ;
			Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
			vTaskDelay(Twenty_MilliSecond_Delay);
			Count                                       = RESET;
			CALIBRARTION.FLOW_SENSOR_7002               = STOP;
			vTaskSuspend(Calibration_Handler);
		}
}



void Expiratory_Valve_Leak_Calibration()
{

	Blower_ON();
	ExpValve_CLOSE();
	Blower_Signal(DAC_SIGNALS.DAC_VALUE_20 );

	vTaskDelay(4000);
	Blower_Signal(0);
	vTaskDelay(2000);
	Temp_Pressure_Val_1 =  Pressure_sensor.Pressure_Val;

	Blower_Signal(0);
	vTaskDelay(8000);
	Temp_Pressure_Val_2 =Pressure_sensor.Pressure_Val;
	ExpValve_OPEN();
	vTaskDelay(2000);

	if(Temp_Pressure_Val_1 < Temp_Pressure_Val_2)
	{
		Temp_Pressure_Val_2=Temp_Pressure_Val_1;

	}
	else
	{
		Total_temp_Pressure_Val = (Temp_Pressure_Val_1-Temp_Pressure_Val_2);
	}


	if(Total_temp_Pressure_Val<=5)
	{
		if(Temp_Pressure_Val_1 > 5)
		{
			SEND_CALIBRATION_PACKET.PASS_FAIL          = CALIBRATION_PASS ;
			SEND_CALIBRATION_PACKET.RESULT             = Total_temp_Pressure_Val ;
			Send_Bluetooth_Data_Farme                  = SEND_CALIBRATION_FRAME;
			vTaskDelay(Twenty_MilliSecond_Delay);
			CALIBRARTION.VALVE_LEAK_TEST               = STOP;
			ExpValve_OPEN();
			vTaskSuspend(Calibration_Handler);
		}
		else
		{
			SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_FAIL ;
			SEND_CALIBRATION_PACKET.RESULT              = Total_temp_Pressure_Val ;
			Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
			vTaskDelay(Twenty_MilliSecond_Delay);
			CALIBRARTION.VALVE_LEAK_TEST                = STOP;
			ExpValve_OPEN();
			vTaskSuspend(Calibration_Handler);
		}
	}
	else
	{
		SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_FAIL ;
		SEND_CALIBRATION_PACKET.RESULT              = Total_temp_Pressure_Val ;
		Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
		vTaskDelay(Twenty_MilliSecond_Delay);
		CALIBRARTION.VALVE_LEAK_TEST                = STOP;
		ExpValve_OPEN();
		vTaskSuspend(Calibration_Handler);
	}
}


void Oxygen_blending_Calibration()
{
	if(Count  <= 7000)
	{
		Count++;

		Oxygen_Parameter.Servo_Position=110;
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Servo_Position_Feedback = SERVO_FULL_CLOSED;
		Servo_Angle(Oxygen_Parameter.Servo_Position);

		O2_Sensor_Voltage = ( (O2_Sensor_Pin_Voltage * 3300) / 4095);


		Parkar_valve_Signal(3100);
		ExpValve_OPEN();

	}
	else if( (Count >= 7000) && (O2_Sensor_Voltage > 500) )
	{
		Parkar_valve_Signal(0);
		ExpValve_OPEN();

		Oxygen_Parameter.Achieved_Oxygen= (uint8_t)(O2_Sensor_Voltage / O2_Sensor_Maximum_mv);
		if(Oxygen_Parameter.Achieved_Oxygen>100)
			Oxygen_Parameter.Achieved_Oxygen=100;

		Oxygen_Parameter.Servo_Position=45;
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Servo_Position_Feedback = SERVO_FULL_OPEND;
		Servo_Angle(Oxygen_Parameter.Servo_Position);




		SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_PASS ;
		SEND_CALIBRATION_PACKET.RESULT              = Oxygen_Parameter.Achieved_Oxygen ;
		Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
		vTaskDelay(Twenty_MilliSecond_Delay);
		Count                                       = RESET;
		CALIBRARTION.O2_CHECK                       = STOP;
		vTaskSuspend(Calibration_Handler);
	 }
	 if( (Count >= 7000) && (O2_Sensor_Voltage < 500) )
	 {
		Parkar_valve_Signal(0);
		ExpValve_OPEN();

		Oxygen_Parameter.Achieved_Oxygen= (uint8_t)(O2_Sensor_Voltage / O2_Sensor_Maximum_mv);

		Oxygen_Parameter.Servo_Position=45;
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Servo_Position_Feedback = SERVO_FULL_OPEND;
		Servo_Angle(Oxygen_Parameter.Servo_Position);

	     Oxygen_Parameter.OXYGEN_Event_Count     = 1;
		 Oxygen_Parameter.OXYGEN_Cumulative_Val  = 1;

		SEND_CALIBRATION_PACKET.PASS_FAIL  = CALIBRATION_FAIL ;
		SEND_CALIBRATION_PACKET.RESULT              = Oxygen_Parameter.Achieved_Oxygen ;
		Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
		vTaskDelay(Twenty_MilliSecond_Delay);
		Count                                       = RESET;
		CALIBRARTION.O2_CHECK                       = STOP;
		vTaskSuspend(Calibration_Handler);
	  }
}



void Led_Buzzer_Calibration()
{

	Oxygen_Parameter.Achieved_Oxygen = 21;
		Red_Led_ON();
		Blue_Led_OFF();
		Green_Led_OFF();
	vTaskDelay(2000);
	    Red_Led_OFF();
		Blue_Led_ON();
		Green_Led_OFF();
	vTaskDelay(2000);
	    Red_Led_OFF();
	 	Blue_Led_OFF();
	 	Green_Led_ON();
	vTaskDelay(2000);
	    Buzzer1_ON();
	vTaskDelay(2000);
	    Buzzer1_ON();
	vTaskDelay(2000);
	    Red_Led_OFF();
		Blue_Led_OFF();
		Green_Led_OFF();
		Buzzer1_OFF();
		Buzzer2_OFF();
	vTaskDelay(400);

    	SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_PASS ;
		Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
		vTaskDelay(Twenty_MilliSecond_Delay);
		CALIBRARTION.ALARAM_TEST                    = STOP;
	    vTaskSuspend(Calibration_Handler);

}



void Battery_Calibration()
{
	if(Battery_Voltage > 50)
	{
		vTaskDelay(2000);
		SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_PASS ;
		SEND_CALIBRATION_PACKET.RESULT              = Battery_Voltage;
		Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
		vTaskDelay(Twenty_MilliSecond_Delay);
		CALIBRARTION.BATTERY_TEST                   = STOP;
		vTaskSuspend(Calibration_Handler);
	}
	else
	{
		Count++;
		if(Count>2500)
		{
			Count=0;
			SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_FAIL ;
			SEND_CALIBRATION_PACKET.RESULT              = Battery_Voltage;
			Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
			vTaskDelay(Twenty_MilliSecond_Delay);
			CALIBRARTION.BATTERY_TEST                   = STOP;
			vTaskSuspend(Calibration_Handler);

		}
	}
}


void Sensor_Voltage_Transmit()
{

	    Pressure_sensor.Pressure_Sensor_Offset_Val   = Runtime_Pressure_Val_filterd;
	    Flow_sensor.AW_Flow_Offset                   = Flow_sensor.AW_flow_milli_volt;

	    SEND_CALIBRATION_PACKET.ERROR_CODE           = Pressure_sensor.Pressure_Sensor_Offset_Val;
	    SEND_CALIBRATION_PACKET.RESULT               = Flow_sensor.AW_Flow_Offset;

    	SEND_CALIBRATION_PACKET.PASS_FAIL            = CALIBRATION_PASS ;
		Send_Bluetooth_Data_Farme                    = SEND_CALIBRATION_FRAME;
		vTaskDelay(Twenty_MilliSecond_Delay);
		CALIBRARTION.SENSOR_OFFSET_TEST              = STOP;
	    vTaskSuspend(Calibration_Handler);

}





void O2_Calibration_Task(void *argument)
{
	while(1)
	{
		Parkar_valve_Signal(1500);
		Oxygen_Parameter.Servo_Position=110;
		Servo_Info(Oxygen_Parameter.Servo_Position);
		Servo_Position_Feedback = SERVO_FULL_CLOSED;
		Servo_Angle(Oxygen_Parameter.Servo_Position);
		O2_calib_Count++;

		if(O2_calib_Count >5000)
		{
			O2_calib_Count  = 0;
			Loop++;

				O2_Sensor_Voltage = ( (O2_Sensor_Pin_Voltage * 3300) / 4095);
				Oxygen_Parameter.OXYGEN_Event_Count ++;
				Oxygen_Parameter.OXYGEN_Cumulative_Val =( Oxygen_Parameter.OXYGEN_Cumulative_Val +((int) O2_Sensor_Voltage) );

				if(Loop >5)
				{
					Loop=0;
					Parkar_valve_Signal(0);
					Oxygen_Parameter.Servo_Position=45;
					Servo_Info(Oxygen_Parameter.Servo_Position);
					Servo_Position_Feedback = SERVO_FULL_OPEND;
					Servo_Angle(Oxygen_Parameter.Servo_Position);

					CALIBRARTION.O2_SENSOR_100_PER_TEST         = 0;
					SEND_CALIBRATION_PACKET.PASS_FAIL           = CALIBRATION_PASS ;
					Send_Bluetooth_Data_Farme                   = SEND_CALIBRATION_FRAME;
					vTaskDelay(Twenty_MilliSecond_Delay);
					CALIBRARTION.O2_SENSOR_100_PER_TEST         = STOP;
					vTaskSuspend(Calibration_Handler);
					vTaskSuspend(O2_Calibration_Handler);
				}
		}
		vTaskDelay(Two_Millisecond_Delay);
	}
}
