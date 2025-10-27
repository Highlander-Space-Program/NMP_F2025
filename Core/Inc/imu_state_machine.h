/*
 * imu_state_machine.h
 *
 *  Created on: Sep 21, 2025
 *      Author: salde
 */

#ifndef INC_IMU_STATE_MACHINE_H_
#define INC_IMU_STATE_MACHINE_H_

#include "main.h"
#include "imu.h"
#include "imu_states.h"


#define LIFTOFF_MIN 0.7f
#define LIFTOFF_MAX 1.2f
#define LANDED_MIN -1.2f
#define LANDED_MAX -0.7f

IMU_TRANSITIONS determineTransition (accel_t *accelData){

	if (((accelData->gX < LIFTOFF_MAX) && (accelData->gX > LIFTOFF_MIN)) ||
		((accelData->gY < LIFTOFF_MAX) && (accelData->gY > LIFTOFF_MIN))) {

		return LIFT_OFF;
	} else if ((accelData->gZ > LANDED_MIN) && (accelData->gZ < LANDED_MAX) ) {
		return TOUCHDOWN;
	}

	return NO_EVENT;
}

void Tick_IMU (accel_t *accelData) {

	readAccelData(accelData);

	accelData->event = determineTransition(accelData);


	switch (accelData->event) {
		case LIFT_OFF:
			if (accelData->state == ON_PAD) {
				accelData->state = IN_FLIGHT;
			}

			break;

		case TOUCHDOWN:
			if (accelData->state == IN_FLIGHT) {
				accelData->state = LANDED;
			}

			break;
		case NO_EVENT:
		break;

	}


	switch (accelData->state) {
		case ON_PAD:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			break;

		case IN_FLIGHT:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			HAL_Delay(20);
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			break;

		case LANDED:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
			break;

	}


}



#endif /* INC_IMU_STATE_MACHINE_H_ */
