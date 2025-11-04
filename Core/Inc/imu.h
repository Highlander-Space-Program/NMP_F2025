/*
 * imu.h
 *
 *  Created on: Sep 26, 2025
 *      Author: antcr
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#define ACCEL_X_REG (0x1F)
#define ACCEL_SCALE (16.0f / 32768.0f)

#include "main.h"
#include "imu_states.h"
#include <stdint.h>

//this struct for the accelerometer
typedef struct{
	int16_t rawX;
	int16_t rawY;
	int16_t rawZ;

	float gX;
	float gY;
	float gZ;

	enum IMU_STATE state;
	enum IMU_TRANSITIONS event;

} accel_t; // _t refers to type

void writeIMU(uint8_t writeReg, uint8_t dataByte);
uint8_t readIMU(uint8_t readReg);
void configIMU(void);
void readAccelData(accel_t* accelData);

#endif /* INC_IMU_H_ */
