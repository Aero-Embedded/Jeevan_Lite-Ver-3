/*
 * Sampled_Parameter.c
 *
 *  Created on: May 23, 2023
 *      Author: asus
 */


#include "Sampled_Parameter.h"

int V_Min=0;


extern int RR_E_TIME_ACHIEVED;
extern int RR_I_TIME_ACHIEVED;
extern uint8_t Toggle;

void Sampled_Parameter_List_1_Transmit()
{
	Sampled_Parameter_1_Packet._header          = Send_BlueTooth_Sampled_Parameter_Header ;
	Sampled_Parameter_1_Packet._length          = (sizeof(Sampled_Parameter_1_Packet)-3)  ;
	Sampled_Parameter_1_Packet._PIP_Val         = Battery_Voltage ;
	Sampled_Parameter_1_Packet._PEEP_Val        = Toggle;
	Sampled_Parameter_1_Packet._VTI_Val_1       = Sampled_Parameter_2.Total_Tidal_Volume_Int ;
	Sampled_Parameter_1_Packet._VTI_Val_2       = Sampled_Parameter_2.Total_Tidal_Volume_Decimal;
	Sampled_Parameter_1_Packet._FIO2            = Oxygen_Parameter.Achieved_Oxygen ;
	Sampled_Parameter_1_Packet._RR              = Breath_In_Minite ;
	Sampled_Parameter_1_Packet._CRC8            = chksum8(&Sampled_Parameter_1_Packet._PIP_Val,(6));

#ifdef	UART_COMMUNICATION_1
	 HAL_UART_Transmit_IT(&huart6,(uint8_t*)&Sampled_Parameter_1_Packet,sizeof(Sampled_Parameter_1_Packet));
#endif
#ifdef	USB_COMMUNICATION_1
	 CDC_Transmit_FS((uint8_t*)&Sampled_Parameter_1_Packet,sizeof(Sampled_Parameter_1_Packet));
#endif



}



void Sampled_Parameter_List_2_Transmit()
{
	Sampled_Parameter_2_Packet._header          	     = Send_BlueTooth_Sampled_Parameter_Header ;
	Sampled_Parameter_2_Packet._length                   = (sizeof(Sampled_Parameter_2_Packet)-3)+16 ;

	Sampled_Parameter_2_Packet._Minite_Volume_int        = Sampled_Parameter_2.Total_Tidal_Volume_Int;
	Sampled_Parameter_2_Packet._Minite_Volume_float      = Sampled_Parameter_2.Total_Tidal_Volume_Decimal;
	Sampled_Parameter_2_Packet._Static_Compliance_Int    = Sampled_Parameter_3.Staic_Compliance_int;
	Sampled_Parameter_2_Packet._Static_Compliance_Float  = Sampled_Parameter_3.Staic_Compliance_float;
	Sampled_Parameter_2_Packet._Dynamic_Compliance_Int   = Sampled_Parameter_3.Dynamic_Compliance_int;
	Sampled_Parameter_2_Packet._Dynamic_Compliance_Float = Sampled_Parameter_3.Dynamic_Compliance_float;

	Sampled_Parameter_2_Packet._CRC8                     = chksum8(&Sampled_Parameter_2_Packet._Minite_Volume_int,(6));

#ifdef	UART_COMMUNICATION_1
	HAL_UART_Transmit_IT(&huart6,(uint8_t*)&Sampled_Parameter_2_Packet,sizeof(Sampled_Parameter_2_Packet));
#endif
#ifdef	USB_COMMUNICATION_1
	CDC_Transmit_FS((uint8_t*)&Sampled_Parameter_2_Packet,sizeof(Sampled_Parameter_2_Packet));
#endif


}



void Sampled_Parameter_List_3_Transmit()
{
	Sampled_Parameter_3_Packet._header          	     = Send_BlueTooth_Sampled_Parameter_Header ;
	Sampled_Parameter_3_Packet._length                   = (sizeof(Sampled_Parameter_3_Packet)-3)+24 ;

	Sampled_Parameter_3_Packet._RInsp_Int                = Sampled_Parameter_3.Rinsp_int;
	Sampled_Parameter_3_Packet._RInsp_Float1             = Sampled_Parameter_3.Rinsp_float_2;
	Sampled_Parameter_3_Packet._MAP_int                  = Sampled_Parameter_4.MAP_int;
	Sampled_Parameter_3_Packet._MAP_float                = Sampled_Parameter_4.MAP_float;
	Sampled_Parameter_3_Packet._VTE_Val                  = Sampled_Parameter_1.Volume_Max_Exp ;

	Sampled_Parameter_3_Packet._CRC8                     = chksum8(&Sampled_Parameter_3_Packet._RInsp_Int,(6));

#ifdef	UART_COMMUNICATION_1
	 HAL_UART_Transmit_IT(&huart6,(uint8_t*)&Sampled_Parameter_3_Packet,sizeof(Sampled_Parameter_3_Packet));
#endif
#ifdef	USB_COMMUNICATION_1
	 CDC_Transmit_FS((uint8_t*)&Sampled_Parameter_3_Packet,sizeof(Sampled_Parameter_3_Packet));
#endif


}




void Sampled_Parameter_List_4_Transmit()
{
	Sampled_Parameter_4_Packet._header          	     = Send_BlueTooth_Sampled_Parameter_Header ;
	Sampled_Parameter_4_Packet._length                   = (sizeof(Sampled_Parameter_4_Packet)-3)+32 ;


	Sampled_Parameter_4_Packet._PIF_Val                  = Sampled_Parameter_1.MAX_Flow ;
	Sampled_Parameter_4_Packet._PEF_Val                  = Sampled_Parameter_1.MIN_Flow ;
	Sampled_Parameter_4_Packet._I_time_Achieved          = RR_I_TIME_ACHIEVED;
	Sampled_Parameter_4_Packet._E_time_Achieved          = RR_E_TIME_ACHIEVED;
	Sampled_Parameter_4_Packet._CRC8                     = chksum8(&Sampled_Parameter_4_Packet._PIF_Val,(6));

#ifdef	UART_COMMUNICATION_1
	HAL_UART_Transmit_IT(&huart6,(uint8_t*)&Sampled_Parameter_4_Packet,sizeof(Sampled_Parameter_4_Packet));
#endif
#ifdef	USB_COMMUNICATION_1
	CDC_Transmit_FS((uint8_t*)&Sampled_Parameter_4_Packet,sizeof(Sampled_Parameter_4_Packet));
#endif

	Reset_Flag_Func();

}



void Sampled_Parameter_List_5_Transmit()
{
	Sampled_Parameter_5_Packet._header          	     = Send_BlueTooth_Sampled_Parameter_Header ;
	Sampled_Parameter_5_Packet._length                   = (sizeof(Sampled_Parameter_5_Packet)-3)+40 ;
	Sampled_Parameter_5_Packet._Plauteu_Pressure         = Sampled_Parameter_3.Plateau_Pressure;
	Sampled_Parameter_5_Packet._I_Ratio_int              = Sampled_Parameter_4.I_Ratio_int;
	Sampled_Parameter_5_Packet._I_Ratio_float            = Sampled_Parameter_4.I_Ratio_float;
	Sampled_Parameter_5_Packet._E_Ratio_int              = Sampled_Parameter_4.E_Ratio_int;
	Sampled_Parameter_5_Packet._E_Ratio_float            = Sampled_Parameter_4.E_Ratio_float;
	Sampled_Parameter_5_Packet._CRC8                     = chksum8(&Sampled_Parameter_5_Packet._Plauteu_Pressure,(6));

#ifdef	UART_COMMUNICATION_1
	HAL_UART_Transmit_IT(&huart6,(uint8_t*)&Sampled_Parameter_5_Packet,sizeof(Sampled_Parameter_5_Packet));
#endif
#ifdef	USB_COMMUNICATION_1
	CDC_Transmit_FS((uint8_t*)&Sampled_Parameter_5_Packet,sizeof(Sampled_Parameter_5_Packet));
#endif



}
void Sampled_Parameter_Func_1()
{
	Average_PIP_Func();
	Average_PEEP_Func();
	Volume_Max_Inspiration_Func();
	Volume_Max_Expiration_Func();
	Peak_Flow_Inspiration_Func();
	Peak_Flow_Expiration_Func();
	Max_Flow = 0;
	Min_Flow = 0;
}


void Average_PIP_Func()
{
	if(Common_Mode_Parameter._RESPIRATORY_RATE_Val > 30 )
	{
		Sampled_Parameter_1.Average_PIP  = PIP_Average_Parameter.Maximum_PIP_Acheived;
	}
	else
	{
		Sampled_Parameter_1.Average_PIP  = PIP_Average_Parameter.PIP_AVG_VAL ;
	}
}


void Average_PEEP_Func()
{
	Sampled_Parameter_1.Average_PEEP = PEEP_Maintain_Parameter.PEEP_AVG_VAL;
}


void Volume_Max_Inspiration_Func()
{
	Sampled_Parameter_1.Volume_Max_Insp = vol.Volume_max;
}

void Volume_Max_Expiration_Func()
{
	Sampled_Parameter_1.Volume_Max_Exp= (Sampled_Parameter_1.Volume_Max_Insp -( V_Min));
}


void Peak_Flow_Inspiration_Func()
{
	Sampled_Parameter_1.MAX_Flow  = Max_Flow;
}
void Peak_Flow_Expiration_Func()
{
	Sampled_Parameter_1.MIN_Flow  = (Min_Flow) * (-1);
}

void Reset_Flag_Func()
{
	Sampled_Parameter_1.Average_PIP       = 0;
	Sampled_Parameter_1.Average_PEEP      = 0;
	Sampled_Parameter_1.Volume_Max_Insp   = 0;
	Sampled_Parameter_1.Volume_Max_Exp    = 0;
	Sampled_Parameter_1.MAX_Flow          = 0;
	Sampled_Parameter_1.MIN_Flow          = 0;
}



void Sampled_Parameter_Func_2()
{
	Minite_Volume_Func();
	Acheived_RR_Func();
	Inspiration_Time_Func();
	Expiration_Time_Func();
}


void Minite_Volume_Func()
{
	if(Breath == 0)
	{
		Sampled_Parameter_2.Total_Tidal_Volume_float     = Sampled_Parameter_2.Minite_Volume / 1000;

		Sampled_Parameter_2.Total_Tidal_Volume_Int       = Sampled_Parameter_2.Total_Tidal_Volume_float;

		Sampled_Parameter_2.Total_Tidal_Volume_Decimal_1 = Sampled_Parameter_2.Total_Tidal_Volume_float - Sampled_Parameter_2.Total_Tidal_Volume_Int;

		Sampled_Parameter_2.Total_Tidal_Volume_Decimal_1 = Sampled_Parameter_2.Total_Tidal_Volume_Decimal_1 * 1000;

		Sampled_Parameter_2.Total_Tidal_Volume_Decimal   = (int)Sampled_Parameter_2.Total_Tidal_Volume_Decimal_1 / 10;

		Sampled_Parameter_2.Minite_Volume=0;

	}
	else
	{
		Sampled_Parameter_2.Minite_Volume +=vol.Volume_max;
	}

}

void Acheived_RR_Func()
{
	Sampled_Parameter_2.Acheived_RR = Breath_In_Minite;
}


void Inspiration_Time_Func()
{
	Sampled_Parameter_2.Acheived_I_Time           = RR_I_TIME_ACHIEVED;


	Sampled_Parameter_2.Acheived_I_Time_float     = (float)Sampled_Parameter_2.Acheived_I_Time/1000;
	Sampled_Parameter_2.Acheived_I_Time_int       = Sampled_Parameter_2.Acheived_I_Time_float;
	Sampled_Parameter_2.Acheived_I_Time_Decimal_1 = Sampled_Parameter_2.Acheived_I_Time_float - Sampled_Parameter_2.Acheived_I_Time_int;
	Sampled_Parameter_2.Acheived_I_Time_Decimal_1 = Sampled_Parameter_2.Acheived_I_Time_Decimal_1 * 1000;
	Sampled_Parameter_2.Acheived_I_Time_Decimal   = (int)Sampled_Parameter_2.Acheived_I_Time_Decimal_1/10;
	//Sampled_Parameter_2.Acheived_I_Time_Decimal   = Sampled_Parameter_2.Acheived_I_Time_int;
}

void Expiration_Time_Func()
{
	Sampled_Parameter_2.Acheived_E_Time           = RR_E_TIME_ACHIEVED;

	if(Sampled_Parameter_2.Acheived_E_Time <1000)
		Sampled_Parameter_2.Acheived_E_Time_float     = (float)Sampled_Parameter_2.Acheived_E_Time/1000;
	else
	    Sampled_Parameter_2.Acheived_E_Time_float     = (float)Sampled_Parameter_2.Acheived_E_Time/1000;


	Sampled_Parameter_2.Acheived_E_Time_int       = Sampled_Parameter_2.Acheived_E_Time_float;
	Sampled_Parameter_2.Acheived_E_Time_Decimal_1 = Sampled_Parameter_2.Acheived_E_Time_float - Sampled_Parameter_2.Acheived_E_Time_int;
	Sampled_Parameter_2.Acheived_E_Time_Decimal_1 = Sampled_Parameter_2.Acheived_E_Time_Decimal_1 * 1000;
	Sampled_Parameter_2.Acheived_E_Time_Decimal   = (int)Sampled_Parameter_2.Acheived_E_Time_Decimal_1/10;
	//Sampled_Parameter_2.Acheived_E_Time_Decimal   =  Sampled_Parameter_2.Acheived_E_Time_int;

}


void Sampled_Parameter_Func_3()
{
	Platau_Pressure();
	Static_and_Dynamic_Compliance();
	Inspiratory_Resistance();
}



void Platau_Pressure()
{
	Sampled_Parameter_3.Compliance= (PIP_Average_Parameter.Maximum_PIP_Acheived - PIP_Average_Parameter.PIP_AVG_VAL );

	if(Sampled_Parameter_3.Compliance>=2)
	{
		Sampled_Parameter_3.Plateau_Pressure = (PIP_Average_Parameter.Maximum_PIP_Acheived - Sampled_Parameter_3.Compliance);
	}
	else
	{
		Sampled_Parameter_3.Plateau_Pressure = PIP_Average_Parameter.PIP_AVG_VAL;
	}
}




void Static_and_Dynamic_Compliance()
{
	if(Alert_Check>3)
	{
		Sampled_Parameter_3.Staic_Compliance_int     = 5;
		Sampled_Parameter_3.Staic_Compliance_float   = 0;
		Sampled_Parameter_3.Dynamic_Compliance_int   = 5;
		Sampled_Parameter_3.Dynamic_Compliance_float = 0;
	}
	else
	{
			if(Sampled_Parameter_3.Compliance>=2)
			{
				Sampled_Parameter_3.Staic_Compliance = ( (Sampled_Parameter_1.Volume_Max_Insp) / (Sampled_Parameter_3.Plateau_Pressure - PEEP_Maintain_Parameter.PEEP_AVG_VAL) );

				Sampled_Parameter_3.Staic_Compliance_int = (uint8_t)Sampled_Parameter_3.Staic_Compliance;

				if( (Sampled_Parameter_3.Staic_Compliance_int < 1) && (Sampled_Parameter_3.Staic_Compliance_int > 250) )
					Sampled_Parameter_3.Staic_Compliance_int =1;
				double dummy,dummy2;

				dummy2 = modf(Sampled_Parameter_3.Staic_Compliance,&dummy);

				dummy2 =dummy2 * 100;

				Sampled_Parameter_3.Staic_Compliance_float = (int)dummy2 ;

			}
			else
			{
				Sampled_Parameter_3.Staic_Compliance = ( (Sampled_Parameter_1.Volume_Max_Insp) / (PIP_Average_Parameter.Maximum_PIP_Acheived  - PEEP_Maintain_Parameter.PEEP_AVG_VAL) );

				Sampled_Parameter_3.Staic_Compliance_int = (uint8_t)Sampled_Parameter_3.Staic_Compliance;

				if( (Sampled_Parameter_3.Staic_Compliance_int < 1) && (Sampled_Parameter_3.Staic_Compliance_int > 250) )
					Sampled_Parameter_3.Staic_Compliance_int =1;

				double dummy3,dummy4;

				dummy4 = modf(Sampled_Parameter_3.Staic_Compliance,&dummy3);

				dummy4 =dummy4 * 100;

				Sampled_Parameter_3.Staic_Compliance_float = (int)dummy4 ;


			}

				Sampled_Parameter_3.Dynamic_Compliance = ( (Sampled_Parameter_1.Volume_Max_Insp) / (PIP_Average_Parameter.Maximum_PIP_Acheived - PEEP_Maintain_Parameter.PEEP_AVG_VAL) );

				Sampled_Parameter_3.Dynamic_Compliance_int = (uint8_t)Sampled_Parameter_3.Dynamic_Compliance;

				double dummy5,dummy6;

				dummy6 = modf(Sampled_Parameter_3.Dynamic_Compliance,&dummy5);

				dummy6 =dummy6 * 100;

				Sampled_Parameter_3.Dynamic_Compliance_float = (int)dummy6 ;
	}

}


void Inspiratory_Resistance()
{
	if(Sampled_Parameter_3.Compliance>=2)
	{
		Sampled_Parameter_3.Rinsp = ( (Sampled_Parameter_3.Plateau_Pressure - PEEP_Maintain_Parameter.PEEP_AVG_VAL) / (Sampled_Parameter_1.Volume_Max_Insp));

		Sampled_Parameter_3.Rinsp_int = (uint8_t)Sampled_Parameter_3.Rinsp;

		double dummy3,dummy4,dummy5,dummy6;

		dummy4 = modf(Sampled_Parameter_3.Rinsp,&dummy3);

		dummy5 =dummy4 * 10;

		Sampled_Parameter_3.Rinsp_float_1 = (int)dummy5;

		dummy6 =dummy4 * 100;

		Sampled_Parameter_3.Rinsp_float_2 = (int)dummy6;
	}
	else
	{
		Sampled_Parameter_3.Rinsp = (  (PIP_Average_Parameter.Maximum_PIP_Acheived  - PEEP_Maintain_Parameter.PEEP_AVG_VAL)  / (Sampled_Parameter_1.Volume_Max_Insp) );

		Sampled_Parameter_3.Rinsp_int = (uint8_t)Sampled_Parameter_3.Rinsp;

		double dummy3,dummy4,dummy5,dummy6;

		dummy4 = modf(Sampled_Parameter_3.Rinsp,&dummy3);

		dummy5 =dummy4 * 10;

		Sampled_Parameter_3.Rinsp_float_1 = (int)dummy5;

		dummy6 =dummy4 * 100;

		Sampled_Parameter_3.Rinsp_float_2 = (int)dummy6;
	}
}

void Sampled_Parameter_Func_4()
{
	Mean_Airway_Pressure();
	I_and_E_Ratio_Func();
}



void Mean_Airway_Pressure()
{
	Sampled_Parameter_4.MAP = ( ( (Sampled_Parameter_2.Acheived_I_Time_float * Sampled_Parameter_1.Average_PIP) ) +  ( (Sampled_Parameter_2.Acheived_E_Time_float   * PEEP_Maintain_Parameter.PEEP_AVG_VAL )) ) / ( Sampled_Parameter_2.Acheived_I_Time_float + Sampled_Parameter_2.Acheived_E_Time_float  );

	Sampled_Parameter_4.MAP_int = (uint8_t)Sampled_Parameter_4.MAP;

	double dummy,dummy2;

	dummy2 = modf(Sampled_Parameter_4.MAP,&dummy);

	dummy2 =dummy2 * 100;

	Sampled_Parameter_4.MAP_float = (int)dummy2 ;
}



void I_and_E_Ratio_Func()
{
	int n1, n2, i, gcd;

	n1 = Sampled_Parameter_2.Acheived_I_Time;
	n2 = Sampled_Parameter_2.Acheived_E_Time;

	for(i=1; i <= n1 && i <= n2; ++i)
	{
		if(n1%i==0 && n2%i==0)
			gcd = i;
	}



	Sampled_Parameter_4.I_Ratio= (float)Sampled_Parameter_2.Acheived_I_Time /(float)gcd ;
	Sampled_Parameter_4.E_Ratio= (float)Sampled_Parameter_2.Acheived_E_Time /(float)gcd ;

	//Sampled_Parameter_4.I_Ratio= (float)Sampled_Parameter_2.Acheived_I_Time_float /(float)gcd ;
	//Sampled_Parameter_4.E_Ratio= (float)Sampled_Parameter_2.Acheived_E_Time_float /(float)gcd ;


	    Sampled_Parameter_4.I_Ratio_int = (uint8_t)Sampled_Parameter_4.I_Ratio;

		double dummy,dummy2;

		dummy2 = modf(Sampled_Parameter_4.I_Ratio,&dummy);

		dummy2 =dummy2 * 100;

		Sampled_Parameter_4.I_Ratio_float = (int)dummy2 ;



		        Sampled_Parameter_4.E_Ratio_int = (uint8_t)Sampled_Parameter_4.E_Ratio;

				double dummy11,dummy12;

				dummy12 = modf(Sampled_Parameter_4.E_Ratio,&dummy11);

				dummy12 =dummy12 * 100;

				Sampled_Parameter_4.E_Ratio_float = (int)dummy12 ;




}


void Sampled_Parameter_Inspiration_time()
{
	V_Min     = vol.Volume;
	Sampled_Parameter_Func_1();
	Sampled_Parameter_Func_2();
	Sampled_Parameter_Func_3();
	Sampled_Parameter_Func_4();
	Send_Frame = First_Frame;
	Send_Frame_Fuel_Gauge = First_Frame;
}
