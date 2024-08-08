/*
 * Sampled_Parameter.h
 *
 *  Created on: May 23, 2023
 *      Author: asus
 */

#ifndef INC_AERO_INC_SAMPLED_PARAMETER_SAMPLED_PARAMETER_H_
#define INC_AERO_INC_SAMPLED_PARAMETER_SAMPLED_PARAMETER_H_

#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


int Max_Flow,Min_Flow;


typedef struct
{
	uint8_t Average_PIP;
	uint8_t Average_PEEP;
	float   Volume_Max_Insp;
	float   Volume_Max_Exp;
	uint8_t     MAX_Flow;
	uint8_t     MIN_Flow;
}sampled_Parameter_1;


typedef struct
{
  float   Minite_Volume;
  int     Total_Tidal_Volume_Int;
  float   Total_Tidal_Volume_Decimal_1;
  int     Total_Tidal_Volume_Decimal;
  float   Total_Tidal_Volume_float;
  uint8_t Acheived_RR;
  uint16_t Acheived_I_Time;
  uint16_t Acheived_E_Time;
  float   Acheived_I_Time_float;
  float   Acheived_E_Time_float;
  int     Acheived_I_Time_int;
  int     Acheived_E_Time_int;
  float   Acheived_E_Time_Decimal_1;
  int     Acheived_E_Time_Decimal;
  float   Acheived_I_Time_Decimal_1;
  int     Acheived_I_Time_Decimal;

}sampled_Parameter_2;



typedef struct
{
	uint8_t Compliance;
	uint8_t Plateau_Pressure;
	double Staic_Compliance;
	uint8_t Staic_Compliance_int;
	uint8_t Staic_Compliance_float;
	double Dynamic_Compliance;
	uint8_t Dynamic_Compliance_int;
	uint8_t Dynamic_Compliance_float;
	double Rinsp;
	uint8_t Rinsp_int;
	uint8_t Rinsp_float_1;
	uint8_t Rinsp_float_2;

}sampled_Parameter_3;

typedef struct  __attribute__((packed))
{
	uint16_t _header ;
	uint8_t  _length ;
	uint8_t  _PIP_Val;
	uint8_t  _PEEP_Val;
    uint8_t  _VTI_Val_1;
    uint8_t  _VTI_Val_2;
	uint8_t   _FIO2;
	uint8_t   _RR;
	uint8_t  _CRC8;
} SAMPLED_PARAMETER_PACKET_1 ;

typedef struct  __attribute__((packed))
{
	uint16_t _header ;
	uint8_t  _length ;
	uint8_t  _Minite_Volume_int;
	uint8_t  _Minite_Volume_float;
	uint8_t _Static_Compliance_Int;
	uint8_t _Static_Compliance_Float;
	uint8_t _Dynamic_Compliance_Int;
	uint8_t _Dynamic_Compliance_Float;
	uint8_t  _CRC8;
} SAMPLED_PARAMETER_PACKET_2 ;


typedef struct  __attribute__((packed))
{
	uint16_t _header ;
	uint8_t  _length ;
	uint8_t  _RInsp_Int;
	uint8_t  _RInsp_Float1;
	uint8_t _MAP_int;
	uint8_t _MAP_float;
	int16_t  _VTE_Val;
	uint8_t  _CRC8;
} SAMPLED_PARAMETER_PACKET_3 ;



typedef struct  __attribute__((packed))
{
	uint16_t _header ;
	uint8_t  _length ;
	uint8_t  _PIF_Val;
	uint8_t  _PEF_Val;
	uint16_t _I_time_Achieved;
	uint16_t _E_time_Achieved;
	uint8_t _CRC8;
} SAMPLED_PARAMETER_PACKET_4;



typedef struct  __attribute__((packed))
{
	uint16_t _header ;
	uint8_t  _length ;
	uint8_t  _Plauteu_Pressure;
	uint8_t _I_Ratio_int;
	uint8_t _I_Ratio_float;
	uint8_t _E_Ratio_int;
	uint8_t _E_Ratio_float;
    uint8_t  NC;
	uint8_t _CRC8;
} SAMPLED_PARAMETER_PACKET_5;



typedef struct
{
	double MAP;
	uint8_t MAP_int;
	uint8_t MAP_float;
	double I_Ratio;
	double E_Ratio;
	uint8_t I_Ratio_int;
	uint8_t E_Ratio_int;
	uint8_t I_Ratio_float;
	uint8_t E_Ratio_float;

}sampled_Parameter_4;

typedef enum
{
	First_Frame = 1,
	Second_Frame =2,
	Third_Frame  =3,
	Fourth_Frame =4,
	Fifth_Frame  =5
}sampled_parameter_packet;


uint8_t Send_Frame;
sampled_Parameter_1 Sampled_Parameter_1;
sampled_Parameter_2 Sampled_Parameter_2;
sampled_Parameter_3 Sampled_Parameter_3;
sampled_Parameter_4 Sampled_Parameter_4;
SAMPLED_PARAMETER_PACKET_1 Sampled_Parameter_1_Packet;
SAMPLED_PARAMETER_PACKET_2 Sampled_Parameter_2_Packet;
SAMPLED_PARAMETER_PACKET_3 Sampled_Parameter_3_Packet;
SAMPLED_PARAMETER_PACKET_4 Sampled_Parameter_4_Packet;
SAMPLED_PARAMETER_PACKET_5 Sampled_Parameter_5_Packet;


void Average_PIP_Func();
void Average_PEEP_Func();
void Volume_Max_Inspiration_Func();
void Volume_Max_Expiration_Func();
void Reset_Flag_Func();
void Peak_Flow_Inspiration_Func();
void Peak_Flow_Expiration_Func();
void Sampled_Parameter_Func_1();
void Sampled_Parameter_Func_2();
void Sampled_Parameter_Func_3();
void Minite_Volume_Func();
void Inspiration_Time_Func();
void Expiration_Time_Func();
void Sampled_Parameter_List_1_Transmit();
void Sampled_Parameter_List_2_Transmit();
void Sampled_Parameter_List_3_Transmit();
void Sampled_Parameter_List_4_Transmit();
void Sampled_Parameter_List_5_Transmit();
void Acheived_RR_Func();
void Platau_Pressure();
void Sampled_Parameter_Flags_Func();
void Static_and_Dynamic_Compliance();
void Inspiratory_Resistance();
void Mean_Airway_Pressure();
void Sampled_Parameter_Func_4();
void I_and_E_Ratio_Func();


void Sampled_Parameter_Inspiration_time();


#endif /* INC_AERO_INC_SAMPLED_PARAMETER_SAMPLED_PARAMETER_H_ */
