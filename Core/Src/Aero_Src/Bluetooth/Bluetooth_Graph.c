/*
 * Bluetooth_Graph.c
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */


#include "Bluetooth_Graph.h"


void Bluetooth_Task(void *argument)
{
	while(1)
	{
		 switch(Bluetooth_Status)
		 {
		      case BlueTooth_Wait_Time:
		    	  Bluetooth_Status=Bluetooth_Status_Init;
		    	  Bluetooth_Parameter.Bluetooth_Transmit_Delay=Four_Second_Delay;

		      break;
		      case Bluetooth_Status_Init:
		    	  MX_USB_DEVICE_Init();
		    	  UART6_Init();
		    	  UART5_Init();
		    	  HAL_UART_Receive_IT(&huart6,(uint8_t *) Bluetooth_Parameter.Bluetooth_RX_BUF, sizeof( Bluetooth_Parameter.Bluetooth_RX_BUF));
		    	  Bluetooth_Status                             = Bluetooth_Status_Send_Data;
		    	  Bluetooth_Parameter.Bluetooth_Transmit_Delay = Twenty_MilliSecond_Delay;

			  break;
		      case Bluetooth_Status_Send_Data:
		    	  Bluetooth_Data_Send();
			  break;
		      default:
		      break;
		 }

		 vTaskDelay(Bluetooth_Parameter.Bluetooth_Transmit_Delay);

	}
}


void UART6_Init()
{
	huart6.Instance             = USART6;
	huart6.Init.BaudRate        = 57600;
	huart6.Init.WordLength      = UART_WORDLENGTH_8B;
	huart6.Init.StopBits        = UART_STOPBITS_1;
	huart6.Init.Parity          = UART_PARITY_NONE;
	huart6.Init.Mode            = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling    = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart6) != HAL_OK)
	{
		Error_Handler();
	}
}


void UART5_Init()
{
	huart5.Instance             = UART5;
	huart5.Init.BaudRate        = 57600;
	huart5.Init.WordLength      = UART_WORDLENGTH_8B;
	huart5.Init.StopBits        = UART_STOPBITS_1;
	huart5.Init.Parity          = UART_PARITY_NONE;
	huart5.Init.Mode            = UART_MODE_TX_RX;
	huart5.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
	huart5.Init.OverSampling    = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart5) != HAL_OK)
	{
	   Error_Handler();
	 }
}

uint8_t chksum8(const unsigned char *buff, size_t len)
{
    unsigned int sum;
    for ( sum = 0 ; len != 0 ; len-- )
        sum += *(buff++);
    return (uint8_t)sum;
}

void Bluetooth_Graph_Data_Send()
{
	if( ( !(Device_Control == RESUME))   )
	{
		vol.Volume       = RESET;
		vol.Volume_max   = RESET;
		vol.Volume_Val   = RESET;
	}
	if(LAST_FLOW_TRIGGER != 0)
	{
		Flow_Sensor_cal._Flow_Val=(int)LAST_FLOW_TRIGGER;
	}
	else
	{
		Flow_Sensor_cal._Flow_Val=(int)Flow_Sensor_cal.Flow1;
	}
	Graph_Data._header          = Send_BlueTooth_Graph_Header;
	Graph_Data._length          = sizeof(Graph_Data)-3 ;
	Graph_Data._Pressure_Val    = Pressure_sensor.Pressure_Val;
	Graph_Data._Flow_Val        = Flow_Sensor_cal._Flow_Val;
	Graph_Data._Volume_Val      = vol.Volume_Val;
	Graph_Data._Status          = BREATH_STATE;
	Graph_Data._CRC8            = chksum8(&Graph_Data._Pressure_Val,(Graph_Data._length - 1));
	LAST_FLOW_TRIGGER           = 0;

#ifdef	UART_COMMUNICATION_1
    HAL_UART_Transmit_IT(&huart6,(uint8_t*)&Graph_Data,sizeof(Graph_Data));
#endif
#ifdef	USB_COMMUNICATION_1
    CDC_Transmit_FS((uint8_t*)&Graph_Data,sizeof(Graph_Data));
#endif


#ifdef UART5_GRAPH_DATA_DEBUG_1
		UART5_GRAPH_DATA_DEBUG();
#endif
#ifdef USB_GRAPH_DATA_DEBUG_1
		USB_GRAPH_DATA_DEBUG();
#endif
}


void UART5_GRAPH_DATA_DEBUG()
{
	sprintf(UART_GARAPH_SAMPLES,"%04d,%04d,%04d\r",(int)Pressure_sensor.Pressure_Val,(int)Flow_Sensor_cal._Flow_Val,(int)vol.Volume_Val);
	HAL_UART_Transmit_IT(&huart5,(uint8_t*)UART_GARAPH_SAMPLES,sizeof(UART_GARAPH_SAMPLES));
}

void USB_GRAPH_DATA_DEBUG()
{
	sprintf(USB_GARAPH_SAMPLES,"%04d,%04d,%04d\r",(int)O2_Flow_Val,(int)Oxygen_Parameter.O2_DAC,(int)O2_Flow_Sensor_Voltage);
	CDC_Transmit_FS((uint8_t*)&USB_GARAPH_SAMPLES,sizeof(USB_GARAPH_SAMPLES));
}



void Bluetooth_Data_Send()
{


	switch (Send_Bluetooth_Data_Farme)
	{

	    case SEND_FUELGAUGE_PARAMETER_FRAME:
	    	switch (Send_Frame_Fuel_Gauge)
	    	{
	    	    case  First_Frame:
	    	    	Send_Frame_Fuel_Gauge = Second_Frame;
	    	    	//Fuel_Gauge_Parameter_List_1_Transmit();
	    	    break;

	    	    case  Second_Frame:
	    	    	//Fuel_Gauge_Parameter_List_2_Transmit();
	    	    	Send_Bluetooth_Data_Farme = SEND_GRAPH_FRAME;
	    	    break;

     		    default:
     		    	Send_Bluetooth_Data_Farme = SEND_GRAPH_FRAME;
     		    break;

	    	}
	    break;
        case SEND_SAMPLED_PARAMETER_FRAME:
        		switch (Send_Frame)
			    {
        		    case First_Frame:
        		    	Sampled_Parameter_List_1_Transmit();
        		    	//Send_Frame = Second_Frame;
        		    	Send_Bluetooth_Data_Farme = SEND_GRAPH_FRAME;
        		    break;
        		    case Second_Frame:
        		    	Sampled_Parameter_List_2_Transmit();
        		    	Send_Frame = Third_Frame;
        		    break;
        		    case Third_Frame:
        		    	Sampled_Parameter_List_3_Transmit();
        		    	Send_Frame = Fourth_Frame;
        		    break;
        		    case Fourth_Frame:
        		    	Sampled_Parameter_List_4_Transmit();
        		    	Send_Frame = Fifth_Frame;

        		    break;
        		    case Fifth_Frame:
        		    	Sampled_Parameter_List_5_Transmit();
        		    	//Send_Bluetooth_Data_Farme = SEND_FUELGAUGE_PARAMETER_FRAME;

        		    default:
        		    	//Send_Bluetooth_Data_Farme = SEND_FUELGAUGE_PARAMETER_FRAME;
        		    break;

			    }

        break;
	    case SEND_CALIBRATION_FRAME:
	    	Send_Calibrated_Packet();
	    break;
		case SEND_ALERT_FRAME:
			switch (Apnea_Alert)
			{

				  case APNEA_ALERT_HAPPEN:
					  SEND_ALERT_PACKET();
					  CLEAR_ALERT_BIT(SECOND_FRAME_UN,_ALERT_APNEA);
					  CLEAR_ALERT_BIT(FIRST_FRAME_UN,_ALERT_PATIENT_CIRCUIT_DISCONNECTED);
					  Apnea_Alert=NO_APNEA_ALERT_HAPPEN;
					  Send_Bluetooth_Data_Farme = SEND_GRAPH_FRAME;
				  break;
				  case NO_APNEA_ALERT_HAPPEN:
					switch (Mode)
					{
						case Idle:
							SEND_ALERT_PACKET();
							Send_Bluetooth_Data_Farme = SEND_GRAPH_FRAME;
						break;
						default:
							SEND_ALERT_PACKET();
							Clear_All_Alert_Bits();
							Send_Bluetooth_Data_Farme = SEND_SAMPLED_PARAMETER_FRAME;
						break;

					}
				  break;
			 }


		break;
		case SEND_GRAPH_FRAME:
			Bluetooth_Graph_Data_Send();
		break;
	}
}





void BlueTooth_Receiver_Task(void *argument)
{
	while (1)
	{
		if (xQueueReceive(Bluetooth_Receive, &(Bluetooth_Parameter.Bluetooth_RX_BUF), portMAX_DELAY) == pdTRUE)
		{
			Uart_Receive_Debug_Toggle_Led();
			vTaskResume(Bluetooth_data_Split_Handler);
		}
   }
}



void Bluetooth_data_split_Task (void *argument)
{
	while (1)
	{
		Bluetooth_Packet_Data_Split((RECEIVE_GRAPH_PACKET*) (Bluetooth_Parameter.Bluetooth_RX_BUF));
		Receive_Led_Signal = 1;
		vTaskSuspend(Bluetooth_data_Split_Handler);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	HAL_UART_Receive_IT(&huart6,(uint8_t *) (Bluetooth_Parameter.Bluetooth_RX_BUF), sizeof( Bluetooth_Parameter.Bluetooth_RX_BUF));
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xQueueSendToFrontFromISR(Bluetooth_Receive, &(Bluetooth_Parameter.Bluetooth_RX_BUF), &xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}


