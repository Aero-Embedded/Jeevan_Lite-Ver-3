/*
 * LED_WS2812.c
 *
 *  Created on: Jul 26, 2023
 *      Author: asus
 */


#include "LED_WS2812.h"




#define MAX_LED 10
#define PI 3.14159265

uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4];  // for brightness
int datasentflag=0;

uint16_t pwmData[(24*MAX_LED)+50];


void LED_Strip_Task(void *argument)
{
	while(1)
	{
		if(Receive_Led_Signal==1)
		{
			Toggle_Led();
		}
		else if( (LED_STATUS == INSPIRATION_lED) || (LED_STATUS == EXPIRATION_LED) )
		{
			switch (LED_STATUS)
			{
				case INSPIRATION_lED:
					Green_Led();
				break;
				case EXPIRATION_LED:
					Pink_Led();
				break;
				case Idle_LED:

				break;
			}

		}
		else
		{
			switch (Run_Current_Breathe_State)
			{
				case Run_Inspiration_Cycle:
					PC_CMV_Inspiration_Time_LED_Function(Led_Delay);
				break;
				case Run_Expiration_Cycle:
					PC_CMV_Expiration_Time_LED_Function(Led_Delay);
				break;
				case No_Run_State:
					Initial_Led();
				break;
				default:
				break;
			}
		}
		vTaskDelay(Two_Millisecond_Delay);
	}

}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim4, TIM_CHANNEL_3);
	datasentflag=1;
}


void Set_LED (int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}



void Set_Brightness (int brightness)  // 0-45
{


	if (brightness > 45) brightness = 45;
	for (int i=0; i<MAX_LED; i++)
	{
		LED_Mod[i][0] = LED_Data[i][0];
		for (int j=1; j<4; j++)
		{
			float angle = 90-brightness;  // in degrees
			angle = angle*PI / 180;  // in rad
			LED_Mod[i][j] = (LED_Data[i][j])/(tan(angle));
		}
	}



}


void WS2812_Send (void)
{
	uint32_t indx=0;
	uint32_t color;

	for (int i= 0; i<MAX_LED; i++)
	{
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | (LED_Mod[i][3]));
		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 70;  // 2/3 of 90
			}

			else pwmData[indx] = 35;  // 1/3 of 90

			indx++;
		}

	}

	for (int i=0; i<50; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *)pwmData, indx);
	/*if(!datasentflag)
	{
		vTaskDelay(1);
	}
	else
	{
		datasentflag = 0;
	}*/
}



void Initial_Led()
{
	 switch(Bluetooth_Status)
	 {
	      case BlueTooth_Wait_Time:
	    	  Loading_Led();

	      break;
	      case Bluetooth_Status_Init:
	    	  Blue_Led();

		  break;
	      case Bluetooth_Status_Send_Data:

		  break;
	      default:
	      break;
	 }

	 if(Device_Control == PAUSE)
	 {
		 if(Led_Pause_Status == 1)
		 {
			 Blue_Led_Resume();
			 Led_Pause_Status = 0;
		 }
	 }
}


void Blue_Led_Resume()
{
	  Set_LED(0, 0, 0, 255);
	  Set_LED(1, 0, 0, 255);
	  Set_LED(2, 0, 0, 255);
	  Set_LED(3, 0, 0, 255);
	  Set_LED(4, 0, 0, 255);
	  Set_LED(5, 0, 0, 255);
	  Set_LED(6, 0, 0, 255);
	  Set_LED(7, 0, 0, 255);
	  Set_LED(8, 0, 0, 255);
	  Set_LED(9, 0, 0, 255);

	  Set_Brightness(45);
	  WS2812_Send();
}

void Blue_Led()
{
	  Set_LED(0, 0, 0, 255);
	  Set_LED(1, 0, 0, 255);
	  Set_LED(2, 0, 0, 255);
	  Set_LED(3, 0, 0, 255);
	  Set_LED(4, 0, 0, 255);
	  Set_LED(5, 0, 0, 255);
	  Set_LED(6, 0, 0, 255);
	  Set_LED(7, 0, 0, 255);
	  Set_LED(8, 0, 0, 255);
	  Set_LED(9, 0, 0, 255);

	  Set_Brightness(45);
	  WS2812_Send();
	  vTaskDelay(100);
}

void Loading_Led()
{
	  Set_LED(0, 0, 0, 255);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 255);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 255);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);


	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 255);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);


	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 255);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);


	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 255);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 255);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 255);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 255);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);
}



void PC_CMV_Inspiration_Time_LED_Function(float Delay)
{

	if(Alert_Status_Led_Strip == ALERT_HAPPEN)
	{
		  Set_LED(0, 255, 0, 0);
		  Set_LED(1, 255, 0, 0);
		  Set_LED(2, 255, 0, 0);
		  Set_LED(3, 255, 0, 0);
		  Set_LED(4, 255, 0, 0);
		  Set_LED(5, 255, 0, 0);
		  Set_LED(6, 255, 0, 0);
		  Set_LED(7, 255, 0, 0);
		  Set_LED(8, 255, 0, 0);
		  Set_LED(9, 255, 0, 0);
		  Set_Brightness(20);
		  WS2812_Send();
		  vTaskDelay (300);

		  Set_LED(0, 255, 0, 0);
		  Set_LED(1, 255, 0, 0);
		  Set_LED(2, 255, 0, 0);
		  Set_LED(3, 255, 0, 0);
		  Set_LED(4, 255, 0, 0);
		  Set_LED(5, 255, 0, 0);
		  Set_LED(6, 255, 0, 0);
		  Set_LED(7, 255, 0, 0);
		  Set_LED(8, 255, 0, 0);
		  Set_LED(9, 255, 0, 0);
		  Set_Brightness(20);
		  WS2812_Send();
		  vTaskDelay (300);

		  Alert_Status_Led_Strip = NO_ALERT;
		  vTaskDelay (100);
	}


	  Set_LED(0, 0, 255, 0);
	  Set_LED(1, 0, 255, 0);
	  Set_LED(2, 0, 255, 0);
	  Set_LED(3, 0, 255, 0);
	  Set_LED(4, 0, 255, 0);
	  Set_LED(5, 0, 255, 0);
	  Set_LED(6, 0, 255, 0);
	  Set_LED(7, 0, 255, 0);
	  Set_LED(8, 0, 255, 0);
	  Set_LED(9, 0, 255, 0);
	  WS2812_Send();


		  if(LED_ITER<45)
		  {
			  LED_ITER++;
			  Set_Brightness(LED_ITER);
			  WS2812_Send();
			  vTaskDelay (Delay);
			  if(LED_ITER>=45)
			  {
				  LED_ITER=45;
			  }
		  }


}


void PC_CMV_Expiration_Time_LED_Function(float Delay)
{
	  Set_LED(0, 0, 255, 0);
	  Set_LED(1, 0, 255, 0);
	  Set_LED(2, 0, 255, 0);
	  Set_LED(3, 0, 255, 0);
	  Set_LED(4, 0, 255, 0);
	  Set_LED(5, 0, 255, 0);
	  Set_LED(6, 0, 255, 0);
	  Set_LED(7, 0, 255, 0);
	  Set_LED(8, 0, 255, 0);
	  Set_LED(9, 0, 255, 0);

	  WS2812_Send();

	  if(LED_ITER>1)
	  {
		  LED_ITER--;
		  Set_Brightness(LED_ITER);
		  WS2812_Send();
		  vTaskDelay (Delay);
		  if(LED_ITER<=1)
		  {
			  LED_ITER=1;
		  }
	  }
}


void Toggle_Led()
{
	  Set_LED(0, 0, 0, 255);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(10);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 255);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(10);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 255);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(10);


	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 255);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(10);


	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 255);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(10);


	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 255);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(10);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 255);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(100);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 255);
	  Set_LED(9, 0, 0, 0);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(10);

	  Set_LED(0, 0, 0, 0);
	  Set_LED(1, 0, 0, 0);
	  Set_LED(2, 0, 0, 0);
	  Set_LED(3, 0, 0, 0);
	  Set_LED(4, 0, 0, 0);
	  Set_LED(5, 0, 0, 0);
	  Set_LED(6, 0, 0, 0);
	  Set_LED(7, 0, 0, 0);
	  Set_LED(8, 0, 0, 0);
	  Set_LED(9, 0, 0, 255);
	  Set_Brightness(15);
	  WS2812_Send();
	  vTaskDelay(10);

	  Receive_Led_Signal=0;
}



void Green_Led()
{


	  Set_LED(0, 0, 255, 0);
	  Set_LED(1, 0, 255, 0);
	  Set_LED(2, 0, 255, 0);
	  Set_LED(3, 0, 255, 0);
	  Set_LED(4, 0, 255, 0);
	  Set_LED(5, 0, 255, 0);
	  Set_LED(6, 0, 255, 0);
	  Set_LED(7, 0, 255, 0);
	  Set_LED(8, 0, 255, 0);
	  Set_LED(9, 0, 255, 0);

	  Set_Brightness(15);
	  WS2812_Send();

}



void Pink_Led()
{
	  Set_LED(0, 255, 51, 221);
	  Set_LED(1, 255, 51, 221);
	  Set_LED(2, 255, 51, 221);
	  Set_LED(3, 255, 51, 221);
	  Set_LED(4, 255, 51, 221);
	  Set_LED(5, 255, 51, 221);
	  Set_LED(6, 255, 51, 221);
	  Set_LED(7, 255, 51, 221);
	  Set_LED(8, 255, 51, 221);
	  Set_LED(9, 255, 51, 221);

	  Set_Brightness(15);
	  WS2812_Send();
}
