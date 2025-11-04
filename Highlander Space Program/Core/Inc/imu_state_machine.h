#ifndef INC_IMU_STATE_MACHINE_H_
#define INC_IMU_STATE_MACHINE_H_

#include "main.h"
#include "imu.h"
#include "imu_states.h"

#define LIFTOFF_MIN 0.8
#define LIFTOFF_MAX 1.2
#define NLIFTOFF_MIN -0.8
#define NLIFTOFF_MAX -1.2
#define TOUCHDOWN_MIN -0.8
#define TOUCHDOWN_MAX -1.2


enum IMU_TRANSITIONS determineTransition(accel_t *accelData)
{
	if ((accelData->gX >= LIFTOFF_MIN && accelData->gX <= LIFTOFF_MAX) || (accelData->gY >= LIFTOFF_MIN && accelData->gY <= LIFTOFF_MAX))
	{
		return LIFT_OFF;
	}
	else if((accelData->gX <= NLIFTOFF_MIN && accelData->gX >= NLIFTOFF_MAX) || (accelData->gY <= NLIFTOFF_MIN && accelData->gY >= NLIFTOFF_MAX))
	{
		return LIFT_OFF;
	}
	else if (accelData->gZ <= TOUCHDOWN_MIN && accelData->gZ >= TOUCHDOWN_MAX)
	{
		return TOUCHDOWN;
	}
	return NO_TRANSITION;
}

void Tick_IMU(accel_t *accelData)
{
	readAccelData(accelData);

	accelData->event = determineTransition(accelData);
	switch (accelData->event)
	{
		case LIFT_OFF:
			accelData->state = IN_FLIGHT;
			break;
		case TOUCHDOWN:
			accelData->state = LANDED;
			break;
		case NO_TRANSITION:
			accelData->state = accelData->state;
			break;
	}

	switch (accelData->state)
	{
	 case ON_PAD:
		 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		 break;
	 case IN_FLIGHT:
		 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		 HAL_Delay(50);
		 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		 break;
	 case LANDED:
		 HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		 break;
	}

}

#endif /* INC_IMU_STATE_MACHINE_H_ */
