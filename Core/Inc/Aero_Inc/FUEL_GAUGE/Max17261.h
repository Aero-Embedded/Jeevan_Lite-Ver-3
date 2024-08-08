/*
 * Max17261.h
 *
 *  Created on: May 12, 2023
 *      Author: asus
 */

#ifndef INC_MAX17261_H_
#define INC_MAX17261_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"



extern I2C_HandleTypeDef hi2c3;

float repsoc;
float repcap;
float tte,ttf;
float Rsense;
float voltage;
float Average_Current,Current;
float Temperature;
int Age;


uint8_t Send_Frame_Fuel_Gauge;

#define	MAX1726X_I2C_ADDR					0x6C



#define MAX1726X_STATUS_REG                 0x00
#define MAX1726X_VALRTTH_REG                0x01
#define MAX1726X_TALRTTH_REG                0x02
#define MAX1726X_SALRTTH_REG                0x03
#define MAX1726X_ATRATE_REG                 0x04
#define MAX1726X_REPCAP_REG                 0x05
#define MAX1726X_REPSOC_REG                 0x06
#define MAX1726X_AGE_REG                    0x07
#define MAX1726X_TEMP_REG                   0x08
#define MAX1726X_VCELL_REG                  0x09
#define MAX1726X_CURRENT_REG                0x0A
#define MAX1726X_AVGCURRENT_REG             0x0B
#define MAX1726X_QRESIDUAL_REG              0x0C
#define MAX1726X_MIXSOC_REG                 0x0D
#define MAX1726X_AVSOC_REG                  0x0E
#define MAX1726X_REMCAP_REG                 0x0F

#define MAX1726X_FULLCAPREP_REG             0x10
#define MAX1726X_TTE_REG                    0X11
#define MAX1726X_QRTABLE00_REG              0x12
#define MAX1726X_FULLSOCTHR_REG             0x13
#define MAX1726X_RCELL_REG                  0x14
//                             reserved for 0x15
#define MAX1726X_AVGTA_REG                  0x16
#define MAX1726X_CYCLES_REG                 0x17
#define MAX1726X_DESIGNCAP_REG              0x18
#define MAX1726X_AVGVCELL_REG               0x19
#define MAX1726X_MAXMINTEMP_REG             0x1A
#define MAX1726X_MAXMINVOLT_REG             0x1B
#define MAX1726X_MAXMINCURR_REG             0x1C
#define MAX1726X_CONFIG_REG                 0x1D
#define MAX1726X_ICHGTERM_REG               0x1E
#define MAX1726X_AVCAP_REG                  0x1F

#define MAX1726X_TTF_REG                    0X20
#define MAX1726X_DEVNAME_REG                0x21
#define MAX1726X_QRTABLE10_REG              0x22
#define MAX1726X_FULLCAPNOM_REG             0x23
//                             reserved for 0x24
//                             reserved for 0x25
//                             reserved for 0x26
#define MAX1726X_AIN_REG                    0x27
#define MAX1726X_LEARNCFG_REG               0x28
#define MAX1726X_FLITERCFG_REG              0x29
#define MAX1726X_RELAXCFG_REG               0x2A
#define MAX1726X_MISCCFG_REG                0x2B
#define MAX1726X_TGAIN_REG                  0x2C
#define MAX1726X_TOFF_REG                   0x2D
#define MAX1726X_CGAIN_REG                  0x2E
#define MAX1726X_COFF_REG                   0x2F

//                             reserved for 0x30
//                             reserved for 0x31
#define MAX1726X_QRTABLE20_REG              0x32
//                             reserved for 0x33
#define MAX1726X_DIETEMP_REG                0x34
#define MAX1726X_FULLCAP_REG                0x35
//                             reserved for 0x36
//                             reserved for 0x37
#define MAX1726X_RCOMP0_REG                 0x38
#define MAX1726X_TEMPCO_REG                 0x39
#define MAX1726X_VEMPTY_REG                 0x3A
//                             reserved for 0x3B
//                             reserved for 0x3C
#define MAX1726X_FSTAT_REG                  0x3D
#define MAX1726X_TIMER_REG                  0x3E
#define MAX1726X_SHDNTIMER_REG              0x3F

//                             reserved for 0x40
//                             reserved for 0x41
#define MAX1726X_QRTABLE30_REG              0x42
#define MAX1726X_RGAIN_REG                  0x43
//                             reserved for 0x44
#define MAX1726X_DQACC_REG                  0x45
#define MAX1726X_DPACC_REG                  0x46
//                             reserved for 0x47
//                             reserved for 0x48
#define MAX1726X_CONVGCFG_REG               0x49
#define MAX1726X_VFREMCAP_REG               0x4A
//                             reserved for 0x4B
//                             reserved for 0x4C
#define MAX1726X_QH_REG                     0x4D
//                             reserved for 0x4E
//                             reserved for 0x4F

#define MAX1726X_MODELDATA0_START_REG       0x80
#define MAX1726X_MODELDATA1_START_REG       0x90

#define MAX1726X_STATUS2_REG                0xB0
#define MAX1726X_POWER_REG                  0xB1
#define MAX1726X_ID_USERMEM2_REG            0xB2
#define MAX1726X_AVGPOWER_REG               0xB3
#define MAX1726X_IALRTTH_REG                0xB4
#define MAX1726X_TTFCFG_REG                 0xB5
#define MAX1726X_CVMIXCAP_REG               0xB6
#define MAX1726X_CVHALFIME_REG              0xB7
#define MAX1726X_CGTEMPCO_REG               0xB8
#define MAX1726X_CURVE_REG                  0xB9
#define MAX1726X_HIBCFG_REG                 0xBA
#define MAX1726X_CONFIG2_REG                0xBB
#define MAX1726X_VRIPPLE_REG                0xBC
#define MAX1726X_RIPPLECFG_REG              0xBD
#define MAX1726X_TIMERH_REG                 0xBE
//                             reserved for 0xBF

#define MAX1726X_RSENSE_USERMEM3_REG        0xD0
#define MAX1726X_SCOCVLIM_REG               0xD1
#define MAX1726X_VGAIN_REG                  0xD2
#define MAX1726X_SOCHOLD_REG                0xD3
#define MAX1726X_MAXPEAKPOWER_REG           0xD4
#define MAX1726X_SUSPEAKPOWER_REG           0xD5
#define MAX1726X_PACKRESISTANCE_REG         0xD6
#define MAX1726X_SYSRESISTANCE_REG          0xD7
#define MAX1726X_MINSYSVOLTAGE_REG          0xD8
#define MAX1726X_MPPCURRENT_REG             0xD9
#define MAX1726X_SPPCURRENT_REG             0xDA
#define MAX1726X_MODELCFG_REG               0xDB
#define MAX1726X_ATQRESIDUAL_REG            0xDC
#define MAX1726X_ATTTE_REG                  0xDD
#define MAX1726X_ATAVSOC_REG                0xDE
#define MAX1726X_ATAVCAP_REG                0xDF

#define MAX1726X_SERIALNUM0					0xD4
#define MAX1726X_SERIALNUM1					0xD5
#define MAX1726X_SERIALNUM2					0xD9
#define MAX1726X_SERIALNUM3					0xDA
#define MAX1726X_SERIALNUM4					0xDC
#define MAX1726X_SERIALNUM5					0xDD
#define MAX1726X_SERIALNUM6					0xDE
#define MAX1726X_SERIALNUM7					0xDF

#define MAX1726X_VFOCV_REG                  0xFB
#define MAX1726X_VFSOC_REG                  0xFF




#define START_MARK  0xAA
#define STOP_MARK   0x55

#define MAX_LENGTH      8
#define DEFAULT_LENGTH  8

#define WRITE  0x00
#define READ   0x01


#define MAX14748_REG  0x00
#define MAX17261_REG  0x01
#define MAX17261_SN   0x02
#define IO_CONTROLS   0x03
#define VERSIONS      0x04


#define CDIR_INDICATOR        0x00
#define SYSOK_INDICATOR       0x01
#define INT_INDICATOR         0x02
#define FLTIN_SET             0x03
#define FSUS_SET              0x04
#define ALERT_INDICATOR       0x05

#define VERSION_HW	0x10	// Hardware version = V1.0
#define VERSION_FW	0x10	// Firmware version = V1.0


extern uint16_t max1726x_regs[256];




typedef struct
{
	uint16_t designcap;
	uint16_t ichgterm;
	uint16_t modelcfg;
	uint16_t vempty;

} max1726x_ez_config_t;

typedef enum
{
	FUEL_GAUGE_MODULE_WORKING,
	FUEL_GAUGE_MODULE_FAULT
}FUEL_GAUGE_CONDITION;


typedef struct
{
	uint16_t _0;
	uint16_t _3_2;
}STEP;


typedef enum
{
	ZERO,
	ONE
}Result;


typedef struct  __attribute__((packed))
{
	uint16_t _header ;
	uint8_t  _length ;
	uint8_t  _Volt_Int;
	uint8_t  _Volt_Float;
	int16_t  _Cap_Int;
	uint8_t  _Cap_float;
	uint8_t  _TTE_Int;
	uint8_t  _TTE_float;
	uint8_t  _Age;
	uint8_t  _CRC8;
}FUEL_GAUGE_PARAMETER_PACKET_1;

typedef struct  __attribute__((packed))
{
	uint16_t _header ;
	uint8_t  _length ;
	uint8_t  _TTF_Int;
	uint8_t  _TTF_float;
	int8_t    _Avg_Current_Int;
	int8_t    _Avg_Current_float;
	int8_t    _Current_Int;
	int8_t    _Current_float;
	uint8_t  _Temp_Int;
	uint8_t  _Temp_Float;
	uint8_t  _CRC8;
}FUEL_GAUGE_PARAMETER_PACKET_2;


STEP Step;

max1726x_ez_config_t max1726x_ez_config;
uint16_t max1726x_serialnum[8];
uint8_t command_packet[MAX_LENGTH];

FUEL_GAUGE_CONDITION FUEL_GAUGE;
FUEL_GAUGE_PARAMETER_PACKET_1 Fuel_Gauge_Parameter_1_Packet;
FUEL_GAUGE_PARAMETER_PACKET_2 Fuel_Gauge_Parameter_2_Packet;


xTaskHandle Fuel_Gauge_Handler;
void FUEL_GAUGE_Task(void *argument);


uint8_t maxim_max1726x_check_por(void);
void maxim_max1726x_read_reg(uint8_t reg_addr, uint16_t *reg_data);
void max17261_i2c_write(uint8_t slave_addr, uint8_t *data, uint8_t data_len, uint8_t restart);
void max17261_i2c_read(uint8_t slave_addr, uint8_t *data, uint8_t data_len, uint8_t restart);
void maxim_max1726x_wait_dnr(void);
void maxim_max1726x_initialize_ez_config(void);
void maxim_max1726x_write_reg(uint8_t reg_addr, uint16_t *reg_data);
uint8_t maxim_max1726x_clear_por(void);
uint8_t maxim_max1726x_write_and_verify_reg(uint8_t reg_addr, uint16_t *reg_data);
void maxim_max1726x_get_serial_number(uint16_t *sn);
float maxim_max1726x_get_repcap(float Rsense);
float maxim_max1726x_get_repsoc(void);
float maxim_max1726x_get_tte(void);
float maxim_max1726x_get_ttf(void);
float maxim_max1726x_get_Avg_Current(float);
float maxim_max1726x_get_Temperature();
uint8_t max1726x_alert_get(void);
float maxim_max1726x_get_voltage(void);
float maxim_max1726x_get_Current(float Rsense);
float maxim_max1726x_get_Age(float Rsense);
void Fuel_Gauge_Parameter_List_1_Transmit();
void Fuel_Gauge_Parameter_List_2_Transmit();
void Volatge_Convert();
void repcap_Convert();
void repsoc_Convert();
void tte_Convert();
void ttf_Convert();
void Average_Current_Convert();
void Current_Convert();
void Temperature_Convert();

int Do_Step_0();
void Do_Step_1();
void Do_Step_2_Point_1();
void Do_Step_3();
void Do_Step_3_Point_2();
void Do_Step_3_Point_3();
void Fuel_Gauge_Detection();
void Fuel_Gauge_Init();
void Convert_Battery_Percentage();

#endif /* INC_MAX17261_H_ */
