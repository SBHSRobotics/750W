/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "API.h"
#include "global_variable_initialization.h"
#include "lcd.h"
#include "shooter.h"
#include "rotor.h"
#include "FPS.h"
#include "relative_and_global_angle.h"

Gyro gyroscope;

Encoder encoderBaseLeft;
Encoder encoderBaseRight;
Encoder encoderShooterLeft;
Encoder encoderShooterRight;

Ultrasonic ultrasonicRotorLeft;
Ultrasonic ultrasonicRotorRight;

unsigned long currentTime;
unsigned int opMode;
unsigned int autonMode;

float avgLeftShooterSpd;
float avgRightShooterSpd;

//added
bool imeSwitch;
int imeLMem;
int imeRMem;
//end added

//added
TaskHandle shooterHandle;
TaskHandle lcdHandle;
TaskHandle rotorHandle;
TaskHandle fpsHandle;
//end added

Base FPSBase;
Location position;

/*
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {
}

/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */
void initialize() {

	//This is to have a default base to then further do calculations on
	FPSBase = (Base){0, 0, (Correction){0, 0}, 0, (Direction){0, 0}};
	position = (Location){0, 0};
	
	gyroscope = gyroInit(GYRO_PORT, GYRO_MULTIPLIER);

	encoderBaseLeft = encoderInit(
			ENCODER_BASE_LEFT_PORT_TOP,
			ENCODER_BASE_LEFT_PORT_BOT,
			ENCODER_BASE_LEFT_REVERSED
		);
	encoderBaseRight = encoderInit(
			ENCODER_BASE_RIGHT_PORT_TOP,
			ENCODER_BASE_RIGHT_PORT_BOT,
			ENCODER_BASE_RIGHT_REVERSED
		);
	/*encoderBaseLeft = (Encoder)0;
	encoderBaseRight = (Encoder)0;*/

	imeInitializeAll();

	imeReset(IME_RIGHT_CHAIN_NUMBER);
	imeReset(IME_LEFT_CHAIN_NUMBER);

	encoderShooterLeft = encoderInit(
			ENCODER_SHOOTER_LEFT_PORT_TOP,
			ENCODER_SHOOTER_LEFT_PORT_BOT,
			ENCODER_SHOOTER_LEFT_REVERSED
		);
	encoderShooterRight = encoderInit(
			ENCODER_SHOOTER_RIGHT_PORT_TOP,
			ENCODER_SHOOTER_RIGHT_PORT_BOT,
			ENCODER_SHOOTER_RIGHT_REVERSED
		);

	ultrasonicRotorLeft 	= ultrasonicInit(ULTRASONIC_LEFT_PORT_ORANGE, ULTRASONIC_LEFT_PORT_YELLOW);
	ultrasonicRotorRight 	= ultrasonicInit(ULTRASONIC_RIGHT_PORT_ORANGE, ULTRASONIC_RIGHT_PORT_YELLOW);

	currentTime = millis();

	opMode = 0;
	autonMode = 0;

	avgLeftShooterSpd = 0;
	avgRightShooterSpd = 0;

	//added
	imeSwitch = false;
	imeLMem = 0;
	imeRMem = 0;
	//not added

	initGyroscope();

	lcdInit(uart1);
	initLcdPresets();

	//add at the end of the file
	startLcdDisplay();

	printf("Init to start tasks\n\r");

	int ignore = 0;
	shooterHandle = taskCreate(
			shooterTask,
			TASK_DEFAULT_STACK_SIZE,
			&ignore,
			TASK_PRIORITY_DEFAULT
			);
	lcdHandle = taskCreate(
			lcdTask,
			TASK_DEFAULT_STACK_SIZE,
			&ignore,
			TASK_PRIORITY_DEFAULT
			);
	/*rotorHandle = taskCreate(
			rotorTask,
			TASK_DEFAULT_STACK_SIZE,
			&ignore,
			TASK_PRIORITY_DEFAULT
			);*/
	fpsHandle = taskCreate(
			FPSTask,
			TASK_DEFAULT_STACK_SIZE,
			&ignore,
			TASK_PRIORITY_DEFAULT
			);
};
