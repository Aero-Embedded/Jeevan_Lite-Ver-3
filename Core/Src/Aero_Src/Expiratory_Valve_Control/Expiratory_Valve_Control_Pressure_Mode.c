/*
 * Expiratory_Valve_Control.c
 *
 *  Created on: Feb 2, 2023
 *      Author: asus
 */


#include "Expiratory_Valve_Control_Pressure_Mode.h"


static uint16_t Below_30_RR_Control(uint8_t,uint8_t,uint32_t,uint8_t,uint16_t);
static uint16_t Above_30_RR_Control(uint8_t,uint8_t,uint32_t,uint8_t,uint16_t);


uint16_t PEEP_Control_For_Expiratory_Valve_Open_Time_In_Pressure_Mode(uint8_t Achieved_PEEP,uint8_t Set_Respiratory_Rate,uint32_t Inspiration_Time_in_MS,uint8_t Set_PEEP,uint16_t Valve_Open_Time)
{
	uint16_t Time;
	if(Set_Respiratory_Rate < 30)
	{
		Time = Below_30_RR_Control(Achieved_PEEP,Set_Respiratory_Rate,Inspiration_Time_in_MS,Set_PEEP,Valve_Open_Time);
	}
	else
	{
		Time = Above_30_RR_Control(Achieved_PEEP,Set_Respiratory_Rate,Inspiration_Time_in_MS,Set_PEEP,Valve_Open_Time);
	}
	return Time;
}


static uint16_t Below_30_RR_Control(uint8_t Achieved_PEEP,uint8_t Set_Respiratory_Rate,uint32_t Inspiration_Time_in_MS,uint8_t Set_PEEP,uint16_t Valve_Open_Time)
{
	static uint8_t Wait_Count_1,Wait_Count_2;
	if (Achieved_PEEP >  (Set_PEEP+8) )
	{
		Wait_Count_1++;
		if (Wait_Count_1 > 0)
		{
			Valve_Open_Time = (Valve_Open_Time + 100);
			Wait_Count_1    = RESET;
				if (Valve_Open_Time >= 700)
				{
					Valve_Open_Time                   = 700;
					PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp - 1);
				}
		}
		if (PEEP_Maintain_Parameter.PEEP_Temp <= 3)
			PEEP_Maintain_Parameter.PEEP_Temp = 3;

	}
	else if( (Achieved_PEEP> (Set_PEEP+2))  && (Achieved_PEEP<= (Set_PEEP+8)) )
	{
		Wait_Count_1++;
		if (Wait_Count_1 > 0)
		{
			Valve_Open_Time = Valve_Open_Time + 40;
			Wait_Count_1    = RESET;
				if (Valve_Open_Time >= 700)
				{
					Valve_Open_Time                   = 700;
					PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp - 1);
				}
		}
		if (PEEP_Maintain_Parameter.PEEP_Temp <= 3)
			PEEP_Maintain_Parameter.PEEP_Temp = 3;
	}
	else if( (Achieved_PEEP> Set_PEEP)  && (Achieved_PEEP <= (Set_PEEP+2)) )
	{
		Wait_Count_1++;
		if (Wait_Count_1 > 0)
		{
			Valve_Open_Time = Valve_Open_Time + 10;
			Wait_Count_1    = RESET;
				if (Valve_Open_Time >= 700)
				{
					Valve_Open_Time                   = 700;
					PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp - 1);
				}
		}
		if (PEEP_Maintain_Parameter.PEEP_Temp <= 3)
			PEEP_Maintain_Parameter.PEEP_Temp = 3;
	}
	else if ((Achieved_PEEP == 0 ) )
	{
		Wait_Count_2++;
		if (Wait_Count_2 > 0)
		{
			PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp + 1);
			Valve_Open_Time                   = (Valve_Open_Time - 10);
			Wait_Count_2                      = RESET;
				if ( (Valve_Open_Time < 1) || (Valve_Open_Time > 700) )
				{
					Valve_Open_Time = 1;
				}
		}
		if (PEEP_Maintain_Parameter.PEEP_Temp  > (Set_PEEP + 10) )
			PEEP_Maintain_Parameter.PEEP_Temp  = PEEP_Maintain_Parameter.PEEP_Temp ;

	}
	else if (Achieved_PEEP< (Set_PEEP -8))
	{
		Wait_Count_2++;
		if (Wait_Count_2 > 0)
		{
			Valve_Open_Time = ( Valve_Open_Time - 50);
			Wait_Count_2    = RESET;
				if ((Valve_Open_Time < 1) || (Valve_Open_Time > 700) )
				{
					Valve_Open_Time                   = 1;
					PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp + 1);
				}
		}
		if (PEEP_Maintain_Parameter.PEEP_Temp> (Set_PEEP + 10) )
			PEEP_Maintain_Parameter.PEEP_Temp =PEEP_Maintain_Parameter.PEEP_Temp;
	}
	else if (( Achieved_PEEP >= (Set_PEEP -8)) && (Achieved_PEEP< (Set_PEEP-2)))
	{
		Wait_Count_2++;
		if (Wait_Count_2 > 0)
		{
			Valve_Open_Time = (Valve_Open_Time - 30);
			Wait_Count_2    = RESET;
				if (Valve_Open_Time< 1 || Valve_Open_Time > 700)
				{
					Valve_Open_Time                   = 1;
					PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp + 1);
				}
		}
		if (PEEP_Maintain_Parameter.PEEP_Temp > (Set_PEEP + 10) )
			PEEP_Maintain_Parameter.PEEP_Temp = PEEP_Maintain_Parameter.PEEP_Temp;

	}
	else if ((Achieved_PEEP>= (Set_PEEP -2)) && (Achieved_PEEP< (Set_PEEP)))
	{
		Wait_Count_2++;
		if (Wait_Count_2 > 0)
		{

			Valve_Open_Time = (Valve_Open_Time - 10);
			Wait_Count_2    = RESET;
				if ( (Valve_Open_Time < 1) || (Valve_Open_Time> 700) )
				{
					PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp + 1);
					Valve_Open_Time                   = 1;
				}
		}
		if (PEEP_Maintain_Parameter.PEEP_Temp > (Set_PEEP + 10) )
			PEEP_Maintain_Parameter.PEEP_Temp = PEEP_Maintain_Parameter.PEEP_Temp;
	}
	else
		PEEP_Maintain_Parameter.PEEP_Temp = PEEP_Maintain_Parameter.PEEP_Temp;

	return Valve_Open_Time;
}



static uint16_t  Above_30_RR_Control(uint8_t Achieved_PEEP,uint8_t Set_Respiratory_Rate,uint32_t Inspiration_Time_in_MS,uint8_t Set_PEEP,uint16_t Valve_Open_Time)
{
	static uint8_t Wait_Count_1,Wait_Count_2;
	if ( (Inspiration_Time_in_MS <= 700))
		Achieved_PEEP = PEEP_Maintain_Parameter.Maximum_PEEP_Acheived;
	PEEP_Maintain_Parameter.Maximum_PEEP_Acheived =0;

		if (Achieved_PEEP> (Set_PEEP +8) )
		{
			Wait_Count_1++;
			if (Wait_Count_1 > 2)
			{
				Valve_Open_Time = (Valve_Open_Time + 100);
				Wait_Count_1    = RESET;
					if (Valve_Open_Time > 700)
					{
						Valve_Open_Time                   = 700;
						PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp - 1);
					}
			}
			if (PEEP_Maintain_Parameter.PEEP_Temp <= 3)
				PEEP_Maintain_Parameter.PEEP_Temp = 3;
		}
		else if( (Achieved_PEEP> (Set_PEEP+2))  && (Achieved_PEEP<= (Set_PEEP+8)) )
		{
			Wait_Count_1++;
			if (Wait_Count_1 > 2)
			{
				Valve_Open_Time = (Valve_Open_Time + 40);
				Wait_Count_1    = RESET;
					if (Valve_Open_Time > 700)
					{
						Valve_Open_Time                   = 700;
						PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp - 1);
					}
			}
			if (PEEP_Maintain_Parameter.PEEP_Temp <= 3)
				PEEP_Maintain_Parameter.PEEP_Temp = 3;
		}


		else if( (Achieved_PEEP> (Set_PEEP))  && (Achieved_PEEP<= (Set_PEEP+2)) )
		{
			Wait_Count_1++;
			if (Wait_Count_1 > 2)
			{

				Valve_Open_Time = (Valve_Open_Time + 10);
				Wait_Count_1    = RESET;
					if (Valve_Open_Time > 700)
					{
						Valve_Open_Time                   = 700;
						PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp - 1);
					}
			}
			if (PEEP_Maintain_Parameter.PEEP_Temp <= 3)
				PEEP_Maintain_Parameter.PEEP_Temp = 3;
		}
		else if ((Achieved_PEEP== 0 ) )
		{
			Wait_Count_2++;
			if (Wait_Count_2 > 2)
			{
				PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp + 1);
				Valve_Open_Time                   =( Valve_Open_Time - 10);
				Wait_Count_2 = RESET;
					if (Valve_Open_Time < 1 || Valve_Open_Time > 700)
					{
						Valve_Open_Time = 1;

					}
			}
			if (PEEP_Maintain_Parameter.PEEP_Temp > (Set_PEEP + 10))
				PEEP_Maintain_Parameter.PEEP_Temp = PEEP_Maintain_Parameter.PEEP_Temp;
		}
		else if ((Achieved_PEEP< (Set_PEEP -8)))
		{
			Wait_Count_2++;
			if (Wait_Count_2 > 2)
			{
				Valve_Open_Time = (Valve_Open_Time - 50);
				Wait_Count_2    = RESET;
					if (Valve_Open_Time < 1 || Valve_Open_Time > 700)
					{
						Valve_Open_Time                   = 1;
						PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp + 1);
					}
			}
			if (PEEP_Maintain_Parameter.PEEP_Temp > Set_PEEP + 10)
				PEEP_Maintain_Parameter.PEEP_Temp = PEEP_Maintain_Parameter.PEEP_Temp;
		}
		else if ((Achieved_PEEP>= (Set_PEEP -8)) && (Achieved_PEEP< (Set_PEEP-2)))
		{
			Wait_Count_2++;
			if (Wait_Count_2 > 2)
			{
				Valve_Open_Time = (Valve_Open_Time - 30);
				Wait_Count_2    = RESET;
					if (Valve_Open_Time < 1 || Valve_Open_Time > 700)
					{
						Valve_Open_Time                   = 1;
						PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp + 1);
					}
			}
			if (PEEP_Maintain_Parameter.PEEP_Temp > (Set_PEEP + 10))
				PEEP_Maintain_Parameter.PEEP_Temp = PEEP_Maintain_Parameter.PEEP_Temp;
		}
		else if ((Achieved_PEEP>= (Set_PEEP -2)) && (Achieved_PEEP< (Set_PEEP)))
		{
			Wait_Count_2++;
			if (Wait_Count_2 > 2)
			{
				Valve_Open_Time = Valve_Open_Time - 10;
				Wait_Count_2    = RESET;
					if (Valve_Open_Time < 1 || Valve_Open_Time > 700)
					{
						PEEP_Maintain_Parameter.PEEP_Temp = (PEEP_Maintain_Parameter.PEEP_Temp + 1);
						Valve_Open_Time                   = 1;
					}
			}
			if (PEEP_Maintain_Parameter.PEEP_Temp > (Set_PEEP + 10))
				PEEP_Maintain_Parameter.PEEP_Temp = PEEP_Maintain_Parameter.PEEP_Temp;
		}

		else
			PEEP_Maintain_Parameter.PEEP_Temp = PEEP_Maintain_Parameter.PEEP_Temp;


		return Valve_Open_Time;
}
