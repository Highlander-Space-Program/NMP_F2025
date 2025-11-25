/*
 * imu.c
 *
 *  Created on: Nov 4, 2025
 *      Author: DrSte
 */
#include "imu.h"
extern SPI_HandleTypeDef hspi1;

//Reads IMU values from parameter reg
uint8_t readIMU (uint8_t readReg){
	uint8_t txData[2];
	uint8_t rxData[2];
	txData[0] = readReg | 0x80;											//MSB needs to be 1 to signal IMU to do read operation
	txData[1] = 0x00;
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);	//write Pin set to low to catch signal at falling edge
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);	//write Pin set to high to catch signal at rising edge
	return rxData[1];
}

//writes a value to a certain IMU reg
void writeIMU(uint8_t writeReg, uint8_t dataByte)
{
	uint8_t txData[2];
	uint8_t rxData[2];
	txData[0] = writeReg & 0x7F;										//MSB needs to be 0 to signal IMU to do read operation
	txData[1] = dataByte;												//dataByte is set to last 8 bits
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);
}

void configIMU(void)
{

	writeIMU(0x4E, 0x03);	//Config power management reg
	writeIMU(0x50, 0x03);	//Config accelerometer reg

}

void readAccelData(accel_t *accelData)
{
	uint8_t txData[7] = {0};
	uint8_t rxData[7] = {0};
	txData[0] = ACCEL_X_REG | 0x80;
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 7, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);
	accelData->rawX = (rxData[1] << 8) | rxData[2];
	//the other raw builds go here
	accelData->rawY = (rxData[3] << 8) | rxData[4];
	accelData->rawZ = (rxData[5] << 8) | rxData[6];

	accelData->gX = accelData->rawX * ACCEL_SCALE;
	accelData->gY = accelData->rawY * ACCEL_SCALE;
	accelData->gZ = accelData->rawZ * ACCEL_SCALE;

}
