/*
 * imu.c
 *
 *  Created on: Oct 1, 2025
 *      Author: Medha
 */

#include "imu.h"
extern SPI_HandleTypeDef hspi1;

uint8_t readIMU (uint8_t readReg){
	uint8_t txData[2];
	uint8_t rxData[2];
	txData[0] = readReg | 0x80; //128, 10000000
	txData[1] = 0x00;
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);
	return rxData[1];
}

void writeIMU(uint8_t writeReg, uint8_t dataByte){
	uint8_t txData[2]; //
	uint8_t rxData[2];

	txData[0] = writeReg & 0x7F; //make first bit 0; 127, 0111 1111
	txData[1] = dataByte; //what we want stored in register

	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET); //pull pin low
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET); //pull pin back HIGH (SET)
}

void configIMU() {
	uint8_t dataByte = 0x03; //0000 0011 (accelerometer in low noise mode)
	writeIMU(0x4E, dataByte); //need to "configure power management register
	dataByte = 0x03; //0x18 for +-16g (0000 0011)
	writeIMU(0x50, dataByte); //need to "configure accelerometer configuration register

}
//8kHz ODR, device reports its position/status to computer 8000 times
//per second, offers reduction in input latency compared to standard
//1000Hz (results in smoother motion and faster registration of clicks)

//document says +- 16g is 000 (for 16g, you'll set bits 7-5 = 000)
//8kHz is 0011 (set bits 3-0 = 0011)
//so 000_ 0011 (4th bit is reserved? so it is always 0?)

void readAccelData(accel_t *accelData){
	uint8_t txData[7] = {0};  //sent to IMU
	uint8_t rxData[7] = {0};  //received
	txData[0] = ACCEL_X_REG | 0x80; //10000000
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 7, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);
	accelData->rawX = (rxData[1] << 8) | rxData[2];
	accelData->rawY = (rxData[3] << 8) | rxData[4];
	accelData->rawZ = (rxData[5] << 8) | rxData[6];
	//the other raw builds go here
	accelData->gX = accelData->rawX * ACCEL_SCALE;
	accelData->gY = accelData->rawY * ACCEL_SCALE;
	accelData->gZ = accelData->rawZ * ACCEL_SCALE;
}


