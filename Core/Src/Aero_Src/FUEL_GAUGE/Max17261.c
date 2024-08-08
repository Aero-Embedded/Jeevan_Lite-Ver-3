/*
 * Max17261.c
 *
 *  Created on: May 12, 2023
 *      Author: asus
 */


#include "Max17261.h"




uint16_t max1726x_regs[256];
uint8_t ret;
//|Batter voltage
uint8_t Volt_Int;
uint8_t Volt_Float;

uint16_t repcap_Int;
uint8_t  repcap_Float;
uint8_t repsoc_Int;
uint8_t repsoc_Float;

uint8_t tte_Int;
uint8_t tte_Float;


uint8_t ttf_Int;
uint8_t ttf_Float;

int8_t Average_Current_Int;
int8_t Average_Current_Float;

int8_t Current_Int;
int8_t Current_Float;

uint8_t Temperature_Int;
uint8_t Temperature_Float;

float Battery_Raw_Value;
extern uint8_t Toggle;
uint8_t Battery_Voltage_old=100,Battery_Voltage_new=0;

/* ************************************************************************* */

void max17261_i2c_write(uint8_t slave_addr, uint8_t *data, uint8_t data_len, uint8_t restart)
{
	ret = HAL_I2C_Master_Transmit(&hi2c3, slave_addr, data, data_len, restart);
	if (ret != HAL_OK)
	{
		FUEL_GAUGE = FUEL_GAUGE_MODULE_FAULT;

	}
	else if(ret == HAL_OK)
	{
		FUEL_GAUGE = FUEL_GAUGE_MODULE_WORKING;

	}
}

/* ************************************************************************* */

void max17261_i2c_read(uint8_t slave_addr, uint8_t *data, uint8_t data_len, uint8_t restart)
{
	HAL_I2C_Master_Receive(&hi2c3, (slave_addr),data,data_len,restart);
}

/* ************************************************************************* */
uint8_t maxim_max1726x_check_por(void)
{
	maxim_max1726x_read_reg(MAX1726X_STATUS_REG, &max1726x_regs[MAX1726X_STATUS_REG]);

	if((max1726x_regs[MAX1726X_STATUS_REG] & 0x0002) == 0x0000)
	{
		return 0;	// No power on reset
	}
	else
	{
		return 1;	// Power on reset
	}
}

/* ************************************************************************* */

void maxim_max1726x_read_reg(uint8_t reg_addr, uint16_t *reg_data)
{
	uint8_t i2c_data[2];

	i2c_data[0] = reg_addr;
	max17261_i2c_write(MAX1726X_I2C_ADDR, i2c_data, 1, 2);
	vTaskDelay(1);
	max17261_i2c_read(MAX1726X_I2C_ADDR, i2c_data, 2, 1);
	vTaskDelay(1);
	*reg_data = i2c_data[1];
	*reg_data = ((*reg_data)<<8) | i2c_data[0];
}

/* ************************************************************************* */

void maxim_max1726x_wait_dnr(void)
{
	maxim_max1726x_read_reg(MAX1726X_FSTAT_REG, &max1726x_regs[MAX1726X_FSTAT_REG]);
	vTaskDelay(10);
	while((max1726x_regs[MAX1726X_FSTAT_REG] & 0x0001) == 0x0001)
	{
		vTaskDelay(10);
		maxim_max1726x_read_reg(MAX1726X_FSTAT_REG, &max1726x_regs[MAX1726X_FSTAT_REG]);
	}
	vTaskDelay(10);
}


/* ************************************************************************* */

void maxim_max1726x_initialize_ez_config(void)
{
	uint16_t tempdata;

	/// customer must provide the battery parameters accordingly
	/// here the values are default for two serials of 18650 bat
	max1726x_ez_config.designcap  = 0x27d8;
	max1726x_ez_config.ichgterm   = 0x0500;
	max1726x_ez_config.modelcfg   = 0x8000;
	max1726x_ez_config.vempty     = 0xAA61;
	/// customer must provide the battery parameters accordingly


	/// Store original HibCFG value
	maxim_max1726x_read_reg(MAX1726X_HIBCFG_REG, &max1726x_regs[MAX1726X_HIBCFG_REG]);

	/// Exit Hibernate Mode step
	tempdata = 0x0090;
	maxim_max1726x_write_reg(0x60, &tempdata);
	tempdata = 0x0000;
	maxim_max1726x_write_reg(MAX1726X_HIBCFG_REG, &tempdata);
	maxim_max1726x_write_reg(0x60, &tempdata);

	/// OPTION 1 EZ Config (No INI file is needed)
	max1726x_regs[MAX1726X_DESIGNCAP_REG] = max1726x_ez_config.designcap;
	max1726x_regs[MAX1726X_ICHGTERM_REG]  = max1726x_ez_config.ichgterm;
	max1726x_regs[MAX1726X_VEMPTY_REG]    = max1726x_ez_config.vempty;
	max1726x_regs[MAX1726X_MODELCFG_REG]  = max1726x_ez_config.modelcfg;

	maxim_max1726x_write_reg(MAX1726X_DESIGNCAP_REG, &max1726x_regs[MAX1726X_DESIGNCAP_REG]);
	vTaskDelay(10);
	maxim_max1726x_write_reg(MAX1726X_ICHGTERM_REG, &max1726x_regs[MAX1726X_ICHGTERM_REG]);
	vTaskDelay(10);
	maxim_max1726x_write_reg(MAX1726X_VEMPTY_REG, &max1726x_regs[MAX1726X_VEMPTY_REG]);
	vTaskDelay(10);
	maxim_max1726x_write_reg(MAX1726X_MODELCFG_REG, &max1726x_regs[MAX1726X_MODELCFG_REG]);
	vTaskDelay(10);

	//Poll ModelCFG.Refresh bit, do not continue until ModelCFG.Refresh==0
	maxim_max1726x_read_reg(MAX1726X_MODELCFG_REG, &max1726x_regs[MAX1726X_MODELCFG_REG]);

	while( (max1726x_regs[MAX1726X_MODELCFG_REG] & 0x8000)  != 0x8000)
	{
		vTaskDelay(10);
		maxim_max1726x_read_reg(MAX1726X_MODELCFG_REG, &max1726x_regs[MAX1726X_MODELCFG_REG]);
	}

	/// Restore Original HibCFG value
	maxim_max1726x_write_reg(MAX1726X_HIBCFG_REG, &max1726x_regs[MAX1726X_HIBCFG_REG]);
	vTaskDelay(10);

}
/* ************************************************************************* */




void maxim_max1726x_write_reg(uint8_t reg_addr, uint16_t *reg_data)
{
	uint8_t i2c_data[3];

	i2c_data[0] = reg_addr;
	i2c_data[1] = (*reg_data) & 0xFF;
	i2c_data[2] = (*reg_data) >> 8;
	max17261_i2c_write(MAX1726X_I2C_ADDR, i2c_data, 3, 10);
	vTaskDelay(1);
}



/* ************************************************************************* */
uint8_t maxim_max1726x_clear_por(void)
{
	maxim_max1726x_read_reg(MAX1726X_STATUS_REG, &max1726x_regs[MAX1726X_STATUS_REG]);

	max1726x_regs[MAX1726X_STATUS_REG] = max1726x_regs[MAX1726X_STATUS_REG] & 0xFFFD;

	return maxim_max1726x_write_and_verify_reg(MAX1726X_STATUS_REG, &max1726x_regs[MAX1726X_STATUS_REG]);
}



/* ************************************************************************* */


uint8_t maxim_max1726x_write_and_verify_reg(uint8_t reg_addr, uint16_t *reg_data)
{
	uint8_t i2c_data[3];
	uint16_t readback_data;
	int8_t retry;

	retry = 3;

	while(retry>0)
	{
		i2c_data[0] = reg_addr;
		i2c_data[1] = (*reg_data) & 0xFF;
		i2c_data[2] = (*reg_data) >> 8;
		max17261_i2c_write(MAX1726X_I2C_ADDR, i2c_data, 3, 2);

		vTaskDelay(10);

		i2c_data[0] = reg_addr;
		max17261_i2c_write(MAX1726X_I2C_ADDR, i2c_data, 1, 2);

		i2c_data[0] = 0x00;
		i2c_data[1] = 0x00;
		max17261_i2c_read(MAX1726X_I2C_ADDR, i2c_data, 2, 1);
		vTaskDelay(1);
		readback_data = i2c_data[1];
		readback_data = (readback_data<<8) | i2c_data[0];

		if(readback_data == (*reg_data))
		{
			return 0; 	// no error
		}
		else
		{
			retry--;
		}
	}

	return 1;	// error
}


/************************************************************************** */

void maxim_max1726x_get_serial_number(uint16_t *sn)
{

	maxim_max1726x_read_reg(MAX1726X_CONFIG2_REG, &max1726x_regs[MAX1726X_CONFIG2_REG]);

	// clear AtRateEn bit and DPEn bit in Config2 register
	max1726x_regs[MAX1726X_CONFIG2_REG] = max1726x_regs[MAX1726X_CONFIG2_REG] & 0xCFFF;
	maxim_max1726x_write_and_verify_reg(MAX1726X_CONFIG2_REG, &max1726x_regs[MAX1726X_CONFIG2_REG]);
	vTaskDelay(40);

	maxim_max1726x_read_reg(MAX1726X_MAXPEAKPOWER_REG, &max1726x_regs[MAX1726X_MAXPEAKPOWER_REG]);
	maxim_max1726x_read_reg(MAX1726X_SUSPEAKPOWER_REG, &max1726x_regs[MAX1726X_SUSPEAKPOWER_REG]);
	maxim_max1726x_read_reg(MAX1726X_MPPCURRENT_REG, &max1726x_regs[MAX1726X_MPPCURRENT_REG]);
	maxim_max1726x_read_reg(MAX1726X_SPPCURRENT_REG, &max1726x_regs[MAX1726X_SPPCURRENT_REG]);
	maxim_max1726x_read_reg(MAX1726X_ATQRESIDUAL_REG, &max1726x_regs[MAX1726X_ATQRESIDUAL_REG]);
	maxim_max1726x_read_reg(MAX1726X_ATTTE_REG, &max1726x_regs[MAX1726X_ATTTE_REG]);
	maxim_max1726x_read_reg(MAX1726X_ATAVSOC_REG, &max1726x_regs[MAX1726X_ATAVSOC_REG]);
	maxim_max1726x_read_reg(MAX1726X_ATAVCAP_REG, &max1726x_regs[MAX1726X_ATAVCAP_REG]);


	sn[0] = max1726x_regs[MAX1726X_MAXPEAKPOWER_REG];
	sn[1] = max1726x_regs[MAX1726X_SUSPEAKPOWER_REG];
	sn[2] = max1726x_regs[MAX1726X_MPPCURRENT_REG];
	sn[3] = max1726x_regs[MAX1726X_SPPCURRENT_REG];
	sn[4] = max1726x_regs[MAX1726X_ATQRESIDUAL_REG];
	sn[5] = max1726x_regs[MAX1726X_ATTTE_REG];
	sn[6] = max1726x_regs[MAX1726X_ATAVSOC_REG];
	sn[7] = max1726x_regs[MAX1726X_ATAVCAP_REG];


	// set AtRateEn bit and DPEn bit in Config2 register
	max1726x_regs[MAX1726X_CONFIG2_REG] = max1726x_regs[MAX1726X_CONFIG2_REG] | 0x3000;
	maxim_max1726x_write_and_verify_reg(MAX1726X_CONFIG2_REG, &max1726x_regs[MAX1726X_CONFIG2_REG]);
	vTaskDelay(40);
}


/************************************************************************** */

float maxim_max1726x_get_repcap(float Rsense)
{
	float repcap;
	maxim_max1726x_read_reg(MAX1726X_REPCAP_REG, &max1726x_regs[MAX1726X_REPCAP_REG]);

	repcap = ( ((float)max1726x_regs[MAX1726X_REPCAP_REG] * 5.0f) / (float)Rsense) /1000;

	return repcap;
}

/* ************************************************************************* */
float maxim_max1726x_get_repsoc(void)
{
	float repsoc;
	maxim_max1726x_read_reg(MAX1726X_REPSOC_REG, &max1726x_regs[MAX1726X_REPSOC_REG]);

	repsoc = ((float)max1726x_regs[MAX1726X_REPSOC_REG] / 256.0f);

	if(repsoc > 100)
		repsoc =100;
	return repsoc;
}

/* ************************************************************************* */

float maxim_max1726x_get_voltage(void)
{
	float voltage;

	maxim_max1726x_read_reg(MAX1726X_VCELL_REG, &max1726x_regs[MAX1726X_VCELL_REG]);

	voltage = ( ( ( (float)max1726x_regs[MAX1726X_VCELL_REG] * (1.25) ) /16) /1000 );
	return voltage;
}

/************************************************************************** */
float maxim_max1726x_get_tte(void)
{

		float tte;
		maxim_max1726x_read_reg(MAX1726X_TTE_REG, &max1726x_regs[MAX1726X_TTE_REG]);

		tte = ( (float)max1726x_regs[MAX1726X_TTE_REG] /640);
		return tte;

}

/************************************************************************** */
float maxim_max1726x_get_ttf(void)
{
	float ttf;
	maxim_max1726x_read_reg(MAX1726X_TTF_REG, &max1726x_regs[MAX1726X_TTF_REG]);

	ttf = ( (float)max1726x_regs[MAX1726X_TTF_REG] /640);
	return ttf;
}

/************************************************************************** */


float maxim_max1726x_get_Avg_Current(float Rsense)
{
	float Average_Current;
	maxim_max1726x_read_reg(MAX1726X_AVGCURRENT_REG, &max1726x_regs[MAX1726X_AVGCURRENT_REG]);

	if(ttf >=102)
	{
		Average_Current = ( ( (65535 -(float)max1726x_regs[MAX1726X_AVGCURRENT_REG] )* (-1.5625f) ) / (float)Rsense) /1000 ;
	}
	else
	{
		Average_Current = ( ((float)max1726x_regs[MAX1726X_AVGCURRENT_REG] * 1.5625f) / (float)Rsense) /1000 ;
	}
	return Average_Current;
}

/***************************************************************************/

float maxim_max1726x_get_Current(float Rsense)
{
	float Current;
	maxim_max1726x_read_reg(MAX1726X_CURRENT_REG, &max1726x_regs[MAX1726X_CURRENT_REG]);

	if(ttf >=102)
	{
		Current = ( ( (65535 -(float)max1726x_regs[MAX1726X_CURRENT_REG] )* (-1.5625f) ) / (float)Rsense) /1000 ;
	}
	else
	{
		Current = ( ((float)max1726x_regs[MAX1726X_CURRENT_REG] * 1.5625f) / (float)Rsense) /1000 ;
	}
	return Current;
}

/************************************************************************** */


float maxim_max1726x_get_Temperature(void)
{
	float Temperature;
	maxim_max1726x_read_reg(MAX1726X_TEMP_REG, &max1726x_regs[MAX1726X_TEMP_REG]);

	Temperature = (max1726x_regs[MAX1726X_TEMP_REG] / 256.0f);
	return Temperature;
}

/************************************************************************** */


float maxim_max1726x_get_Age(float Rsense)
{
	int FullCapRep,Age;
	maxim_max1726x_read_reg(MAX1726X_FULLCAPNOM_REG, &max1726x_regs[MAX1726X_FULLCAPNOM_REG]);

	FullCapRep = ((max1726x_regs[MAX1726X_FULLCAPNOM_REG] * 5.0f) / (float)Rsense) /1000;

	Age = ((float)FullCapRep/5100) *100 ;

	//maxim_max1726x_read_reg(MAX1726X_AGE_REG, &max1726x_regs[MAX1726X_AGE_REG]);

	return Age;
}

/***************************************************************************/

void Fuel_Gauge_Init()
{
	Rsense=0.01;
	voltage=0;

	Step._0 = Do_Step_0();
	vTaskDelay(100);

	if(Step._0 == ZERO)
	{
		Do_Step_3_Point_2();
	}
	else
	{
		Do_Step_1();
		Do_Step_2_Point_1();
		Do_Step_3();

		    Step._0 = Do_Step_0();
			vTaskDelay(100);

				if(Step._0 == ZERO)
				{
					Do_Step_3_Point_2();
				}

				xSemaphoreGive(binarysem);
	}
}

/************************************************************************** */



int Do_Step_0()
{
	uint8_t Val;
	Val = maxim_max1726x_check_por();
	return Val;
}

/************************************************************************** */

void Do_Step_1()
{
	maxim_max1726x_wait_dnr();
}

/************************************************************************** */

void Do_Step_2_Point_1()
{
	maxim_max1726x_initialize_ez_config();
}

/************************************************************************** */

void Do_Step_3()
{
	maxim_max1726x_clear_por();
}

/************************************************************************** */

void Do_Step_3_Point_2()
{
	Step._3_2 = Do_Step_0();
	vTaskDelay(100);
	if(Step._3_2 == ZERO)
	{
		//Do_Step_3_Point_3();
		xSemaphoreGive(binarysem);
	}
	else
	{
		Fuel_Gauge_Init();
	}
}



/************************************************************************** */




void FUEL_GAUGE_Task(void *argument)
{
	while(1)
	{
		  /*repcap = maxim_max1726x_get_repcap(Rsense);
		  repcap_Convert();
		  vTaskDelay(10);
		  repsoc = maxim_max1726x_get_repsoc();
		  repsoc_Convert();
		  vTaskDelay(10);
		  voltage = maxim_max1726x_get_voltage();
		  Volatge_Convert();
		  vTaskDelay(10);

		  tte = maxim_max1726x_get_tte();
		  tte_Convert();
		  vTaskDelay(10);

		  ttf = maxim_max1726x_get_ttf();
		  ttf_Convert();
		  vTaskDelay(10);
		  Average_Current = maxim_max1726x_get_Avg_Current(Rsense);
		  Average_Current_Convert();
		  vTaskDelay(10);
		  Current = maxim_max1726x_get_Current(Rsense);
		  Current_Convert();
		  vTaskDelay(10);
		  Temperature = maxim_max1726x_get_Temperature();
		  Temperature_Convert();
		  vTaskDelay(10);
		  Age = maxim_max1726x_get_Age(Rsense);*/


        if( (Running_Mode == Aprv)  )
        {
             if((BREATH_STATE == EXPIRATION_CYCLE))
             {
				Battery_Raw_Value  = ((Adc.AdcData[3]-2250)*100)/(750.0);
				if(Battery_Raw_Value>100)
				{
					Battery_Raw_Value=100;
				}
				Bat_Avg += Battery_Raw_Value;
				Bat_Avg_count++;
             }
        }
        else if(Run_Current_Breathe_State == Run_Expiration_Cycle)
        {

			Battery_Raw_Value  = ((Adc.AdcData[3]-2250)*100)/(750.0);
			if(Battery_Raw_Value>100)
			{
				Battery_Raw_Value=100;
			}
			Bat_Avg += Battery_Raw_Value;
			Bat_Avg_count++;
        }
        else if(Run_Current_Breathe_State == No_Run_State)
        {

			Battery_Raw_Value  = ((Adc.AdcData[3]-2250)*100)/(750.0);
			if(Battery_Raw_Value>100)
			{
				Battery_Raw_Value=100;
			}
			Bat_Avg += Battery_Raw_Value;
			Bat_Avg_count++;

			if(Bat_Avg_count > 1000)
			{
				AC_Supply_or_Battery_Supply_Indication();
				Convert_Battery_Percentage();
			}
        }


		vTaskDelay(10);
	}
}

void Convert_Battery_Percentage()
{
	if(Toggle == 0)
	{
		Battery_Voltage =  (uint8_t) (Bat_Avg / Bat_Avg_count);
		Battery_Voltage_old = Battery_Voltage;


		if(Battery_Voltage_new > Battery_Voltage)
		{
			Battery_Voltage = Battery_Voltage_new;
		}

		Battery_Voltage_new = Battery_Voltage;

	}
	else if(Toggle == 1)
	{
		Battery_Voltage =  (uint8_t) (Bat_Avg / Bat_Avg_count);
		if(Battery_Voltage_old < Battery_Voltage)
		{
			Battery_Voltage = Battery_Voltage_old;
		}
		Battery_Voltage_old = Battery_Voltage;
		Battery_Voltage_new = 0;
	}
	Bat_Avg         = 0;
	Bat_Avg_count   = 0;
	Send_Bluetooth_Data_Farme = SEND_SAMPLED_PARAMETER_FRAME;
	Send_Frame                = First_Frame;
}



void repcap_Convert()
{
	repcap_Int = repcap;
	double dummy,dummy2;
	dummy2 = modf(repcap,&dummy);
	dummy2 =dummy2 * 100;
	repcap_Float = (int)dummy2 ;
}


void repsoc_Convert()
{
	repsoc_Int = repsoc;
	double dummy,dummy2;
	dummy2 = modf(repsoc,&dummy);
	dummy2 =dummy2 * 100;
	repsoc_Float = (int)dummy2 ;
}

void Volatge_Convert()
{
	Volt_Int = voltage;
	double dummy,dummy2;
	dummy2 = modf(voltage,&dummy);
	dummy2 =dummy2 * 100;
	Volt_Float = (int)dummy2 ;
}


void tte_Convert()
{
	tte_Int = tte;
	double dummy,dummy2;
	dummy2 = modf(tte,&dummy);
	dummy2 =dummy2 * 100;
	tte_Float = (int)dummy2 ;
}


void ttf_Convert()
{
	ttf_Int = tte;
	double dummy,dummy2;
	dummy2 = modf(ttf,&dummy);
	dummy2 =dummy2 * 100;
	ttf_Float = (int)dummy2 ;
}


void Average_Current_Convert()
{
	Average_Current_Int = Average_Current;
	double dummy,dummy2;
	dummy2 = modf(Average_Current,&dummy);
	dummy2 =dummy2 * 100;
	Average_Current_Float = (int)dummy2 ;
}


void Current_Convert()
{
	Current_Int = Current;
	double dummy,dummy2;
	dummy2 = modf(Current,&dummy);
	dummy2 =dummy2 * 100;
	Current_Float = (int)dummy2 ;
}


void Temperature_Convert()
{
	Temperature_Int = Current;
	double dummy,dummy2;
	dummy2 = modf(Temperature,&dummy);
	dummy2 =dummy2 * 100;
	Temperature_Float = (int)dummy2 ;
}

void Fuel_Gauge_Parameter_List_1_Transmit()
{
	Fuel_Gauge_Parameter_1_Packet._header        = 0x5064 ;
	Fuel_Gauge_Parameter_1_Packet._length        = sizeof(Fuel_Gauge_Parameter_1_Packet)-4 ;
	Fuel_Gauge_Parameter_1_Packet._Volt_Int      = Volt_Int;
	Fuel_Gauge_Parameter_1_Packet._Volt_Float    = Volt_Float;
	Fuel_Gauge_Parameter_1_Packet._Cap_Int       = repcap_Int;
	Fuel_Gauge_Parameter_1_Packet._Cap_float     = repcap_Float;
	Fuel_Gauge_Parameter_1_Packet._TTE_Int       = tte_Int;
	Fuel_Gauge_Parameter_1_Packet._TTE_float     = tte_Float;
	Fuel_Gauge_Parameter_1_Packet._Age           = Age;
	Fuel_Gauge_Parameter_1_Packet._CRC8          = chksum8(&Fuel_Gauge_Parameter_1_Packet._Volt_Int,(Fuel_Gauge_Parameter_1_Packet._length));

#ifdef	UART_COMMUNICATION_1
	HAL_UART_Transmit_IT(&huart6,(uint8_t*)&Fuel_Gauge_Parameter_1_Packet,sizeof(Fuel_Gauge_Parameter_1_Packet));
#endif
#ifdef	USB_COMMUNICATION_1
	 CDC_Transmit_FS((uint8_t*)&Fuel_Gauge_Parameter_1_Packet,sizeof(Fuel_Gauge_Parameter_1_Packet));
#endif

}



void Fuel_Gauge_Parameter_List_2_Transmit()
{
	Fuel_Gauge_Parameter_2_Packet._header             = 0x5065 ;
	Fuel_Gauge_Parameter_2_Packet._length             = sizeof(Fuel_Gauge_Parameter_2_Packet)-4 ;
	Fuel_Gauge_Parameter_2_Packet._TTF_Int            = ttf_Int;
	Fuel_Gauge_Parameter_2_Packet._TTF_float          = ttf_Float;
	Fuel_Gauge_Parameter_2_Packet._Avg_Current_Int    = Average_Current_Int;
	Fuel_Gauge_Parameter_2_Packet._Avg_Current_float  = Average_Current_Float;
	Fuel_Gauge_Parameter_2_Packet._Current_Int        = Current_Int;
	Fuel_Gauge_Parameter_2_Packet._Current_float      = Current_Float;
	Fuel_Gauge_Parameter_2_Packet._Temp_Int           = Temperature_Int;
	Fuel_Gauge_Parameter_2_Packet._Temp_Float         = Temperature_Float;
	Fuel_Gauge_Parameter_2_Packet._CRC8               = chksum8(&Fuel_Gauge_Parameter_2_Packet._TTF_Int,(Fuel_Gauge_Parameter_2_Packet._length));

#ifdef	UART_COMMUNICATION_1
	HAL_UART_Transmit_IT(&huart6,(uint8_t*)&Fuel_Gauge_Parameter_2_Packet,sizeof(Fuel_Gauge_Parameter_2_Packet));
#endif
#ifdef	USB_COMMUNICATION_1
	 CDC_Transmit_FS((uint8_t*)&Fuel_Gauge_Parameter_2_Packet,sizeof(Fuel_Gauge_Parameter_2_Packet));
#endif

}
