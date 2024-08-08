/*
 * Calibration.h
 *
 *  Created on: Mar 16, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_CALIBRATION_CALIBRATION_H_
#define INC_AERO_INC_CALIBRATION_CALIBRATION_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



typedef struct __attribute__((packed))
{
	uint16_t  _header ;
	uint8_t   _length ;
	uint8_t   BLOWER_CALIBRATION;
	uint8_t   PRESSURE_SENSOR_CALIBRATION;
	uint8_t   FLOW_SENSOR_7002_CALIBRATION;
	uint8_t   VALVE_LEAK_TEST_CALIBRATION;
	uint8_t   O2_CHECK_CALIBRATION;
	uint8_t   LED_ALARAM_CALIBRATION;
	uint8_t   BATTERY_CALIBRATION;
	uint8_t   SENSOR_OFFSET_CALIBRATION;
	uint8_t   O2_SENSOR_100_PER_CALIBRATE;
	uint8_t   NC3;
	uint8_t   NC4;
	uint8_t   NC5;
	uint8_t   NC6;
	uint8_t   NC7;
	uint8_t   NC8;
	uint8_t   NC9;
	uint8_t   _CRC8;

} CALIBRATION_PACKET;


typedef struct __attribute__((packed))
{
	    uint16_t _header ;
		uint8_t  _length ;
		uint8_t   PASS_FAIL;
		uint8_t   ERROR_CODE;
		uint8_t   STAGE;
		uint16_t  RESULT;
		uint8_t   NC;
		uint8_t   _CRC8;

} Send_Calibration_Packet;



typedef struct
{
	uint8_t BLOWER;
	uint8_t	PRESSURE_SENSOR;
	uint8_t	FLOW_SENSOR_7002;
	uint8_t	VALVE_LEAK_TEST ;
	uint8_t	O2_CHECK ;
	uint8_t	ALARAM_TEST;
	uint8_t	BATTERY_TEST;
	uint8_t SENSOR_OFFSET_TEST;
	uint8_t O2_SENSOR_100_PER_TEST;
}Calibration;


typedef enum
{
	STOP,
	START
}Calibration_Command;

typedef enum
{
	CALIBRATION_FAIL          = 0X00,
	CALIBRATION_PASS          = 0XFF,

}Calibrated_Result;

xTaskHandle Calibration_Handler;
void CALIBRATION_Task(void *argument);

Send_Calibration_Packet SEND_CALIBRATION_PACKET;
Calibration CALIBRARTION;

void Split_Calibration_Packets(CALIBRATION_PACKET *);
void Blower_Calibration();
void Send_Calibrated_Packet();
uint8_t chksum8_cal(const unsigned char *, size_t);

void Pressure_Sensor_Calibration();
void Flow_Sensor_7002_Calibration();
void Expiratory_Valve_Leak_Calibration();
void Oxygen_blending_Calibration();
void Led_Buzzer_Calibration();
void Battery_Calibration();
void Call_Battery();
void Sensor_Voltage_Transmit();
void O2_Calibration_Task(void *argument);

extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

#endif /* INC_AERO_INC_CALIBRATION_CALIBRATION_H_ */
