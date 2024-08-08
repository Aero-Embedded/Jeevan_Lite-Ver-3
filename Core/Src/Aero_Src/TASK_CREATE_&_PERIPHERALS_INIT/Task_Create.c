/*
 * Task_Create.c
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */


#include "Task_Create.h"


extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim12;

void Initial_Task_Create()
{
		   HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&Adc.AdcData,6);
		   Blower_ON();
		   HAL_DAC_Start(&hdac,DAC1_CHANNEL_1);
		   Blower_Signal(0);
		   HAL_DAC_Start(&hdac,DAC1_CHANNEL_2);
		   Parkar_valve_Signal(0);
		   HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
		   Servo_Angle(SERVO_FULL_OPEND);
		   Uart_Receive_Debug_Toggle_Led();
		   PIP_Not_Acheieved();
		   Blue_Led_ON();
		   Red_Led_OFF();
		   Green_Led_OFF();

		   Bluetooth_Receive = xQueueCreate(1, sizeof(Bluetooth_Parameter.Bluetooth_RX_BUF));
		   vSemaphoreCreateBinary(binarysem);

		   xTaskCreate(Sensor_Offset_Task,"Sensor-Offset-Task", 128, NULL, 3, &Sensor_Offset_Handler);
		   xTaskCreate(Time_Trace_Task,"Time-Trace-Task", 128, NULL, 2, &Time_Trace_Handler);
		   xTaskCreate(Sensor_Read_Task,"Sensor-Read-Task", 256, NULL, 2, &Sensor_Read_Handler);
		   xTaskCreate(Bluetooth_Task,"Bluetooth-Task", 128, NULL, 2, &Bluetooth_Handler);
		   xTaskCreate(BlueTooth_Receiver_Task,"BlueTooth-Data-Receive-Task", 128, NULL, 5, &Bluetooth_Receiver_Handler);
		   xTaskCreate(Bluetooth_data_split_Task,"Uart-data-split-Task", 128, NULL, 4, &Bluetooth_data_Split_Handler);
		   xTaskCreate(Mode_Initial_Flag_Set_Task,"Initial-Flag-Set-Task", 128, NULL, 3, &Mode_initial_Flag_Set_Handler);
		   xTaskCreate(One_Time_Task,"One-Time-Task", 128, NULL,3, &One_Time_Handler);
		   xTaskCreate(Pc_Cmv_Task,"Pc-Cmv-Task", 128, NULL, 2, &Pc_Cmv_Handler);
		   xTaskCreate(PIP_Control_Task,"PIP-Control-PC-CMV-Task", 128, NULL, 2, &PIP_Control_Task_Handler);
		   xTaskCreate(Vc_Cmv_Task,"Vc-Cmv-Task", 128, NULL, 2, &Vc_Cmv_Handler);
		   xTaskCreate(Volume_Control_Task,"Volume-Control-VC-CMV-Task", 128, NULL, 2, &Volume_Control_Task_Handler);
		   xTaskCreate(Oxygen_Blending_Task, "Oxygen-Blending-Task", 128, NULL, 2, &Oxygen_Blending_Handler);
		   xTaskCreate(Alert_Task, "Alert Task", 128, NULL, 2, &Alert_Handler);
		   xTaskCreate(Pc_Simv_Assist_ON_Task,"Pc-Simv-Assist-ON-Task", 128, NULL, 2, &Pc_Simv_Assist_ON_Handler);
		   xTaskCreate(Pc_Simv_Assist_OFF_Task,"Pc-Simv-Assist-OFF-Task", 128, NULL, 2, &Pc_Simv_Assist_OFF_Handler);
		   xTaskCreate(Vc_Simv_Assist_ON_Task,"Vc-Simv-Assist-ON-Task", 128, NULL, 2, &Vc_Simv_Assist_ON_Handler);
		   xTaskCreate(Vc_Simv_Assist_OFF_Task,"Vc-Simv-Assist-OFF-Task", 128, NULL, 2, &Vc_Simv_Assist_OFF_Handler);
		   xTaskCreate(PSV_Task,"PSV-Task", 128, NULL, 2, &PSV_Handler);
		   xTaskCreate(PSV_PID_Task,"PSV-PID-Task", 128, NULL, 2, &PSV_PID_Handler);
		   xTaskCreate(CPAP_Task,"CPAP-Task", 128, NULL, 2, &CPAP_Handler);
		   xTaskCreate(BIPAP_Task,"BIPAP-Task", 128, NULL, 2, &BIPAP_Handler);
		   xTaskCreate(BIPAP_PID_Task,"BIPAP-PID-Task", 128, NULL, 2, &BIPAP_PID_Handler);
		   xTaskCreate(APRV_Task,"APRV-Task", 128, NULL, 2, &APRV_Handler);
		   xTaskCreate(APRV_ONE_TIME_Task,"APRV-ONE-TIME-Task", 128, NULL, 2, &APRV_ONE_TIME_Handler);
		   xTaskCreate(BACKUP_PC_CMV_Task,"BACKUP-PC-CMV-Task", 128, NULL, 2, &BACKUP_PC_Handler);
		   xTaskCreate(Nebuliser_Task,"Nebulizer-Task", 128, NULL, 2, &Nebuliser_Handler);
	       xTaskCreate(Suction_Task,"Suction-Task", 128, NULL, 2, &Suction_Handler);
	       xTaskCreate(HFNC_Task,"HFNC-Task", 128, NULL, 2, &HFNC_Handler);
	       xTaskCreate(O2_Calibration_Task,"O2-Calibration-Task", 128, NULL, 2, &O2_Calibration_Handler);
	       xTaskCreate(CALIBRATION_Task, "CALIBRATION Task", 128, NULL, 2, &Calibration_Handler);
	       xTaskCreate(FUEL_GAUGE_Task, "FUEL GAUGE Task", 128, NULL, 2, &Fuel_Gauge_Handler);
	       xTaskCreate(LED_Strip_Task, "LED Strip Task", 128, NULL, 1, &LED_Strip_Task_Handler);
#ifdef PID_ON_1
	       HFNC_DAC_VALUE =0;
		   HFNC_Dac1=1800;
		   HFNC_Dac2=3100;
		   kp=10;
		   ki=10;
		   kd=1;
		   Time=50;
		   TempSetpoint =30;
		   PID(&TPID, &Temp_Flow, &PIDOut, &TempSetpoint, kp, ki, kd, _PID_P_ON_E, _PID_CD_DIRECT);
		   PID_SetMode(&TPID, _PID_MODE_AUTOMATIC);
		   PID_SetSampleTime(&TPID, Time);
		   PID_SetOutputLimits(&TPID, HFNC_Dac1, HFNC_Dac2);
#endif
		   free_heap_size = xPortGetFreeHeapSize();
		   vTaskStartScheduler();
}


