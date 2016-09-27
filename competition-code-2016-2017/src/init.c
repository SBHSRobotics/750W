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
#include "controller.h"


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
/*
 * ================= Primary Remote =================
 */

	ch1 = 0; // Horizontal Left Analog Stick
	ch2 = 0; // Vertical Left Analog Stick
	ch3 = 0; // Horizontal Right Analog Stick
	ch4 = 0; // Vertical Right Analog Stick

	ch5u = 0;  // Left Top Bumper
	ch5d = 0;  // Left Bottom Bumper

	ch6u = 0;  // Right Top Bumper
	ch6d = 0;  // Right Bottom Bumper

	ch7u = 0;  // Left Keypad Top Button
	ch7d = 0;  // Left Keypad Bottom Button
	ch7l = 0;  // Left Keypad Left Button
	ch7r = 0;  // Left Keypad Right Button

	ch8u = 0;  // Right Keypad Top Button
	ch8d = 0;  // Right Keypad Bottom Button
	ch8l = 0;  // Right Keypad Left Button
	ch8r = 0;  // Right Keypad Right Button

/*
 * ================= Secondary Remote =================
 */

	sch1 = 0;  // Horizontal Left Analog Stick
	sch2 = 0;  // Vertical Left Analog Stick
	sch3 = 0;  // Horizontal Right Analog Stick
	sch4 = 0;  // Vertical Right Analog Stick

	sch5u = 0; // Left Top Bumper
	sch5d = 0; // Left Bottom Bumper

	sch6u = 0; // Right Top Bumper
	sch6d = 0; // Right Bottom Bumper

	sch7u = 0; // Left Keypad Top Button
	sch7d = 0; // Left Keypad Bottom Button
	sch7l = 0; // Left Keypad Left Button
	sch7r = 0; // Left Keypad Right Button
	
	sch8u = 0; // Right Keypad Top Button
	sch8d = 0; // Right Keypad Bottom Button
	sch8l = 0; // Right Keypad Left Button
	sch8r = 0; // Right Keypad Right Button 
}
