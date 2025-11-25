/*
 * imu_state_machine.h
 *
 *  Created on: Nov 6, 2025
 *      Author: DrSte
 */

#ifndef INC_IMU_STATE_MACHINE_H_
#define INC_IMU_STATE_MACHINE_H_

#define LIFTOFF_MIN	    0.8	//This stays at -1G?
#define LIFTOFF_MAX		1.2
#define TOUCHDOWN_MIN	-0.8	//This stays at -1G?
#define TOUCHDOWN_MAX	-1.2
#include "main.h"
#include "imu.h"
#include "imu_states.h"

void determineTransition (accel_t *accelData)
{
	if((accelData->gX <= LIFTOFF_MAX && accelData->gX >= LIFTOFF_MIN) || (accelData->gY <= LIFTOFF_MAX && accelData->gY >= LIFTOFF_MIN))
	{
		accelData->event = Lift_Off;
	}
	else if(accelData->gZ <= TOUCHDOWN_MIN && accelData->gZ >= TOUCHDOWN_MAX)
	{
		accelData->event = Touchdown;
	}
	else
	{
		accelData->event = Start;
	}
}

void Tick_IMU (accel_t *accelData) {
	readAccelData(accelData);
	determineTransition(accelData);

	switch(accelData->state)
	{
		case On_Pad:
			if(accelData->event == Start)
				accelData->state = On_Pad;
			else if(accelData->event == Lift_Off)
				accelData->state = In_Flight;
			break;
		case In_Flight:
			if(accelData->event != Touchdown)
				accelData->state = In_Flight;
			else
				accelData->state = Landed;
			break;
		case Landed:
			accelData->state = Landed;
			break;
		default:
			break;

	}
	switch(accelData->state)
	{
		case On_Pad:
			//Blink LED
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			break;
		case In_Flight:
			//Blink LED super fast
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			break;
		case Landed:
			//Turn off LED
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			break;
		default:
			break;
	}
}


#endif /* INC_IMU_STATE_MACHINE_H_ */
