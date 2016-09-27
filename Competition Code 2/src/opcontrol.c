/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HFPSBaseERS AND CONTRIBUTORS "AS IS" AND
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

/**
 * Includes
 */
#include "main.h"
#include "base.h"
#include "global_variable_initialization.h"
#include "shooter.h"
#include "intake.h"
#include "rotor.h"
#include "lcd.h"
#include "FPS.h"

/**
 * @var encoderShooterL
 * This is the left shooter encoder and it is used here to reset it
 * @var encoderShooterR
 * This is the right shooter encoder and it is used here to reset it
 */
Encoder encoderShooterL;
Encoder encoderShooterR;00

/**
 * @var imeSwitch
 * @var imeLMem
 * @var imeRMem
 * These are to get the data from the ime's every other iteration to reduce
 * the load on the cortex
 * TODO: test if this actually reduces the load on the cortex
 */
bool imeSwitch;
int imeLMem;
int imeRMem;

Base FPSBase;

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
void operatorControl()
{

	/**
	 * Reset the encoders so that they start at speed 0 and rotation 0.
	 * It then updates properly
	 */
	encoderReset(encoderShooterL);
	encoderReset(encoderShooterR);

	/**
	 * @def ch5U
	 * @def ch5D
	 * @def ch8U
	 * @def ch8D
	 * @def ch3
	 * These are variables to assign to certain controller inputs
	 */
	bool ch5U = false;
	bool ch6U = false;

	bool ch8U = false;
	bool ch8D = false;

	int ch3 = 0;
	int ch4 = 0;

	/**
	 * @var intakeLeft
	 * @var intakeRight
	 * Reinterpret the input to the intake
	 */
	int intakeLeft = 0;
	int intakeRight = 0;

	while (1)
	{
		//Get the values from the controller
		ch3 = joystickGetAnalog(1, 3);
		ch4 = joystickGetAnalog(1, 4);
		ch5U = joystickGetDigital(1, 5, JOY_UP);
		ch6U = joystickGetDigital(1, 6, JOY_UP);
		ch8U = joystickGetDigital(1, 8, JOY_UP);
		ch8D = joystickGetDigital(1, 8, JOY_DOWN);

		//If opMode is 0 - when it is operator mode
		if (opMode == 0) {

			//Set the value of the base
			FPSBase = baseSetQuad(FPSBase, ch4, ch3, ch5U, false, ch6U, false, 80);

			/*for (int i = 1; i < 11; i++)
				motorSet(i, 60);*/

			//Change the values of the left and right intake depending on inputs
			if (ch8U ^ ch8D)
			{
				intakeLeft = ch8U ? 120 : -120;
			}
			else if (ch3 < -INPUT_THRESHOLD)
			{
				intakeLeft = 120;
			}
			else intakeLeft = 0;

			//intakeRight = ch8U ^ ch8D ? (ch8U ? 120 : -120) : 0;
			intakeRight = ch8U ^ ch8D ? (ch8U ? 100 : -100) : 0;

			//TODO: Temp for new robot
			intakeLeft = intakeRight;

			//set the intake to joystick values
			setIntake(intakeLeft, intakeRight);
		}

		//update the lcd every other iteration
		if (imeSwitch)
		{
			imeGet(IME_LEFT_CHAIN_NUMBER, &imeLMem);
			imeGet(IME_RIGHT_CHAIN_NUMBER, &imeRMem);

			imeSwitch = false;
		}
		else
			imeSwitch = true;

		//delay so that the cortex can rest and not overheat
		wait(TIME_DELAY);
	}
}

