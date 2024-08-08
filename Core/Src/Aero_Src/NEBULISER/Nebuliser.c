/*
 * Nebuliser.c
 *
 *  Created on: Mar 14, 2023
 *      Author: asus
 */


#include "Nebuliser.h"




uint8_t Suction_Started = 0;


void Split_Nebuliser_Packets(NEBULISER_PACKET * Receive_Graph_Packet)
{
	Nebuliser.NEBULISER_ON_Val           = (0X80 & (Receive_Graph_Packet->NEBULISER)) >>7;
	Nebuliser.NEBULISER_SYNC_ON_Val      = (0X40 & ( Receive_Graph_Packet->NEBULISER)) >>6;
	Nebuliser.NEBULISER_TIME             = (0X3F & (Receive_Graph_Packet->NEBULISER)) * 60000;
	Nebuliser.NEBULISER_TIMER            = Nebuliser.NEBULISER_TIME;

	Nebuliser.SUCTION                    = (0X08 & ( Receive_Graph_Packet->MANUOVER)) >> 3;


	 if(Nebuliser.NEBULISER_ON_Val==1)
	 {
		 SET_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_ON);
		 CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_OFF);
		 vTaskResume(Nebuliser_Handler);
	 }
	 else if(Nebuliser.NEBULISER_ON_Val==0)
	 {
     	 SET_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_OFF);
     	 CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_ON);
		 Nebuliser.NEBULISER_TIMER = 0;
		 vTaskResume(Nebuliser_Handler);
	 }


	 if(Nebuliser.SUCTION  == 1)
	 {
		 Suction_Started  = 1;
		 SUCTION_Packet_Data();
	 }
	 else if( (Nebuliser.SUCTION  == 0) && (Suction_Started == 1) )
	 {
		 Suction_Started   = 0;
		 Blower_Signal(0);
		 ExpValve_OPEN();
		 vTaskSuspend(Suction_Handler);
	 }




}


void Nebuliser_Task (void *argument)
{
	while(1)
	{
		if(Nebuliser.NEBULISER_TIMER>0)
		{
			Nebuliser_Func();
		}
		else
		{
			Nebuliser_OFF();
		}
		vTaskDelay(Ten_Millisecond_Delay);
	}
}



void Nebuliser_Func()
{

	    if(Running_Mode  == Aprv)
	    {
	        if(Nebuliser.NEBULISER_ON_Val==1)
	        {
	        	if(Nebuliser.NEBULISER_SYNC_ON_Val==0)
	        	{
	        		Nebuliser_ON();
	        	}
	        	else
	        	{
	        		if(APRV_current_breathe      == APRV_INS)
	        		{
	        			Nebuliser_ON();
	        		}
	                else if(APRV_current_breathe == APRV_EXP)
	                {
	                	Nebuliser_OFF();
	                }
	         	}
	        }
	        else
	        {
	        	Nebuliser.NEBULISER_TIMER = 0;
	        	Nebuliser_OFF();
	        	vTaskSuspend(Nebuliser_Handler);
	        }
	    }

	    else
	    {
	        if(Nebuliser.NEBULISER_ON_Val==1)
	        {
	        	if(Nebuliser.NEBULISER_SYNC_ON_Val==0)
	        	{
	        		Nebuliser_ON();
	        	}
	        	else
	        	{
	        		if(Run_Current_Breathe_State      == Run_Inspiration_Cycle)
	        		{
	        			Nebuliser_ON();
	        		}
	                else if(Run_Current_Breathe_State == Run_Expiration_Cycle)
	                {
	                	Nebuliser_OFF();
	                }
	         	}
	        }
	        else
	        {
	        	SET_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_OFF);
	        	CLEAR_ALERT_BIT(FIRST_FRAME_UN, _ALERT_NEBULISER_ON);
	        	Nebuliser.NEBULISER_TIMER = 0;
	        	Nebuliser_OFF();
	        	vTaskSuspend(Nebuliser_Handler);
	        }
	    }

}


uint8_t chksum8_cal_neb(const unsigned char *buff, size_t len)
{
    unsigned int sum;
    for ( sum = 0 ; len != 0 ; len-- )
        sum += *(buff++);
    return (uint8_t)sum;
}






