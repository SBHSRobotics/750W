#ifndef INCLUDE_FPS_H_
#define INCLUDE_FPS_H_

#include "API.h"
#include "global_variable_initialization.h"
#include "relative_and_global_angle.h"
#include "math.h"

//temp
Ultrasonic ultraLeft, ultraBack, ultraRight;
//Encoder encoderLeft, encoderRight;
Encoder encoderBaseLeft, encoderBaseRight;
//Gyro gyro;//Already exists
Gyro gyroscope;
//temp

Base FPSBase;
Location position;

/*
typedef struct Location
{
	int x;
	int y;
}Location;

typedef struct ShooterAngle
{
	int theta;
	int phi;
}ShooterAngle;
*/
//height goal, shooter, velocity of ball

//int shooterYaw = 0;

int getFPSGlobalAngle(Correction correction, Gyro gyro);

int getSonarLeft();

int getSonarBack();

int getSonarRight();

Location getLocation();

void FPSTask(void* ignore);

#endif /* INCLUDE_FPS_H_ */
