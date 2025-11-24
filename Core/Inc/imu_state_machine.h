/*
 * imu_state_machine.h
 *
 *  Created on: Oct 15, 2025
 *      Author: Medha
 */

#ifndef INC_IMU_STATE_MACHINE_H_
#define INC_IMU_STATE_MACHINE_H_

#define LIFTOFF_MIN 0.7f //should be checking x/y values, somehow need to identify that this is for one direction (x,y,z)
#define LIFTOFF_MAX  1.2f //should be checking x/y valueswtf are these supposed to be
#define TOUCHDOWN_MIN -1.2f //checking z value (-1)
#define TOUCHDOWN_MAX -0.7f //check z value (-1)
//on_pad = before liftoff


#include "main.h"
#include "imu.h"
#include "imu_states.h"

enum IMU_TRANSITIONS determineTransition(accel_t*accelData) {
	//if else to compare g values to define MIN / MAX
	if ((accelData->gX > LIFTOFF_MIN && accelData->gX < LIFTOFF_MAX) || (accelData->gY > LIFTOFF_MIN && accelData->gY < LIFTOFF_MAX)) { //does that Z part make sense???
		return LIFT_OFF;
	} else if (accelData->gZ > TOUCHDOWN_MIN && accelData->gZ < TOUCHDOWN_MAX) { //maybe include the fact that x and y values are decreasing?
		return TOUCHDOWN;
	} else
	    return HOLD;
}

void Tick_IMU(accel_t*accelData) {
	//call accelReadData, then determineTransition before switch case statement
	//remember 3rd transition in 1st switch case statement
	//accelData->event = HOLD; //do i include enum here
	readAccelData(accelData);
	accelData->event = determineTransition(accelData); //is this the right parameter
	switch (accelData->state) {
		        case ON_PAD:
		            if (accelData->event == LIFT_OFF) {
		                accelData->state = IN_FLIGHT;
		            } else if (accelData->event == HOLD) {
		            	accelData->state = ON_PAD;
		            }
		            break;
		        case IN_FLIGHT:
		            if (accelData->event == TOUCHDOWN) {
		                accelData->state = LANDED;
		            } else if (accelData->event == HOLD) {
		                accelData->state = IN_FLIGHT;
		            }
		            break;
		        case LANDED:
		            if (accelData->event == HOLD) {
		                accelData->state = LANDED;
		            }
		            break;
		    }
	switch (accelData->state) {
		case ON_PAD:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			break;
		case IN_FLIGHT:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			break;
		case LANDED:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			break;
	}

}

#endif /* INC_IMU_STATE_MACHINE_H_ */
