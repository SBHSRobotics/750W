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
 #include "base.h"
 #include "global_vars.h"

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

/*
 * ================= Primary Remote =================
 */

	int ch1; // Horizontal Left Analog Stick
	int ch2;	// Vertical Left Analog Stick
	int ch3;	// Horizontal Right Analog Stick
	int ch4;	// Vertical Right Analog Stick

	bool ch5u; // Left Top Bumper
	bool ch5d; // Left Bottom Bumper

	bool ch6u; // Right Top Bumper
	bool ch6d; // Right Bottom Bumper

	bool ch7u; // Left Keypad Top Button
	bool ch7d; // Left Keypad Bottom Button
	bool ch7l; // Left Keypad Left Button
	bool ch7r; // Left Keypad Right Button

	bool ch8u; // Right Keypad Top Button
	bool ch8d; // Right Keypad Bottom Button
	bool ch8l; // Right Keypad Left Button
	bool ch8r; // Right Keypad Right Button

/*
 * ================= Secondary Remote =================
 */

	int sch1; // Horizontal Left Analog Stick
	int sch2; // Vertical Left Analog Stick
	int sch3; // Horizontal Right Analog Stick
	int sch4; // Vertical Right Analog Stick

	bool sch5u; // Left Top Bumper
	bool sch5d; // Left Bottom Bumper

	bool sch6u; // Right Top Bumper
	bool sch6d; // Right Bottom Bumper

	bool sch7u; // Left Keypad Top Button
	bool sch7d; // Left Keypad Bottom Button
	bool sch7l; // Left Keypad Left Button
	bool sch7r; // Left Keypad Right Button

	bool sch8u; // Right Keypad Top Button
	bool sch8d; // Right Keypad Bottom Button
	bool sch8l; // Right Keypad Left Button
	bool sch8r; // Right Keypad Right Button

/*
 *	================= Primary Remote =================
 */

Base RobotBase;

void operatorControl() {

	while (1) {
		/*
		 * ================= Primary Remote =================
		 */

		ch1 = joystickGetAnalog( 1 , 1 ); // Horizontal Left Analog Stick
		ch2 = joystickGetAnalog( 1 , 2 ); // Vertical Left Analog Stick
		ch3 = joystickGetAnalog( 1 , 3 ); // Horizontal Right Analog Stick
		ch4 = joystickGetAnalog( 1 , 4 ); // Vertical Right Analog Stick

		ch5u = joystickGetDigital( 1 , 5 , JOY_UP );  // Left Top Bumper
		ch5d = joystickGetDigital( 1 , 5 , JOY_DOWN );  // Left Bottom Bumper

		ch6u = joystickGetDigital( 1 , 6 , JOY_UP );  // Right Top Bumper
		ch6d = joystickGetDigital( 1 , 6 , JOY_DOWN );  // Right Bottom Bumper

		ch7u = joystickGetDigital( 1 , 7 , JOY_UP );  // Left Keypad Top Button
		ch7d = joystickGetDigital( 1 , 7 , JOY_DOWN );  // Left Keypad Bottom Button
		ch7l = joystickGetDigital( 1 , 7 , JOY_LEFT );  // Left Keypad Left Button
		ch7r = joystickGetDigital( 1 , 7 , JOY_RIGHT );  // Left Keypad Right Button

		ch8u = joystickGetDigital( 1 , 8 , JOY_UP );  // Right Keypad Top Button
		ch8d = joystickGetDigital( 1 , 8 , JOY_DOWN );  // Right Keypad Bottom Button
		ch8l = joystickGetDigital( 1 , 8 , JOY_LEFT );  // Right Keypad Left Button
		ch8r = joystickGetDigital( 1 , 8 , JOY_RIGHT );  // Right Keypad Right Button

		/*
		 * ================= Secondary Remote =================
		 */

		sch1 = joystickGetAnalog( 2 , 1 );  // Horizontal Left Analog Stick
		sch2 = joystickGetAnalog( 2 , 2 );  // Vertical Left Analog Stick
		sch3 = joystickGetAnalog( 2 , 3 );  // Horizontal Right Analog Stick
		sch4 = joystickGetAnalog( 2 , 4 );  // Vertical Right Analog Stick

		sch5u = joystickGetDigital( 2 , 5 , JOY_UP ); // Left Top Bumper
		sch5d = joystickGetDigital( 2 , 5 , JOY_DOWN ); // Left Bottom Bumper

		sch6u = joystickGetDigital( 2 , 6 , JOY_UP ); // Right Top Bumper
		sch6d = joystickGetDigital( 2 , 6 , JOY_DOWN ); // Right Bottom Bumper

		sch7u = joystickGetDigital( 2 , 7 , JOY_UP ); // Left Keypad Top Button
		sch7d = joystickGetDigital( 2 , 7 , JOY_DOWN ); // Left Keypad Bottom Button
		sch7l = joystickGetDigital( 2 , 7 , JOY_LEFT ); // Left Keypad Left Button
		sch7r = joystickGetDigital( 2 , 7 , JOY_RIGHT ); // Left Keypad Right Button

		sch8u = joystickGetDigital( 2 , 8 , JOY_UP ); // Right Keypad Top Button
		sch8d = joystickGetDigital( 2 , 8 , JOY_DOWN ); // Right Keypad Bottom Button
		sch8l = joystickGetDigital( 2 , 8 , JOY_LEFT ); // Right Keypad Left Button
		sch8r = joystickGetDigital( 2 , 8 , JOY_RIGHT ); // Right Keypad Right Button

		/*
		 *	Location ~ Movement
		 */

		RobotBase = baseSetQuad(RobotBase, ch4, ch3, ch5u, false, ch6u, false, 80);
		 
		delay(TIME_DELAY);
	}
}
