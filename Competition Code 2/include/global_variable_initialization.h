/**
 * @file global_variable_initialization.h
 *
 * @brief Global variable declaration
 *
 * Declaration of all the global variables and their association with one another
 */

#ifndef GLOBAL_VARIABLE_INITIALIZATION_H_
#define GLOBAL_VARIABLE_INITIALIZATION_H_

/**
 * Includes
 */
#include <math.h>
#include "API.h"

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

extern Location position;

/**
 * TIME_DELAY describes the time between iterations of the code
 */
#define TIME_DELAY 20
/**
 * currentTime describes what the current time of the cortex in
 * milliseconds is from when it was booted.
 */
extern unsigned long currentTime;


/**
 * opMode describes what mode the operator is in when controlling the robot
 * 		the two modes are: comp and rotor describing competition mode
 * 		and rotor calibration mode
 */
extern unsigned int opMode;
/**
 * autonMode describes what mode the autonomous is in when autonomous is
 * 		called. The two modes are: 15S and 1Min describing when to use the
 * 		15 second autonomous for competition matches and the 1 minute autonomous
 * 		for when the robot is competiting in programming skills
 */
extern unsigned int autonMode;


/* Analog Sensors - Sensors that return variable values */
/* Gyroscopic Sensor */
extern Gyro gyroscope;
#define GYRO_PORT 4
#define GYRO_MULTIPLIER 196

//Digital Sensors - Sensors that return on or off

//Shaft Encoder on the left side of the wheelbase
extern Encoder encoderBaseLeft;
//Ports and Reversal variable for Shaft Encoder on the left side of the wheelbase
#define ENCODER_BASE_LEFT_PORT_TOP 1
#define ENCODER_BASE_LEFT_PORT_BOT 2
#define ENCODER_BASE_LEFT_REVERSED 0

//Shaft Encoder on the left side of the wheelbase
extern Encoder encoderBaseRight;
//Ports and Reversal variable for Shaft Encoder on the right side of the wheelbase
#define ENCODER_BASE_RIGHT_PORT_TOP 3
#define ENCODER_BASE_RIGHT_PORT_BOT 4
#define ENCODER_BASE_RIGHT_REVERSED 1

//Shaft Encoder on the left side of the Flywheel Shooter
extern Encoder encoderShooterLeft;
//Ports and Reversal variable for Shaft Encoder on the left side of the Flywheel Shooter
#define ENCODER_SHOOTER_LEFT_PORT_TOP 5
#define ENCODER_SHOOTER_LEFT_PORT_BOT 6
#define ENCODER_SHOOTER_LEFT_REVERSED 1

//Shaft Encoder on the right side of the Flywheel Shooter
extern Encoder encoderShooterRight;
//Ports and Reversal variable for Shaft Encoder on the right side of the Flywheel Shooter
#define ENCODER_SHOOTER_RIGHT_PORT_TOP 7
#define ENCODER_SHOOTER_RIGHT_PORT_BOT 8
#define ENCODER_SHOOTER_RIGHT_REVERSED 0


//Ultrasonic (Sonar) Sensors

//Ultrasonic Sensor on the left side of the robot
extern Ultrasonic ultrasonicRotorLeft;
//Ports for the Ultrasonic Sensor on the left side of the robot
#define ULTRASONIC_LEFT_PORT_ORANGE 9
#define ULTRASONIC_LEFT_PORT_YELLOW 10

//Ultrasonic Sensor on the right side of the robot
extern Ultrasonic ultrasonicRight;
//Ports for the Ultrasonic Sensor on the right side of the robot
#define ULTRASONIC_RIGHT_PORT_ORANGE 11
#define ULTRASONIC_RIGHT_PORT_YELLOW 12


#define IME_LEFT_CHAIN_NUMBER 1
#define IME_LEFT_MULTIPLIER -1
#define IME_RIGHT_CHAIN_NUMBER 0
#define IME_RIGHT_MULTIPLIER -1

/**
 * Conversion from radians to degrees
 */
#define TO_DEGREE (180.0f/M_PI)
/**
 * Description of the base wheel radius
 */
#define WHEEL_RADIUS 2.0f
/**
 * Description of the base wheel circumference
 */
#define WHEEL_CIRCUMFERENCE (M_PI * 2.0f * WHEEL_RADIUS)
/**
 * Conversion from centimeters to inches
 */
#define TO_INCHES (2.54f)

/**
 * TILE_LENGTH is the length of a tile along its side
 * TODO: check for the need of TILE_RATIO_ADJUSTED
 */
#define TILE_LENGTH 24.0f
/**
 * TILE_RATIO_ADJUSTED is amount of degrees that a wheel has to travel to go
 * half the diagonal of a tile
 */
#define TILE_RATIO_ADJUSTED (TILE_LENGTH * 360.0f)//(TILE_LENGTH * M_SQRT2 * 360 * 1.0f/2.0f)


/**
 * used as the inverse proportionality constant for the Base PID adjustment
 * made to make sure the robot is going in a straight line
 * when it should be
 */
#define BASE_DIV 4


/**
 * This is used to make sure that the rotor does not go beyond a certain
 * angle to make sure it stays within the size requirements as well as
 * keep it from tipping when it goes too far inwards
 * TODO: implement DEGREE_TIP and DEGREE_MAX for the rotor
 */
#define DEGREE_TIP -200
/**
 * This is to make sure that the robot does not shrink too much in size
 * so that it will cause it to become unstable and tip
 */
#define DEGREE_MAX 200

/**
 * SHOOTER_ITERATIONS describes over how many iterations is the data for
 * the shooters position and speed to be remembered to get a smoothed
 * average
 */
#define SHOOTER_ITERATIONS 5
/**
 * @var avgLeftShooterSpd
 * This describes the change of the shooter rotation value between this iteration and last
 * @var avgRightShooterSpd
 * This describes the change of the shooter rotation value between this iteration and last
 */
extern float avgLeftShooterSpd;
extern float avgRightShooterSpd;

/**
 * @def MIN_SHOOTER_SPEED_6
 * Used to control the PID loop for the shooter and restrict the amount of
 * interference the PID has when it is not needed.
 * @def MIN_SHOOTER_DIFFERENCE
 * Used to control the PID loop for the shooter and restrict the amount of
 * interference the PID has when it is not needed.
 * @def SHOOTER_DIV
 * Used to control the PID loop for the shooter and restrict the amount of
 * interference the PID has when it is not needed.
 */
#define MIN_SHOOTER_SPEED_6 1500//7500
#define MIN_SHOOTER_DIFFERENCE 80//200//750/2
#define SHOOTER_DIV 40//100

/**
 * @def HOTKEY_POS_0
 * Used to control the hotkeys from the controllers to the rotor and shooter
 * to correspond to certain actions that have to be taken by them.
 * @def HOTKEY_POS_1
 * Used to control the hotkeys from the controllers to the rotor and shooter
 * to correspond to certain actions that have to be taken by them.
 * @def HOTKEY_POS_2
 * Used to control the hotkeys from the controllers to the rotor and shooter
 * to correspond to certain actions that have to be taken by them.
 * @def HOTKEY_POS_3
 * Used to control the hotkeys from the controllers to the rotor and shooter
 * to correspond to certain actions that have to be taken by them.
 */
#define HOTKEY_POS_0 1
#define HOTKEY_POS_1 2
#define HOTKEY_POS_2 4
#define HOTKEY_POS_3 8
//#define BTN_POS_4 16

/**
 * @def MOTOR_ROTOR_LEFT_PORT
 * 	Description of the motor port for the left rotor
 * @def MOTOR_ROTOR_RIGHT_PORT
 * 	Description of the motor port for the right rotor
 * @def MOTOR_BASE_TOP_LEFT_PORT
 * 	Description of the motor port for the top left base
 * @def MOTOR_BASE_TOP_RIGHT_PORT
 * 	Description of the motor port for the top right base
 * @def MOTOR_BASE_BOT_LEFT_PORT
 * 	Description of the motor port for the bottom left base
 * @def MOTOR_BASE_BOT_RIGHT_PORT
 * 	Description of the motor port for the bottom right base
 * @def MOTOR_SHOOTER_LEFT_PORT
 * 	Description of the motor port for the left shooter.
 * 	This goes through the power expander
 * @def MOTOR_SHOOTER_RIGHT_PORT
 * 	Description of the motor port for the right shooter.
 * 	This goes through the power expander
 * @def MOTOR_INTAKE_FRONT_PORT
 * 	Description of the motor port for the front intake
 * @def MOTOR_INTAKE_BACK_PORT
 * 	Description of the motor port for the back intake
 */
#define MOTOR_ROTOR_LEFT_PORT 2
#define MOTOR_ROTOR_RIGHT_PORT 9
#define MOTOR_BASE_TOP_LEFT_PORT 4
#define MOTOR_BASE_TOP_RIGHT_PORT 7
#define MOTOR_BASE_BOT_LEFT_PORT 3
#define MOTOR_BASE_BOT_RIGHT_PORT 8
#define MOTOR_SHOOTER_LEFT_PORT 5
#define MOTOR_SHOOTER_RIGHT_PORT 6
#define MOTOR_INTAKE_FRONT_PORT 10
#define MOTOR_INTAKE_BACK_PORT 1


/**
 * @def MOTOR_ROTOR_LEFT_MULTIPLIER -1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_ROTOR_RIGHT_MULTIPLIER -1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_BASE_TOP_LEFT_MULTIPLIER -1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_BASE_TOP_RIGHT_MULTIPLIER 1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_BASE_BOT_LEFT_MULTIPLIER 1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_BASE_BOT_RIGHT_MULTIPLIER -1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_SHOOTER_LEFT_MULTIPLIER -1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_SHOOTER_RIGHT_MULTIPLIER 1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_INTAKE_FRONT_MULTIPLIER -1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 * @def MOTOR_INTAKE_BACK_MULTIPLIER -1
 * Used to reverse the input to the motor
 * to compensate for the motor's orientation
 */
#define MOTOR_ROTOR_LEFT_MULTIPLIER -1
#define MOTOR_ROTOR_RIGHT_MULTIPLIER -1
#define MOTOR_BASE_TOP_LEFT_MULTIPLIER 1
#define MOTOR_BASE_TOP_RIGHT_MULTIPLIER -1
#define MOTOR_BASE_BOT_LEFT_MULTIPLIER 1
#define MOTOR_BASE_BOT_RIGHT_MULTIPLIER -1
#define MOTOR_SHOOTER_LEFT_MULTIPLIER 1
#define MOTOR_SHOOTER_RIGHT_MULTIPLIER -1
#define MOTOR_INTAKE_FRONT_MULTIPLIER -1
#define MOTOR_INTAKE_BACK_MULTIPLIER 1

/*
 * @def MAX_SPEED
 * This is the max speed that can be sent to a motor
 * @def INPUT_THRESHOLD
 * This is the minimum amount of power that can be sent to
 * 		a motor. If below this then it is zero effectively.
 */
#define MAX_SPEED 127
#define INPUT_THRESHOLD 15

/**
 * @def BASE_LINEAR_SPEED_INCREASE_MAXIMUM
 * 	Used to define the maximum amount of speed increase in one iteration that the base can have going straight
 * @def BASE_TURN_SPEED_INCREASE_MAXIMUM
 * 	Used to define the maximum amount of speed increase in one iteration that the base can have turning
 * @def SHOOTER_SPEED_INCREASE_MAXIMUM
 * 	Used to define the maximum amount of speed increase in one iteration that this motor can have
 * @def SHOOTER_SPEED_INCREASE_MINIMUM
 * 	Used to define the minimum amount of speed increase that is required to change the speed of
 * 	the motor
 * @def GENERAL_SPEED_INCREASE_MAXIMUM
 * 	Used to define the maximum amount of speed increase in one iteration that this motor can have
 * @def GENERAL_SPEED_INCREASE_MINIMUM
 * 	Used to define the minimum amount of speed increase that is required to change the speed of
 * 	the motor
 */
#define BASE_LINEAR_SPEED_INCREASE_MAXIMUM 6
#define BASE_TURN_SPEED_INCREASE_MAXIMUM 8
#define SHOOTER_SPEED_INCREASE_MAXIMUM_FAST 8
#define SHOOTER_SPEED_INCREASE_MINIMUM_FAST 2
#define SHOOTER_SPEED_INCREASE_MAXIMUM_SLOW 4
#define SHOOTER_SPEED_INCREASE_MINIMUM_SLOW 2
#define GENERAL_SPEED_INCREASE_MAXIMUM 4
#define GENERAL_SPEED_INCREASE_MINIMUM 2

/*
 * port of the second battery connected to the power expander that is connected
 * back to this digital port to observe its battery level in millivolts
 */
#define BATTERY_SECOND_PORT 1

//added
extern bool imeSwitch;
extern int imeLMem;
extern int imeRMem;

extern int leftShooterSpdHistory[SHOOTER_ITERATIONS];
extern int rightShooterSpdHistory[SHOOTER_ITERATIONS];
//end added

//added
extern TaskHandle shooterHandle;
extern TaskHandle lcdHandle;
extern TaskHandle rotorHandle;
extern TaskHandle fpsHandle;
//end added

extern Base FPSBase;

#endif /* GLOBAL_VARIABLE_INITIALIZATION_H_ */
