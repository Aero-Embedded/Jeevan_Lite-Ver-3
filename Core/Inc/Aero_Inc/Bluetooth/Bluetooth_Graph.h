/*
 * Bluetooth_Graph.h
 *
 *  Created on: Jan 19, 2023
 *      Author: asus
 */

#ifndef INC_BLUETOOTH_GRAPH_H_
#define INC_BLUETOOTH_GRAPH_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"






typedef struct  __attribute__((packed))
{
	uint16_t _header ;
	uint8_t  _length ;
	uint8_t  _Pressure_Val;
	int16_t  _Flow_Val;
	int16_t  _Volume_Val;
	uint8_t  _Status;
	uint8_t  _CRC8;
}GRAPH_PACKET_SEND ;

typedef struct __attribute__((packed))
{
	uint16_t header;
	uint8_t  length;
	uint8_t  Control_Byte;
	uint8_t  PIP_PS_Phigh;
	uint8_t  PEEP_CPAP_Plow;
	uint16_t VTI_VS;
	uint8_t FiO2;
	uint8_t RR;
	uint8_t T_high;
	uint8_t T_low;
	uint8_t Rise_Time;
	uint8_t Flow_Limit_FiO2_Flow_Limit;
	uint8_t Apnea_Time;
	uint8_t Trigger_Time;
	uint8_t Trigger_Limit;
    uint8_t NC1;
    uint8_t NC2;
    uint8_t CRC8;
}RECEIVE_GRAPH_PACKET ;

typedef enum
{
	BlueTooth_Wait_Time = 0,
	Bluetooth_Init ,
	Bluetooth_Send_Data
}Bluetooth_T;

typedef enum
{
	BlueTooth_Status_Wait_Time = 0,
	Bluetooth_Status_Init,
	Bluetooth_Status_Send_Data,

}Bluetooth_Status_T;


typedef enum
{
	Send_BlueTooth_Graph_Header             = 0x4C2A,
	Send_BlueTooth_Alert_Header             = 0x4D2A,
	Send_BlueTooth_Sampled_Parameter_Header = 0x532A,
	Send_BlueTooth_Calibration_Header       = 0x542B

}Send_Graph_header;


typedef enum
{
	Receive_BlueTooth_Graph_Header = 0x4D2B,
	Receive_BlueTooth_Alert_Header = 0x412B,
	Receive_Nebuliser_Header       = 0x462B,
	Receive_Calibration_Header     = 0x542C

}Receive_Graph_header;


typedef struct
{
	uint16_t Bluetooth_Transmit_Delay;
	uint8_t Bluetooth_RX_BUF[20];
	uint8_t UART_RX_BUF_CURR[20];
}Bluetooth_parameter;


xQueueHandle Bluetooth_Receive;


GRAPH_PACKET_SEND Graph_Data;

Bluetooth_parameter Bluetooth_Parameter;

Bluetooth_T Bluetooth;
Bluetooth_Status_T Bluetooth_Status;
Send_Graph_header Send_Graph_Header;
Receive_Graph_header Receive_Graph_Header;


xTaskHandle Bluetooth_data_Split_Handler;
xTaskHandle Bluetooth_Handler;
xTaskHandle Bluetooth_Receiver_Handler;

void Bluetooth_Task(void *argument);

uint8_t chksum8(const unsigned char *buff, size_t len);
void UART6_Init();
void UART5_Init();
void Bluetooth_Graph_Data_Send();
void Bluetooth_Data_Send();
void BlueTooth_Receiver_Task(void *argument);

void Bluetooth_Packet_Data_Split(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void Bluetooth_data_split_Task (void *argument);
void Pc_Cmv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void Vc_Cmv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void Pc_Simv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void Vc_Simv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void PSV_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void CPAP_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void BIPAP_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void APRV_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void Backup_Pc_Cmv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void Backup_Vc_Cmv_Mode_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void HFNC_Packet_Data(RECEIVE_GRAPH_PACKET *Receive_Graph_Packet);
void SUCTION_Packet_Data();
void UART5_GRAPH_DATA_DEBUG();
void USB_GRAPH_DATA_DEBUG();


#endif /* INC_BLUETOOTH_GRAPH_H_ */
