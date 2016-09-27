#include "intake.h"

/*
 * Remembers the intake speed for the front and back intake from the last iteration
 */
int oldIntakeSpeedFront = 0;
int oldIntakeSpeedBack = 0;

/*
 *
 */
bool adjustedIntakeSet(int *speed, bool front);

/*
 *
 */
void setFinalIntakeSpeeds(int speed, bool front);

bool adjustedIntakeSet(int *speed, bool front){
	return adjustSpeed(speed, (front ? oldIntakeSpeedFront : oldIntakeSpeedBack), GENERAL_SPEED_INCREASE_MINIMUM, GENERAL_SPEED_INCREASE_MAXIMUM);
}

void setFinalIntakeSpeeds(int speed, bool front){
	//if(millis() - time < timeDelay)
	//	return;
	if(!adjustedIntakeSet(&speed, front))
		return;
	speed = abs(speed) > MAX_SPEED ? sgnSimple(speed) * MAX_SPEED :
		abs(speed) < INPUT_THRESHOLD ? 0 : speed;
	if (front) {
		motorSet(MOTOR_INTAKE_FRONT_PORT, MOTOR_INTAKE_FRONT_MULTIPLIER * speed);
		oldIntakeSpeedFront = speed;
	}
	else {
		motorSet(MOTOR_INTAKE_BACK_PORT, MOTOR_INTAKE_BACK_MULTIPLIER * speed);
		oldIntakeSpeedBack = speed;
	}
}

void setIntake(int speedFront, int speedBack) {
	if (abs(speedFront) > INPUT_THRESHOLD) {
		//speedFront = 85;
		setFinalIntakeSpeeds(speedFront, true);
	}
	else
		setFinalIntakeSpeeds(0, true);
	if (abs(speedBack) > INPUT_THRESHOLD)
		setFinalIntakeSpeeds(speedBack, false);
	else
		setFinalIntakeSpeeds(0, false);
}
