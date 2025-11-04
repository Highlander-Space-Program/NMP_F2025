/*
 * imu_states.h
 *
 *  Created on: Sep 29, 2025
 *      Author: antcr
 */

#ifndef INC_IMU_STATES_H_
#define INC_IMU_STATES_H_

enum IMU_STATE {ON_PAD = 0, IN_FLIGHT = 1, LANDED = 2};
enum IMU_TRANSITIONS {eventOP = 0, eventIF = 1, eventLD = 2};

#endif /* INC_IMU_STATES_H_ */
