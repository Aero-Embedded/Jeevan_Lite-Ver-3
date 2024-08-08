/*
 * SUCTION.c
 *
 *  Created on: Mar 15, 2023
 *      Author: asus
 */


#include "SUCTION.h"

uint8_t initial_suction_valve_open = 1;

void Suction_Task(void *argument)
{
	while (1)
	{
		if(initial_suction_valve_open == 1)
		{
			ExpValve_OPEN();
			initial_suction_valve_open = 0;
			vTaskDelay(1000);
		}
		else
		{
			ExpValve_CLOSE();
			Blower_Signal(1300);
		}
		vTaskDelay(100);
	}
}

void SUCTION_Packet_Data()
{
	ExpValve_OPEN();
	vTaskSuspend(Vc_Cmv_Handler);
	vTaskSuspend(Volume_Control_Task_Handler);
	vTaskSuspend(One_Time_Handler);
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
	vTaskSuspend(HFNC_Handler);
	Blower_Signal( 0);
	Parkar_valve_Signal(0);
	Oxygen_Parameter.Servo_Position   =45;
	Servo_Info(Oxygen_Parameter.Servo_Position);
	Servo_Position_Feedback          = SERVO_FULL_OPEND;
	Servo_Angle(Oxygen_Parameter.Servo_Position);
	vol.Volume       = RESET;
	vol.Volume_max   = RESET;
	vol.Volume_Val   = RESET;
	initial_suction_valve_open = 1;
	vTaskResume(Suction_Handler);
}
