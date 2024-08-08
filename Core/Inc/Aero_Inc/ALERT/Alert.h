/*
 * Alert.h
 *
 *  Created on: Mar 30, 2022
 *      Author: asus
 */

#ifndef INC_ALERT_H_
#define INC_ALERT_H_


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"





#define Red_Led_ON()     GPIOD->ODR|=(1<<4)
#define Red_Led_OFF()    GPIOD->ODR&=(~(1<<4))
#define Blue_Led_ON()    GPIOD->ODR|=(1<<3)
#define Blue_Led_OFF()   GPIOD->ODR&=(~(1<<3))
#define Green_Led_ON()   GPIOD->ODR|=(1<<5)
#define Green_Led_OFF()  GPIOD->ODR&=(~(1<<5))



#define SET_ALERT_BIT(frame,bitname)    (DEVICE_ALERT_DATA_SEND.frame.FRAMEBits.bitname = 1 )
#define CLEAR_ALERT_BIT(frame,bitname)  (DEVICE_ALERT_DATA_SEND.frame.FRAMEBits.bitname = 0 )
#define DEVICE_ALERT_DATA_SEND_LENGTH 6



typedef struct __attribute__((packed)) {

	volatile uint8_t _ALERT_PLUGGED_IN:1;
	volatile uint8_t _ALERT_BATTERY_MODE:1;
	volatile uint8_t _ALERT_20_PER_BATTERY_DRAIN:1;
	volatile uint8_t _ALERT_OXYGEN_SUPPLY_FAILED:1;
	volatile uint8_t _ALERT_PATIENT_CIRCUIT_DISCONNECTED:1;
	volatile uint8_t _ALERT_FLOW_SENSOR_WRONG_DIRECTION:1;
	volatile uint8_t _ALERT_NEBULISER_ON:1;
	volatile uint8_t _ALERT_NEBULISER_OFF:1;

} FIRST_FRAME ;


typedef struct __attribute__((packed)) {

	volatile uint8_t _ALERT_APNEA:1;
	volatile uint8_t _ALERT_LOW_SPO2:1;
	volatile uint8_t _ALERT_PRESSURE_LIMITATION:1;
	volatile uint8_t _ALERT_LEAK:1;
	volatile uint8_t _ALERT_NC1:1;
	volatile uint8_t _ALERT_NC2:1;
	volatile uint8_t _ALERT_NC3:1;
	volatile uint8_t _ALERT_NC4:1;

} SECOND_FRAME ;


typedef struct __attribute__((packed)) {

	volatile uint8_t _ALERT_HIGH_PIP:1;
	volatile uint8_t _ALERT_LOW_PIP:1;
	volatile uint8_t _ALERT_HIGH_PEEP:1;
	volatile uint8_t _ALERT_LOW_PEEP:1;
	volatile uint8_t _ALERT_HIGH_TIDAL_VOLUME:1;
	volatile uint8_t _ALERT_LOW_TIDAL_VOLUME:1;
	volatile uint8_t _ALERT_HIGH_MINUTE_VOLUME:1;
	volatile uint8_t _ALERT_LOW_MINUTE_VOLUME:1;

} THIRD_FRAME ;


typedef struct __attribute__((packed)) {

	volatile uint8_t _ALERT_HIGH_FIO2:1;
	volatile uint8_t _ALERT_LOW_FIO2:1;
	volatile uint8_t _ALERT_HIGH_FLOW:1;
	volatile uint8_t _ALERT_LOW_FLOW:1;
	volatile uint8_t _ALERT_HIGH_PULSE:1;
	volatile uint8_t _ALERT_LOW_PULSE:1;
	volatile uint8_t _ALERT_HIGH_RR:1;
	volatile uint8_t _ALERT_LOW_RR:1;



} FOURTH_FRAME ;

typedef struct __attribute__((packed)) {

	volatile uint32_t _ALERT_NC:1;

} FIFTH_FRAME ;


typedef struct __attribute__((packed)) {

	volatile uint32_t _ALERT_NC:1;
} SIXTH_FRAME ;




typedef struct __attribute__((packed)) {
	uint16_t _header; // 2
    uint8_t  _length; // 1
	union {
	 volatile unsigned char FIRST_BYTES;
	 FIRST_FRAME FRAMEBits ;
	}FIRST_FRAME_UN;

	union {
	 volatile unsigned char SECOND_BYTES;
	 SECOND_FRAME FRAMEBits ;
	}SECOND_FRAME_UN;

	union {
	 volatile unsigned char THIRD_BYTES;
	 THIRD_FRAME FRAMEBits ;
	}THIRD_FRAME_UN;

	union {
	 volatile unsigned char FOURTH_BYTES;
	 FOURTH_FRAME FRAMEBits ;
	}FOURTH_FRAME_UN;

	union {
	 volatile unsigned char FIFTH_BYTES;
	 FIFTH_FRAME FRAMEBits ;
	}FIFTH_FRAME_UN;

	union {
	  volatile unsigned char SIXTH_BYTES;
	  SIXTH_FRAME FRAMEBits ;
	 }SIXTH_FRAME_UN;



	volatile uint8_t _CRC8;

}Device_Alert_Data_Send;


typedef struct  __attribute__((packed))
{
	uint16_t  _header ;
	uint8_t   _length ;
	uint8_t _RANGE_PIP_MIN;
	uint8_t _RANGE_PIP_MAX;

	uint16_t _RANGE_VT_MIN;
	uint16_t _RANGE_VT_MAX;

	uint8_t _RANGE_RR_MIN;
	uint8_t _RANGE_RR_MAX;

	uint8_t _RANGE_MINT_VOL_MIN;
	uint8_t _RANGE_MINT_VOL_MAX;

	uint8_t _RANGE_SPO2_MIN;


	uint8_t _RANGE_PULSE_MIN;
	uint8_t _RANGE_PULSE_MAX;

	uint8_t _RANGE_NC1;
	uint8_t _RANGE_NC2;
	uint8_t _RANGE_NC3;
	uint8_t _CRC8;
} ALERT_RANGE_PACKET;


typedef struct
{
	uint8_t  _RANGE_PIP_MIN_Val;
	uint8_t  _RANGE_PIP_MAX_Val;
	uint16_t _RANGE_VT_MIN_Val;
	uint16_t _RANGE_VT_MAX_Val;
	uint8_t  _RANGE_RR_MIN_Val;
	uint8_t  _RANGE_RR_MAX_Val;
	uint8_t _RANGE_MINT_VOL_MIN_Val;
	uint8_t _RANGE_MINT_VOL_MAX_Val;
	uint8_t  _RANGE_SPO2_MIN_Val;
	uint8_t  _RANGE_PULSE_MIN_Val;
	uint8_t  _RANGE_PULSE_MAX_Val;
	uint8_t  _RANGE_NC1;
	uint8_t  _RANGE_NC2;
	uint8_t  _RANGE_NC3;

}ALERT_RANGE;


typedef struct
{
	uint8_t PIP_Alert_Count;
	uint8_t PEEP_Alert_Count;
	uint8_t Tidal_Volume_Alert_Count;
	uint8_t FIO2_Supply_Alert_Count;
	uint8_t FIO2_Alert_Count;
	uint8_t Leak_Alert_Count;
	uint16_t Patient_Circuit_disconnected_Alert_Count;
	uint16_t Proximal_Flow_Sensor_reversed_Alert_Count;
}Alert_count;



typedef enum
{
	NO_ALERT,
	ALERT_HAPPEN
}alert_status;


typedef enum
{
	NO_APNEA_ALERT_HAPPEN,
	APNEA_ALERT_HAPPEN
}apnea_alert;


typedef enum
{
	SEND_GRAPH_FRAME,
	SEND_ALERT_FRAME,
	SEND_CALIBRATION_FRAME,
	SEND_SAMPLED_PARAMETER_FRAME,
	SEND_FUELGAUGE_PARAMETER_FRAME
}send_bluetooth_data_farme;


typedef enum
{
	CHECK,
	DO_NOT_CHECK

}PATIENT_CIRCUIT_DISCONNECTED_ALERT_check;






PATIENT_CIRCUIT_DISCONNECTED_ALERT_check PATIENT_CIRCUIT_DISCONNECTED_ALERT_CHECK,PROXIMAL_SENSOR_REVERSE_ALERT_CHECK;


Device_Alert_Data_Send DEVICE_ALERT_DATA_SEND ;
ALERT_RANGE Alert_Range;
Alert_count Alert_Count;
alert_status Alert_Status;
apnea_alert Apnea_Alert;
send_bluetooth_data_farme Send_Bluetooth_Data_Farme;
uint8_t Alert_Check;


xTaskHandle Alert_Handler;
void Alert_Task (void *argument);

void Split_Alert_Parameter_Packets(ALERT_RANGE_PACKET *Receive_Graph_Packet);
void Check_Alert();
void SEND_ALERT_PACKET();
void Pip_Alert(uint8_t);
void Alert_Inspiration_Time_Parameter();
void Clear_All_Alert_Bits();
void UART5_Alert_data_Debug();
void Peep_Alert(uint8_t);
void Tidal_Volume_Alert(int);
void Minite_Volume_Alert(int);
void Alert_Flags_Reset();
void Respiratory_Rate_Alert();
void Alert_Expiration_Time_Parameter();
void Oxygen_Supply_Alert(uint8_t,uint8_t);
void Oxygen_Blending_Alert(uint8_t,uint8_t);
void Patient_Circuit_Disconnected_Alert();
void Leak_Alert();
void Proximal_Flow_Sensor_Reverse_Direction();
void LED_Alert();
void AC_Supply_or_Battery_Supply_Indication();
void Hide_Alert_1();
void Hide_Alert_2();

#endif /* INC_ALERT_H_ */
