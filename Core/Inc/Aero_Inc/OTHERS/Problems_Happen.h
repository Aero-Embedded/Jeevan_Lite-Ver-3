/*
 * Problems_Happen.h
 *
 *  Created on: Jan 18, 2023
 *      Author: asus
 */

#ifndef INC_PROBLEMS_HAPPEN_H_
#define INC_PROBLEMS_HAPPEN_H_



/* Free rtos based bugs

 * check freertos heap size.
 * check task stack size.
 * check priority base concepts works or not.
 * check Queue takes more time or not.
 * check semaphore works or not.
 * check one shot timer works or not.
 * check how much RAM Space consumes.

*/


/*device based problem

 * check expiratory valve flop,connection.
 * check nrv.
 * check lung.
 * check blower leak.
 * check sensor voltages.
 * check Oxygen input bar pressure.
 * Don't use multiple lines in interrupt code it will cause or not execute some lines.
 * suspend a take in sensor offset task.
 * suspend a task in Mode initial Flag set.
 * Check all Task Run in Respected Millisecond Delay.
 * Check Variables are Reset or Not.
 * hard fault handler happen for anything divisible by zero
 * check Tubing in case any peep problem oscillations.

*/


/*hints  for re modifying coding 09/09/23

 * fuel gauge method is hided and normal battery read mode is used
 * in sampled parameter frame one only used another all are commented
 * in uart to Bluetooth communication sampled parameter and fuel gauge method are hide.please verify it.
 * fio2 blending values are changed please change it.
 * please check ws812 alert indication led.
 * check the pressure and o2 flow sensor read pin area may be it will be change.

*/



#endif /* INC_PROBLEMS_HAPPEN_H_ */
