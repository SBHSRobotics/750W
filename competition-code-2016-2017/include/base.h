/**
 * @file base.h
 * @file base.c
 *
 * @brief Allows for interfacing with the base and have it intelligently change its speeds
 *
 * This file provides memory of the base to allow for intelligent speed change as well as
 * only allowing certain powers from the joystick's axis' and certain inputs from the buttons
 * to specify certain movements of the base. This file allows for holonomic drive and tank
 * drive of 4 motors for both configurations
 */

#ifndef BASE_H_
#define BASE_H_

// Includes
#include "API.h"
#include "utility_methods.h"
#include "global_variable_initialization.h"
#include "relative_and_global_angle.h"
#include <math.h>

/**
 * Stops all the motors of the base
 * DO NOT USE UNLESS IT IS FOR AN EMERGANCY OR FOR TESTING PURPOSES
 *
 * @param old is the state of the base from the last iteration
 * @return the Base object after the motors have been set to 0 and the base reset accordingly
 */
Base baseStop(Base old);

/**
 * Used to interpret the input from the joystick and then decided what the desired speed of the
 * base motors should be for a x-drive or holonomic drive or if it is turning
 *
 * @param old is the state of the base from the last iteration
 * @param chS is the power of the secondary axis(X-axis)
 * @param chP is the power of the primary axis(Y-axis)
 * @param leftFast is whether or not the robot should turn left fast
 * @param leftSlow is whether or not the robot should turn left slow
 * @param rightFast is whether or not the robot should turn right fast
 * @param rightSlow is whether or not the robot should turn right slow
 * @param turnSpeed is the max speed the robot should turn at for fast, slow is half of this speed
 *
 * @return the Base object after it has undergone the change specified to it
 */
Base baseSetQuad(Base old, int chS, int chP,
		bool leftFast, bool leftSlow, bool rightFast, bool rightSlow,
		int turnSpeed);

/**
 * Used to interpret the input from the joystick and then decided what the desired speed of the
 * base motors should be for a tank drive or if it is turning
 *
 * @param old is the state of the base from the last iteration
 * @param chP is the power of the primary axis(Y-axis)
 * @param left is whether or not the robot should turn left
 * @param right is whether or not the robot should turn right
 * @param turnSpeed is the max speed the robot should turn at
 *
 * @return the Base object after it has undergone the change specified to it
 */
Base baseSetNorm(Base old, int chP, bool left, bool right, int turnSpeed);

Base adjustedBaseSet();

void baseSpeed(Base old);

#endif /* BASE_H_ */
