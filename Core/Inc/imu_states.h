/*
 * imu_states.h
 *
 *  Created on: Nov 6, 2025
 *      Author: DrSte
 */

#ifndef INC_IMU_STATES_H_
#define INC_IMU_STATES_H_
#include "main.h"   //HAL, GPIO, and SPI

#include <stdint.h> //for uint data types

typedef enum {On_Pad, In_Flight, Landed} IMU_STATE;
typedef enum {Start, Lift_Off, Touchdown }IMU_TRANSITIONS;

#endif /* INC_IMU_STATES_H_ */
