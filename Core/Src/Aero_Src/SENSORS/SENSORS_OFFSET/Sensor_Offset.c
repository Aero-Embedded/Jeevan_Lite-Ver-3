/*
 * Sensor_Offset.c
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */


#include "Sensor_Offset.h"


#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "main.h"


void Sensor_Offset_Task (void *argument)
{
	while (1)
	{

xSemaphoreTake(binarysem,10);

        vTaskDelay(4000);
		vTaskSuspend(Sensor_Read_Handler);
		vTaskSuspend(Time_Trace_Handler);
		vTaskSuspend(Bluetooth_Handler);
		vTaskSuspend(Bluetooth_data_Split_Handler);
		vTaskSuspend(Mode_initial_Flag_Set_Handler);
		vTaskSuspend(One_Time_Handler);
		vTaskSuspend(Alert_Handler);
		vTaskSuspend(Oxygen_Blending_Handler);

		vTaskSuspend(Pc_Cmv_Handler);
		vTaskSuspend(PIP_Control_Task_Handler);

		vTaskSuspend(Vc_Cmv_Handler);
		vTaskSuspend(Volume_Control_Task_Handler);

		vTaskSuspend(Pc_Simv_Assist_ON_Handler);
		vTaskSuspend(Pc_Simv_Assist_OFF_Handler);

		vTaskSuspend(Vc_Simv_Assist_ON_Handler);
		vTaskSuspend(Vc_Simv_Assist_OFF_Handler);

		vTaskSuspend(PSV_Handler);
		vTaskSuspend(PSV_PID_Handler);

		vTaskSuspend(CPAP_Handler);

		vTaskSuspend(APRV_Handler);
		vTaskSuspend(APRV_ONE_TIME_Handler);

		vTaskSuspend(BIPAP_Handler);
		vTaskSuspend(BIPAP_PID_Handler);

		vTaskSuspend(BACKUP_PC_Handler);

		vTaskSuspend(Nebuliser_Handler);

		vTaskSuspend(Suction_Handler);

		vTaskSuspend(LED_Strip_Task_Handler);


		vTaskSuspend(Suction_Handler);
		vTaskSuspend(HFNC_Handler);
		vTaskSuspend(O2_Calibration_Handler);
		vTaskSuspend(Calibration_Handler);
		vTaskSuspend(Fuel_Gauge_Handler);


		Oxygen_Parameter.Achieved_Oxygen = 21;
		Send_Frame_Fuel_Gauge = First_Frame;
		Fuel_Gauge_Init();                          // Fuel Gauge

if( (xSemaphoreTake(binarysem,4000)) == pdTRUE)
{
		Pressure_Sensor_offset();
		Flow_Sensor_7002_offset();

		INITIAL_DAC_VALUE();
		Device_Control = PAUSE;
		INCREMENT_DAC_VAL_PER_ITERATION = 3;

		HFNC_DAC_VALUE =0;
		HFNC_Dac1=230;
		HFNC_Dac2=1800;
		kp=10;
		ki=10;
		kd=1;
		Time=50;
}
if( (xSemaphoreTake(binarysem,4000)) == pdTRUE)
{

			vTaskResume(Sensor_Read_Handler);
			vTaskResume(Time_Trace_Handler);
			vTaskResume(Bluetooth_Handler);
			vTaskResume(Fuel_Gauge_Handler);

xSemaphoreGive(binarysem);
}
		vTaskDelete(Sensor_Offset_Handler);
	}
}



void One_Time_Task(void *argument)
{
	while (1)
	{

		switch (Next_Half_Breath_Cycle)
		{
		case Generate_Inspiration_Cycle:
			switch (Mode)
			{
				case Pccmv:
					PC_CMV_Inspiration_Time_Variables_Reset();
				break;
				case Vccmv:
					VC_CMV_Inspiration_Time_Variables_Reset();
				break;
				case Pcsimv:
					PC_SIMV_Inspiration_Time_Variables_Reset();
				break;
				case Vcsimv:
					VC_SIMV_Inspiration_Time_Variables_Reset();
				break;
				case Backup_Pccmv:
					BACKUP_PC_CMV_Inspiration_Time_Variables_Reset();
				break;
				case Idle:
					vol.Volume=0;
					vol.Volume_Val = 0;
					vTaskDelay(20);
				break;
				default:
					vol.Volume=0;
					vol.Volume_Val = 0;
					vTaskDelay(20);
				break;
			}
		break;

		case Generate_Expiration_Cycle:
			switch (Mode)
			{
				case Pccmv:
					PC_CMV_Expiration_Time_Variables_Reset();
					break;
				case Vccmv:
					VC_CMV_Expiration_Time_Variables_Reset();
				break;
				case Pcsimv:
					PC_SIMV_Expiration_Time_Variables_Reset();
				break;
				case Vcsimv:
					VC_SIMV_Expiration_Time_Variables_Reset();
				break;
				case Backup_Pccmv:
					BACKUP_PC_CMV_Expiration_Time_Variables_Reset();
				break;
				case Idle:
					vol.Volume=0;
					vol.Volume_Val = 0;
					vTaskDelay(20);
				break;
				default:
					vol.Volume=0;
					vol.Volume_Val = 0;
					vTaskDelay(20);
					break;
			}
			break;

		case NoWaveFormState:
			break;
		default:
			break;

		}
	}
}
