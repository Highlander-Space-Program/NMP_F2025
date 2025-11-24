/*
 * imu.h
 *
 *  Created on: Oct 1, 2025
 *      Author: Medha
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_
#define ACCEL_X_REG 0x1F //hexadecimal number for the first register we want to read
#define ACCEL_SCALE (16.0f / 32768.0f)

#include "main.h" //HAL, GPIO, and SPI
#include <stdint.h> //for uint data types
#include "imu_states.h"

//struct for accelerometer
typedef struct {
	int16_t rawX;
	int16_t rawY;
	int16_t rawZ;
	float gX;
	float gY;
	float gZ;
	enum IMU_STATE state;
	enum IMU_TRANSITIONS event;
} accel_t;

void writeIMU(uint8_t writeReg, uint8_t dataByte);
uint8_t readIMU(uint8_t readReg);
void configIMU(void);
void readAccelData(accel_t*accelData);

#endif /* INC_IMU_H_ */
