/*
 * imu.c
 *
 *  Created on: Sep 19, 2025
 *      Author: Alan Sanchez Rodriguez
 *
 */

//includes
#include "imu.h"

//tells the compiler that this handle is defined externally
extern SPI_HandleTypeDef hspi1;

//function definitions

void writeIMU (uint8_t writeReg, uint8_t dataByte){
	uint8_t txData[2];

	txData[0] = writeReg & 0x7F;
	txData[1] = dataByte;

	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi1, txData, 2, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);

}

uint8_t readIMU (uint8_t readReg){
	uint8_t txData[2]; //this array holds the data we transmit (tx): the register we're writing to
	uint8_t rxData[2]; //this will hold the data we receive (rx)
	//we need 2 elements since we clock the SPI for two bytes = 2x uint8_t
	txData[0] = readReg | 0x80;
	txData[1] = 0x00;

	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);

	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);

	return rxData[1];

}

void configIMU() {
	writeIMU(0x4E, 0x03);
	writeIMU(0x50, 0x03);

}

void readAccelData(accel_t *accelData){

	uint8_t txData[7] = {0};
	uint8_t rxData[7] = {0};

	txData[0] = ACCEL_X_REG | 0x80;


	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 7, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);

	accelData->rawX = (rxData[1] << 8) | rxData[2];
	accelData->rawY = (rxData[3] << 8) | rxData[4];
	accelData->rawZ = (rxData[5] << 8) | rxData[6];

	accelData->gX = accelData->rawX * ACCEL_SCALE;
	accelData->gY = accelData->rawY * ACCEL_SCALE;
	accelData->gZ = accelData->rawZ * ACCEL_SCALE;
}

