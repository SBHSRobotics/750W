#pragma once

/**
 * Includes
 */
#include <math.h>
#include "API.h"

/**
 *	Object Definitions
 */

/**
 *	@enum Direction
 *  @brief angle bot is facing
 *
 *	Describes the Direction the bot is facing relative to the field.
 */
typedef struct Direction
{
	int chX;
	int chY;
}Direction;

/**
 *	@enum Axis
 *	@brief The direction of motion of the robot
 *
 *	Describes the motion of the robot including if it is not moving.
 *	Does not distinguish between directions of motion but instead of the type of motion.
 */
typedef enum Axis
{
	/** The robot is not moving */
	none,
	/** The robot is moving along its X-Axis */
	axisX,
	/**The robot is moving along its Y-Axis */
	axisY,
	/**The robot is moving in a direction different from X and Y*/
	axisBoth,
	 /**The robot is turning in place*/
	axisTurn
}Axis;

/**
 * @struct Correction
 * @brief This is used to describe the location of the robot
 *
 *
 * Describes the location of the robot relative to the last local reset as well as relative to the orientation
 * from when it was first booted
 */
typedef struct Correction
{
	/**
	* The angle of the robot in respect to the orientation of the robot when it was turned on
	*/
	int globalAngle;
	/**
	 * The angle of the robot in respect to the last reset. Is used for convenience in angle calculations
	 * over globalAngle
	 */
	int localAngle;
}Correction;

/**
 * @struct Base
 * @brief This describes the characteristics of the base of the robot
 *
 * This describes the rotation of the left and right side of the robot,
 * its current orientation as well as direction of motion
 */
typedef struct Base
{
	/** The speed that the left designation of the robot should/is set to */
	int left;
	/** The speed that the right designation of the robot should/is set to */
	int right;
	/** The angle of the robot */
	Correction correction;
	/** The type of movement of the robot */
	Axis axis;

	Direction direction;
}Base;

/**
 * @struct Location
 * @brief Coordinates of the Bot
 *
 * Describes the exact location of the bot and its position relative to the field
 */
typedef struct Location
{
	int x;
	int y;
}Location;

/*
 *	Global Variable Declarations
 */


/*
 *	@brief milliseconds between iterations of cortex
 *
 *	Defines the spacing between each iteration of the Cortex. Defined in milliseconds.
 */
#define TIME_DELAY 20

#define
