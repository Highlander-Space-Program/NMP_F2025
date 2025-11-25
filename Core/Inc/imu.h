/*
 * imu.h
 *
 *  Created on: Nov 4, 2025
 *      Author: DrSte
 */
#ifndef INC_IMU_H_
#define INC_IMU_H_

#include "main.h"   //HAL, GPIO, and SPI
#include "imu_states.h"
#include <stdint.h> //for uint data types

void configIMU(void);
#define ACCEL_X_REG 0x1F
#define ACCEL_SCALE (16.0f / 32768.0f)

typedef struct{
	int16_t rawX;
	int16_t rawY;
	int16_t rawZ;
	float gX;
	float gY;
	float gZ;
	IMU_STATE state;
	IMU_TRANSITIONS event;

}accel_t;


void writeIMU(uint8_t writeReg, uint8_t dataByte);
uint8_t readIMU(uint8_t readReg);
void configIMU(void);
void readAccelData(accel_t *accelData);

#endif /* INC_IMU_H_ */
