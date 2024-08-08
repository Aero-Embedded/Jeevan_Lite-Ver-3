/*
 * Common_Parameters.c
 *
 *  Created on: Feb 1, 2023
 *      Author: asus
 */


#include "Common_Parameter.h"


void Find_Max_PIP()
{
	if(Pressure_sensor.Pressure_Val >= PIP_Average_Parameter.Maximum_PIP_Acheived)
	{
		PIP_Average_Parameter.Maximum_PIP_Acheived   = Pressure_sensor.Pressure_Val;
	}
}

void Find_Maximum_PEEP()
{
	if(PEEP_Maintain_Parameter.Maximum_PEEP_Acheived < Pressure_sensor.Pressure_Val)
	{
		PEEP_Maintain_Parameter.Maximum_PEEP_Acheived = Pressure_sensor.Pressure_Val;
	}
}


void PIP_AVERAGE()
{
	if((PIP_Average_Parameter.PIP_Cumulative_Val == 0) || (PIP_Average_Parameter.PIP_Event_Count == 0))
	{
		PIP_Average_Parameter.PIP_Cumulative_Val = 1;
		PIP_Average_Parameter.PIP_Event_Count    = 1;
	}

	PIP_Average_Parameter.PIP_AVG_VAL        = (uint32_t)(PIP_Average_Parameter.PIP_Cumulative_Val/ PIP_Average_Parameter.PIP_Event_Count);
	PIP_Average_Parameter.PIP_Cumulative_Val = RESET;
	PIP_Average_Parameter.PIP_Event_Count    = RESET;
}
void PEEP_AVERAGE()
{

	if(Patient_Trigger == PATIENT_TRIGGER_HAPPEN)
	{
		if((PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger == 0) || (PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger == 0))
		{
			PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger = 1;
			PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger    = 1;
		}
		PEEP_Maintain_Parameter.PEEP_AVG_VAL                = (uint32_t)(PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger/ PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger);
		PEEP_Maintain_Parameter.PEEP_Cumulative_Val_Trigger = RESET;
		PEEP_Maintain_Parameter.PEEP_Event_Count_Trigger    = RESET;
	}
	else
	{
		if((PEEP_Maintain_Parameter.PEEP_Cumulative_Val == 0) || (PEEP_Maintain_Parameter.PEEP_Event_Count == 0))
		{
			PEEP_Maintain_Parameter.PEEP_Cumulative_Val = 1;
			PEEP_Maintain_Parameter.PEEP_Event_Count    = 1;
		}
		PEEP_Maintain_Parameter.PEEP_AVG_VAL        = (uint32_t)(PEEP_Maintain_Parameter.PEEP_Cumulative_Val/ PEEP_Maintain_Parameter.PEEP_Event_Count);
		PEEP_Maintain_Parameter.PEEP_Cumulative_Val = RESET;
		PEEP_Maintain_Parameter.PEEP_Event_Count    = RESET;
	}
}






void INITIAL_DAC_VALUE()
{
	DAC_SIGNALS.DAC_VALUE_5       = 450;
	DAC_SIGNALS.DAC_VALUE_6       = 500;
	DAC_SIGNALS.DAC_VALUE_7       = 550;
	DAC_SIGNALS.DAC_VALUE_8       = 600;
	DAC_SIGNALS.DAC_VALUE_9       = 630;
	DAC_SIGNALS.DAC_VALUE_10      = 660;
	DAC_SIGNALS.DAC_VALUE_11      = 690;
	DAC_SIGNALS.DAC_VALUE_12      = 720;
	DAC_SIGNALS.DAC_VALUE_13      = 750;
	DAC_SIGNALS.DAC_VALUE_14      = 780;
	DAC_SIGNALS.DAC_VALUE_15      = 810;
	DAC_SIGNALS.DAC_VALUE_16      = 840;
	DAC_SIGNALS.DAC_VALUE_17      = 870;
	DAC_SIGNALS.DAC_VALUE_18      = 900;
	DAC_SIGNALS.DAC_VALUE_19      = 930;
	DAC_SIGNALS.DAC_VALUE_20      = 950;
	DAC_SIGNALS.DAC_VALUE_21      = 970;
	DAC_SIGNALS.DAC_VALUE_22      = 990;
	DAC_SIGNALS.DAC_VALUE_23      = 1010;
	DAC_SIGNALS.DAC_VALUE_24      = 1030;
	DAC_SIGNALS.DAC_VALUE_25      = 1050;
	DAC_SIGNALS.DAC_VALUE_26      = 1080;
	DAC_SIGNALS.DAC_VALUE_27      = 1100;
	DAC_SIGNALS.DAC_VALUE_28      = 1120;
	DAC_SIGNALS.DAC_VALUE_29      = 1140;
	DAC_SIGNALS.DAC_VALUE_30      = 1160;
	DAC_SIGNALS.DAC_VALUE_31      = 1180;
	DAC_SIGNALS.DAC_VALUE_32      = 1200;
	DAC_SIGNALS.DAC_VALUE_33      = 1220;
	DAC_SIGNALS.DAC_VALUE_34      = 1240;
	DAC_SIGNALS.DAC_VALUE_35      = 1260;
	DAC_SIGNALS.DAC_VALUE_36      = 1280;
	DAC_SIGNALS.DAC_VALUE_37      = 1290;
	DAC_SIGNALS.DAC_VALUE_38      = 1310;
	DAC_SIGNALS.DAC_VALUE_39      = 1325;
	DAC_SIGNALS.DAC_VALUE_40      = 1340;
	DAC_SIGNALS.DAC_VALUE_41      = 1360;
	DAC_SIGNALS.DAC_VALUE_42      = 1380;
	DAC_SIGNALS.DAC_VALUE_43      = 1400;
	DAC_SIGNALS.DAC_VALUE_44      = 1420;
	DAC_SIGNALS.DAC_VALUE_45      = 1440;
	DAC_SIGNALS.DAC_VALUE_46      = 1460;
	DAC_SIGNALS.DAC_VALUE_47      = 1470;
	DAC_SIGNALS.DAC_VALUE_48      = 1480;
	DAC_SIGNALS.DAC_VALUE_49      = 1500;
	DAC_SIGNALS.DAC_VALUE_50      = 1520;
	DAC_SIGNALS.DAC_VALUE_51      = 1540;
	DAC_SIGNALS.DAC_VALUE_52      = 1560;
	DAC_SIGNALS.DAC_VALUE_53      = 1570;
	DAC_SIGNALS.DAC_VALUE_54      = 1580;
	DAC_SIGNALS.DAC_VALUE_55      = 1600;
	DAC_SIGNALS.DAC_VALUE_56      = 1610;
	DAC_SIGNALS.DAC_VALUE_57      = 1630;
	DAC_SIGNALS.DAC_VALUE_58      = 1650;
	DAC_SIGNALS.DAC_VALUE_59      = 1670;
	DAC_SIGNALS.DAC_VALUE_60      = 1680;

}





uint16_t DAC_VAL(uint8_t Set_VAL)
{

	switch(Set_VAL)
	{
	    case 5:
	    	 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_5;
	    break;
	    case 6:
	    	 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_6;
	    break;
	    case 7:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_7;
		break;
	    case 8:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_8;
		break;
	    case 9:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_9;
		break;
	    case 10:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_10;
		break;
		case 11:
			 DAC_VALUE   = DAC_SIGNALS.DAC_VALUE_11;
		break;
		case 12:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_12;
		break;
		case 13:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_13;
		break;
		case 14:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_14;
		break;
		case 15:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_15;
		break;
		case 16:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_16;
		break;
		case 17:
			 DAC_VALUE   = DAC_SIGNALS.DAC_VALUE_17;
		break;
		case 18:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_18;
		break;
		case 19:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_19;
		break;
		case 20:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_20;
		break;
		case 21:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_21;
		break;
		case 22:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_22;
		break;
		case 23:
			 DAC_VALUE   = DAC_SIGNALS.DAC_VALUE_23;
		break;
		case 24:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_24;
		break;
		case 25:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_25;
		break;
		case 26:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_26;
		break;
		case 27:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_27;
		break;
		case 28:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_28;
		break;
	    case 29:
			 DAC_VALUE   = DAC_SIGNALS.DAC_VALUE_29;
		break;
		case 30:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_30;
		break;
		case 31:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_31;
		break;
		case 32:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_32;
		break;
		case 33:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_33;
		break;
		case 34:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_34;
		break;
		case 35:
			 DAC_VALUE   = DAC_SIGNALS.DAC_VALUE_35;
		break;
		case 36:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_36;
		break;
		case 37:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_37;
		break;
		case 38:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_38;
		break;
		case 39:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_39;
		break;
		case 40:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_40;
		break;
		case 41:
			 DAC_VALUE   = DAC_SIGNALS.DAC_VALUE_41;
		break;
		case 42:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_42;
		break;
		case 43:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_43;
		break;
		case 44:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_44;
		break;
		case 45:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_45;
		break;
		case 46:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_46;
		break;
		case 47:
			 DAC_VALUE   = DAC_SIGNALS.DAC_VALUE_47;
		break;
		case 48:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_48;
		break;
		case 49:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_49;
		break;
		case 50:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_50;
		break;
		case 51:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_51;
		break;
		case 52:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_52;
		break;
	    case 53:
			 DAC_VALUE   = DAC_SIGNALS.DAC_VALUE_53;
		break;
		case 54:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_54;
		break;
		case 55:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_55;
		break;
		case 56:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_56;
		break;
		case 57:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_57;
		break;
		case 58:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_58;
		break;
		case 59:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_59;
		break;
		case 60:
			 DAC_VALUE  = DAC_SIGNALS.DAC_VALUE_60;
		break;
	}

	return DAC_VALUE;
}





void Change_DAC_Values(uint8_t Set_Value,uint16_t Blower_Value)
{
	switch(Set_Value)
	{
	    case 5:
	    	 DAC_SIGNALS.DAC_VALUE_5   =Blower_Value;
	    break;
	    case 6:
	    	 DAC_SIGNALS.DAC_VALUE_6   =Blower_Value;
	    break;
	    case 7:
			 DAC_SIGNALS.DAC_VALUE_7   =Blower_Value;
		break;
	    case 8:
			 DAC_SIGNALS.DAC_VALUE_8   =Blower_Value;
		break;
	    case 9:
			 DAC_SIGNALS.DAC_VALUE_9   =Blower_Value;
		break;
	    case 10:
			 DAC_SIGNALS.DAC_VALUE_10  =Blower_Value;
		break;
		case 11:
			 DAC_SIGNALS.DAC_VALUE_11  =Blower_Value;
		break;
		case 12:
			 DAC_SIGNALS.DAC_VALUE_12  =Blower_Value;
		break;
		case 13:
			 DAC_SIGNALS.DAC_VALUE_13  =Blower_Value;
		break;
		case 14:
			 DAC_SIGNALS.DAC_VALUE_14  =Blower_Value;
		break;
		case 15:
			 DAC_SIGNALS.DAC_VALUE_15  =Blower_Value;
		break;
		case 16:
			 DAC_SIGNALS.DAC_VALUE_16  =Blower_Value;
		break;
		case 17:
			 DAC_SIGNALS.DAC_VALUE_17  =Blower_Value;
		break;
		case 18:
			 DAC_SIGNALS.DAC_VALUE_18  =Blower_Value;
		break;
		case 19:
			 DAC_SIGNALS.DAC_VALUE_19  =Blower_Value;
		break;
		case 20:
			 DAC_SIGNALS.DAC_VALUE_20  =Blower_Value;
		break;
		case 21:
			 DAC_SIGNALS.DAC_VALUE_21  =Blower_Value;
		break;
		case 22:
			 DAC_SIGNALS.DAC_VALUE_22  =Blower_Value;
		break;
		case 23:
			 DAC_SIGNALS.DAC_VALUE_23  =Blower_Value;
		break;
		case 24:
			 DAC_SIGNALS.DAC_VALUE_24  =Blower_Value;
		break;
		case 25:
			 DAC_SIGNALS.DAC_VALUE_25  =Blower_Value;
		break;
		case 26:
			 DAC_SIGNALS.DAC_VALUE_26  =Blower_Value;
		break;
		case 27:
			 DAC_SIGNALS.DAC_VALUE_27  =Blower_Value;
		break;
		case 28:
			 DAC_SIGNALS.DAC_VALUE_28  =Blower_Value;
		break;
	    case 29:
			 DAC_SIGNALS.DAC_VALUE_29  =Blower_Value;
		break;
		case 30:
			 DAC_SIGNALS.DAC_VALUE_30  =Blower_Value;
		break;
		case 31:
			 DAC_SIGNALS.DAC_VALUE_31  =Blower_Value;
		break;
		case 32:
			 DAC_SIGNALS.DAC_VALUE_32  =Blower_Value;
		break;
		case 33:
			 DAC_SIGNALS.DAC_VALUE_33  =Blower_Value;
		break;
		case 34:
			 DAC_SIGNALS.DAC_VALUE_34  =Blower_Value;
		break;
		case 35:
			 DAC_SIGNALS.DAC_VALUE_35  =Blower_Value;
		break;
		case 36:
			 DAC_SIGNALS.DAC_VALUE_36  =Blower_Value;
		break;
		case 37:
			 DAC_SIGNALS.DAC_VALUE_37  =Blower_Value;
		break;
		case 38:
			 DAC_SIGNALS.DAC_VALUE_38  =Blower_Value;
		break;
		case 39:
			 DAC_SIGNALS.DAC_VALUE_39  =Blower_Value;
		break;
		case 40:
			 DAC_SIGNALS.DAC_VALUE_40  =Blower_Value;
		break;
		case 41:
			 DAC_SIGNALS.DAC_VALUE_41  =Blower_Value;
		break;
		case 42:
			 DAC_SIGNALS.DAC_VALUE_42  =Blower_Value;
		break;
		case 43:
			 DAC_SIGNALS.DAC_VALUE_43  =Blower_Value;
		break;
		case 44:
			 DAC_SIGNALS.DAC_VALUE_44  =Blower_Value;
		break;
		case 45:
			 DAC_SIGNALS.DAC_VALUE_45  =Blower_Value;
		break;
		case 46:
			 DAC_SIGNALS.DAC_VALUE_46  =Blower_Value;
		break;
		case 47:
			 DAC_SIGNALS.DAC_VALUE_47  =Blower_Value;
		break;
		case 48:
			 DAC_SIGNALS.DAC_VALUE_48  =Blower_Value;
		break;
		case 49:
			 DAC_SIGNALS.DAC_VALUE_49  =Blower_Value;
		break;
		case 50:
			 DAC_SIGNALS.DAC_VALUE_50  =Blower_Value;
		break;
		case 51:
			 DAC_SIGNALS.DAC_VALUE_51  =Blower_Value;
		break;
		case 52:
			 DAC_SIGNALS.DAC_VALUE_52  =Blower_Value;
		break;
	    case 53:
			 DAC_SIGNALS.DAC_VALUE_53  =Blower_Value;
		break;
		case 54:
			 DAC_SIGNALS.DAC_VALUE_54  =Blower_Value;
		break;
		case 55:
			 DAC_SIGNALS.DAC_VALUE_55  =Blower_Value;
		break;
		case 56:
			 DAC_SIGNALS.DAC_VALUE_56  =Blower_Value;
		break;
		case 57:
			 DAC_SIGNALS.DAC_VALUE_57  =Blower_Value;
		break;
		case 58:
			 DAC_SIGNALS.DAC_VALUE_58  =Blower_Value;
		break;
		case 59:
			 DAC_SIGNALS.DAC_VALUE_59  =Blower_Value;
		break;
		case 60:
			 DAC_SIGNALS.DAC_VALUE_60  =Blower_Value;
		break;
	}

}
