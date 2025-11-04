/*
 * imu_state_machine.h
 *
 *  Created on: Sep 29, 2025
 *      Author: antcr
 */

#ifndef INC_IMU_STATE_MACHINE_H_
#define INC_IMU_STATE_MACHINE_H_

#include "main.h"
#include "imu.h"
#include "imu_states.h"

#define LIFTOFF_MIN 0.7f
#define LIFTOFF_MAX 1.2f
#define TOUCHDOWN_MIN -1.2f
#define TOUCHDOWN_MAX -0.7f

enum IMU_TRANSITIONS determineTransition (accel_t* accelData){
	if (accelData->gZ > LIFTOFF_MIN && accelData->gZ < LIFTOFF_MAX){
		return(eventOP);
	}
	else{
		if (accelData->gX > LIFTOFF_MIN && accelData->gX < LIFTOFF_MAX){
			return(eventIF);
		}
		if (accelData->gY > LIFTOFF_MIN && accelData->gY < LIFTOFF_MAX){
			return(eventIF);
		}
		else{
			if (accelData->gZ > TOUCHDOWN_MIN && accelData->gZ < TOUCHDOWN_MAX){
				return(eventLD);
			}
		}
	}
	return(eventOP);
}

void Tick_IMU (accel_t* accelData){
// switching based on transition
	readAccelData(accelData);
	accelData->event = determineTransition(accelData);

	switch (accelData->event){
	case eventOP:
		accelData->state = ON_PAD;
		break;
	case eventIF:
		accelData->state = IN_FLIGHT;
		break;
	case eventLD:
		accelData->state = LANDED;
		break;
	}

// actions based off of state
	switch (accelData->state){
	case ON_PAD: // stay on
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		break;
	case eventIF: // blink
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		HAL_Delay(30);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		HAL_Delay(30);
		break;
	case eventLD: // stay off
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		break;
	}
}

#endif /* INC_IMU_STATE_MACHINE_H_ */
