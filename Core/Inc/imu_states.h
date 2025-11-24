/*
 * imu_states.h
 *
 *  Created on: Oct 15, 2025
 *      Author: Medha
 */

#ifndef INC_IMU_STATES_H_
#define INC_IMU_STATES_H_


enum IMU_STATE {
    ON_PAD, IN_FLIGHT, LANDED
};

enum IMU_TRANSITIONS {
	LIFT_OFF, TOUCHDOWN, HOLD //need 3rd transition, on->blink, blink->off, off->on
};

#endif /* INC_IMU_STATES_H_ */
