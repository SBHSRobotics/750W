/**
 * @file relative_and_global_angle.h
 * @file relative_and_global_angle.c
 *
 * @brief Calculates the relative and global orientation of the robot
 *
 * This calculates the orientation of the robot from its last reset as well as from its last boot
 * Also allows you to reset either of these values if the need arises.
 */
#ifndef RELATIVE_AND_GLOBAL_ANGLE_H_
#define RELATIVE_AND_GLOBAL_ANGLE_H_

/**
 * Includes
 */
#include "API.h"
#include "global_variable_initialization.h"
#include <math.h>
#include "utility_methods.h"

void initGyroscope();

//TODO: correctly write both header and source files, transfer EVERYTHING, will take a long time

/**
 * This returns the angle described by the distance traveled between the left and right side of the robot
 *
 * @param left is the left change in distance of the robot
 * @param right is the right change in distance of the robot
 *
 * @return the angle that the robot has turned
 */
int getAngle(int left, int right);

/**
 * This returns the angle of the robot since its last orientation reset
 *
 * @param gyro is the gyroscope that measures the value of the orientation
 *
 * @return Returns the angle that that the robot has turned since last reset
 */
int getLocalAngle(Gyro gyro);

/**
 * This method gets the orientation of the robot from when it was first booted
 *
 * @param correction is the current memory of the relative and global angle of the robot
 * @param gyro is the gyroscope that measures the value of the orientation
 *
 * @return The current orientation of the robot from its last boot
 */
int getGlobalAngle(Correction correction, Gyro gyro);

/**
 * This method resets the local angle
 *
 * @param correction the variable to have its relative orientation reset
 * @param gyro is the gyroscope that measures the value of the orientation
 *
 * @return is the correction of the robot after it has its relative angle reset
 */
Correction resetLocalAngle(Correction correction, Gyro gyro);

/**
 * This method resets both the relative and global angle
 *
 * @param correction the variable that has to have its values reset
 * @param gyro is the gyro that will be reset
 *
 * @return Returns the reset Correction
 */
Correction resetGlobalAngle(Correction correction, Gyro gyro);


#endif /* RELATIVE_AND_GLOBAL_ANGLE_H_ */
