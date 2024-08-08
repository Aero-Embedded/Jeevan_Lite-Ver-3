/*
 * Bluetooth_Data_Split.c
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */


#include "Bluetooth_Data_Split.h"



void Rearrange_Bluetooth_Data();




void Split_Mode_Parameter_Packets(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet)
{
	    uint8_t Mode_Val;
	    uint8_t BackupMode;
	    Mode_Val         =  ( 0x0F & (Receive_Graph_Packet->Control_Byte) ) ;
	    BackupMode       =  ( 0x20 & (Receive_Graph_Packet->Control_Byte) ) >>5;

	    if(BackupMode == Backup_PCCMV_Mode)
	    {
	    	Mode = Backup_Pccmv;
	    }
	    else
	    {
	    	Mode= Mode_Val;
	    }

	    Device_Control   =  (0x10 & (Receive_Graph_Packet->Control_Byte))>>4;
	    if(Device_Control == PAUSE)
	    {

	    	Running_Mode   = Idle;
	    	Run_Current_Breathe_State = No_Run_State;
	    	vTaskSuspend(Vc_Cmv_Handler);
			vTaskSuspend(Volume_Control_Task_Handler);
			vTaskSuspend(One_Time_Handler);
			vTaskSuspend(Oxygen_Blending_Handler);
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
			vol.Volume       = RESET;
			vol.Volume_max   = RESET;
			vol.Volume_Val   = RESET;
		    Blue_Led_ON();
		    Red_Led_OFF();
		    Green_Led_OFF();
		    Led_Pause_Status = 1;
	    }
	    else if(Device_Control == RESUME)
	    {

				switch(Mode)
				{
					case Pccmv:
						Pc_Cmv_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Running_Mode        = Pccmv;
						Current_Backup_Mode = No_Backup_Mode;
					break;
					case Vccmv:
						Vc_Cmv_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Running_Mode        = Vccmv;
						Current_Backup_Mode = No_Backup_Mode;
					break;
					case Pcsimv:
						Pc_Simv_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Running_Mode        = Pcsimv;
						Current_Backup_Mode = No_Backup_Mode;
					break;
					case Vcsimv:
						Vc_Simv_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Running_Mode        = Vcsimv;
						Current_Backup_Mode = No_Backup_Mode;
					break;
					case Psv:
						PSV_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Running_Mode        = Psv;
					break;
					case Cpap:
						CPAP_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Running_Mode        = Cpap;
					break;
					case BiPap:
						BIPAP_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Running_Mode        = BiPap;
					break;
					case Aprv:
						APRV_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Running_Mode        = Aprv;
					break;
					case Backup_Pccmv:
						Backup_Pc_Cmv_Mode_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
						Current_Backup_Mode  = Backup_PCCMV_Mode;
					break;

					case Hfnc:
						Running_Mode        = Hfnc;
						 HFNC_Packet_Data((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
					     Current_Backup_Mode = No_Backup_Mode;
					break;
					case Suction:
						 Running_Mode        = Suction;
						 SUCTION_Packet_Data();
					     Current_Backup_Mode = No_Backup_Mode;
					break;
					default:
				    break;
				}
	    }

}


uint8_t chksum8_2(const unsigned char *buff, size_t len)
{
    unsigned int sum;
    for ( sum = 0 ; len != 0 ; len-- )
        sum += *(buff++);
    return (uint8_t)sum;
}


void Bluetooth_Packet_Data_Split(RECEIVE_GRAPH_PACKET  *Receive_Graph_Packet)
{

	uint8_t RX_CRC8;
	RX_CRC8 = chksum8_2(&(Receive_Graph_Packet-> Control_Byte),( 16 ));
	if(RX_CRC8 == (Receive_Graph_Packet ->CRC8) )
	{
		switch(Receive_Graph_Packet->header)
		{
			case Receive_BlueTooth_Graph_Header:
				Split_Mode_Parameter_Packets((RECEIVE_GRAPH_PACKET*) (Receive_Graph_Packet));
			break;
			case Receive_BlueTooth_Alert_Header:
				Split_Alert_Parameter_Packets((ALERT_RANGE_PACKET*) (Receive_Graph_Packet));
			break;
			case Receive_Nebuliser_Header:
				Split_Nebuliser_Packets((NEBULISER_PACKET*) (Receive_Graph_Packet));
			break;
			case Receive_Calibration_Header:
				Split_Calibration_Packets((CALIBRATION_PACKET*) (Receive_Graph_Packet));
			break;
			default:
			break;
		}
	}
	else
	{

		Rearrange_Bluetooth_Data();

		switch(Receive_Graph_Packet->header)
		{
			case Receive_BlueTooth_Graph_Header:
				Split_Mode_Parameter_Packets((RECEIVE_GRAPH_PACKET*) (Bluetooth_Parameter.UART_RX_BUF_CURR));
			break;
			case Receive_BlueTooth_Alert_Header:
				Split_Alert_Parameter_Packets((ALERT_RANGE_PACKET*) (Bluetooth_Parameter.UART_RX_BUF_CURR));
			break;
			case Receive_Nebuliser_Header:
				Split_Nebuliser_Packets((NEBULISER_PACKET*) (Bluetooth_Parameter.UART_RX_BUF_CURR));
			break;
			case Receive_Calibration_Header:
				Split_Calibration_Packets((CALIBRATION_PACKET*) (Bluetooth_Parameter.UART_RX_BUF_CURR));
			break;
			default:
			break;
		}
	}


}



void Rearrange_Bluetooth_Data()
{

	Received_APP_data_found=0;
	Received_index_Number=0;


	for(arrangecount=0;arrangecount<=19;arrangecount++)
	{
		if(Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount]==83)
		{
			if(Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount+1]==80)
			{
				Received_index_Number=arrangecount;
				Received_APP_data_found=1;
				break;
			}
		}
		else if(Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount]==84)
		{
			if(Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount+1]==80)
			{
				Received_index_Number=arrangecount;
				Received_APP_data_found=1;
				break;
			}
		}
		else if(Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount]==82)
		{
			if(Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount+1]==80)
			{
				Received_index_Number=arrangecount;
				Received_APP_data_found=1;
				break;
			}
		}
		else if(Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount]==87)
		{
			if(Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount+1]==80)
			{
				Received_index_Number=arrangecount;
				Received_APP_data_found=1;
				break;
			}
		}
	}

	if(Received_APP_data_found==1)
	{
		for(arrangecount=0;arrangecount<=19;arrangecount++)
		{
			Received_index_number2  = Received_index_Number + arrangecount;

			if(Received_index_number2>19)
			{
				Bluetooth_Parameter.UART_RX_BUF_CURR[arrangecount]=Bluetooth_Parameter.Bluetooth_RX_BUF[Received_index_number2-20];
			}
			else
			{
				Bluetooth_Parameter.UART_RX_BUF_CURR[arrangecount]=Bluetooth_Parameter.Bluetooth_RX_BUF[Received_index_number2];
			}

		}

	}

	for(arrangecount=0;arrangecount<=19;arrangecount++)
	{
		Bluetooth_Parameter.Bluetooth_RX_BUF[arrangecount]  =  Bluetooth_Parameter.UART_RX_BUF_CURR[arrangecount];
	}

}



