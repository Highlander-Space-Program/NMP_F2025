/*
 * imu_states.h
 *
 *  Created on: Sep 21, 2025
 *      Author: salde
 */

#ifndef INC_IMU_STATES_H_
#define INC_IMU_STATES_H_

typedef enum {
	ON_PAD = 0,
	IN_FLIGHT = 1,
	LANDED = 2
} IMU_STATE;

typedef enum {
	LIFT_OFF = 0,
	TOUCHDOWN = 1,
	NO_EVENT = 2
} IMU_TRANSITIONS;


#endif /* INC_IMU_STATES_H_ */
