#include "imu.h"
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;


uint8_t readIMU(uint8_t readReg)
{
	uint8_t txData[2]; // data transmitting
	uint8_t rxData[2]; // data receiving
	txData[0] = readReg | 0x80;
	txData[1] = 0x00;
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);
	return rxData[1];
}

void writeIMU(uint8_t writeReg, uint8_t dataByte)
{
	// MSB of the register we are writing to has to be a zero to write to the IMU
	uint8_t txData[2];
	uint8_t rxData[2];
	txData[0] = writeReg & 0x7F; // 0111 1111
	txData[1] = dataByte;
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET); // pull chipselect down to 0 same as read function
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 2, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET); // pull chipselect up to 1 signaling we are done transmitting

}

void configIMU(void)
{
	writeIMU(0x4E, 0xF); // config power management - low noise mode - 0000 1111
	writeIMU(0x50, 0x03); // config accelerometer - 8kHZ +-16g - 00000 0011
	writeIMU(0x4F, 0x03); // config gyro -8kHz +-2000 dps - 0000 0011

}

void readAccelData(accel_t*accelData)
{
	uint8_t txData[7] = {0};
	uint8_t rxData[7] = {0};
	txData[0] = ACCEL_X_REG | 0x80;
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 7, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);
	// combines the two bytes of data
	accelData->rawX = (rxData[1] << 8) | rxData[2];
	accelData->rawY = (rxData[3] << 8) | rxData[4];
	accelData->rawZ = (rxData[5] << 8) | rxData[6];
	accelData->gX = accelData->rawX * ACCEL_SCALE;
	accelData->gY = accelData->rawY * ACCEL_SCALE;
	accelData->gZ = accelData->rawZ * ACCEL_SCALE;

}

void readGyroData(gyro_t*gyroData)
{
	uint8_t txData[7] = {0};
	uint8_t rxData[7] = {0};
	txData[0] = GYRO_X_REG | 0x80;
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, txData, rxData, 7, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IMU_NCS_GPIO_Port, IMU_NCS_Pin, GPIO_PIN_SET);
	gyroData->rawX = (rxData[1] << 8) | rxData[2];
	gyroData->rawY = (rxData[3] << 8) | rxData[4];
	gyroData->rawZ = (rxData[5] << 8) | rxData[6];
	gyroData->gX = gyroData->rawX * GYRO_SCALE;
	gyroData->gY = gyroData->rawY * GYRO_SCALE;
	gyroData->gY = gyroData->rawZ * GYRO_SCALE;
}

void dataToPlotter(accel_t*accelData, gyro_t*gyroData)
{
	int length;
	readAccelData(accelData);
	readGyroData(gyroData);
	char uartBuffer[256];
	length = snprintf(uartBuffer, sizeof(uartBuffer),
			"%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\r\n",
			accelData->gX, accelData->gY, accelData->gZ,
			gyroData->gX, gyroData->gY, gyroData->gZ);
	HAL_UART_Transmit(&huart2, (uint8_t*)uartBuffer, length, HAL_MAX_DELAY);
}
