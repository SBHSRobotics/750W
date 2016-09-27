/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
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
#include "autonomous_utility_methods.h"

//
#include <string.h>
//

unsigned int autonMode;

Ultrasonic ultrasonicRotorLeft;
Ultrasonic ultrasonicRotorRight;

/*void setControllers(void* baseRef, void** baseParameters, int sizeBaseParams) {
}*/

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
void autonomous()
{
	/*while(true)
		setShooter(94);*/

	/*
	 * turn rotor on
	 * wait 1 second
	 * turn shooter on
	 * wait 1 second
	 * turn on intake
	 */

	/*int sizeBase = sizeof(Base) + sizeof(int) + sizeof(float) + sizeof(int);

	void* dataBase = malloc(sizeBase);//calloc(sizeBase, sizeof(char));

	for (int i = 0; i < sizeBase / sizeof(char); i++) {
		char temp = (char)0;
		memcpy(dataBase + i, &temp, sizeof(char));
	}

	Base* now = (void*)dataBase;
	int* speed = (void*)now + (sizeof(Base) << 2);
	float* mobility = (void*)speed + (sizeof(int) << 2);
	int* direction = (void*)mobility + (sizeof(float) << 2);

	//
	//testing code

	*now = (Base){1, 2, (Correction){3, 4}, 5};
	*speed = 6;
	*mobility = 7.0f;
	*direction = 8;

	printf("dataBase: %p\n\r", dataBase);

	printf("\n\n");

	printf("now: %p\n\r", now);
	printf("now.left: %p, %d\n\r", &(now->left), now->left);
	printf("now.right: %p, %d\n\r", &(now->right), now->right);
	printf("now.correction.globalAngle: %p, %d\n\r", &(now->correction.globalAngle), now->correction.globalAngle);
	printf("now.correction.localAngle: %p, %d\n\r", &(now->correction.localAngle), now->correction.localAngle);
	printf("now.axis: %p, %d\n\r", &(now->axis), now->axis);

	printf("\n\n");

	printf("speed: %p, %d\n\r", speed, *speed);

	printf("\n\n");

	printf("mobility: %p, %f", mobility, *mobility);

	printf("\n\n");

	printf("direction: %p, %d", direction, *direction);*/

	//end testing code
	//

	/*TaskHandle baseHandle = taskCreate (
			baseController,
			TASK_DEFAULT_STACK_SIZE,
			dataBase,
			TASK_PRIORITY_DEFAULT
	);*/


	//pos2 is for shooter(preset)
	/*if (autonMode == 0) {
		unsigned long t0 = millis();
		unsigned long t = t0;
		while (t < t0 + 1000) {
			setRotor(0, 0, 0, 0, 1, 0);
			t = millis();
		}

		t = t0 = millis();
		while (t < t0 + 1000) {
			setRotor(0, 0, 0, 0, 1, 0);
			handleShooterUpdating(0, 0, 0, 0, 0, 0, 1);
			t = millis();
		}

		//t = t0 = millis();
		while(true) {//t < t0 + 1000) {
			setRotor(0, 0, 0, 0, 1, 0);
			handleShooterUpdating(0, 0, 0, 0, 0, 0, 1);
			setIntake(120, 120);
			//t = millis();
		}
	}
	else {

	}*//*
	while(true) {
		handleShooterUpdating(0, 0, 0, 127, 0, 0, 0);
	}*/

	if (autonMode == 0) {//15 sec autonomous
		handleShooterUpdating(0, 0, 0, 0, 0, 0, 1);

		delay(2 * 1000);

		while(true) {
			//handleShooterUpdating(0, 0, 0, 0, 1, 0, 0);
			//handleShooterUpdating(0, 0, 0, 0, 0, 0, 1);
			//setRotor(0, 0, 1, 0, 0, 0);
			//setIntake(120, 120);
			setIntake(60, 60);
		}

	}
	else if (autonMode == 1) {
		Base old = (Base){0, 0, (Correction){0, 0}, 0};

		//temp

		//delay(100 * 1000);
		//end temp

		//SHOOTER - Ramp up and shooting
		//ROTOR - Set rotor to shooting position
		handleShooterUpdating(0, 0, 0, 0, 0, 0, 1);
		//setRotor(0, 0, 0, 0, 1, 0);
		//delay(5 * 1000);
		delay(33 * 1000);

		/////////////////////////////////////////////////////////////////
		delay(100 * 1000);
		/////////////////////////////////////////////////////////////////

		//SHOOTER - Ramp down
		//Rotor - Lift ramp to turning angle
		shooterStop();
		//setRotor(0, 0, 0, 0, 0, 1);
		//delay(8 * 1000);

		//BASE - Turn right to go to the other side and correct
		old = turnRight(old, 120, 45);//51);//51.3401917 degree ~= 51
		//delay(8 * 1000);
		//old = turnLeft(old, 120, gyroGet(gyroscope));//abs(old.correction.globalAngle) + gyroGet(gyroscope) - 90);
		//delay(100 * 1000);

		delay(1500);

		//BASE - Move forward to other side
		//old = moveForward(old, 120, 3);
		old = moveForward(old, 120, 3);
		//delay(100 * 1000);

		delay(1500);

		//BASE - Turn to reorient the robot to face the other shooter
		old = turnLeft(old, 120, 105);//old = turnLeft(old, 120, 129);//180 - 51.3401917 ~= 180 - 51 = 129
		//old = turnRight(old, 120, gyroGet(gyroscope));

		//SHOOTER - Ramp up and shooting
		//ROTOR - Set rotor in shooting position
		handleShooterUpdating(0, 0, 0, 0, 0, 0, 1);
		//setRotor(0, 0, 0, 0, 1, 0);
		}
	else {//1 min autonomous

		//SETUP
		Base old = (Base){0, 0, (Correction){0, 0}, 0};
		unsigned long t0 = millis();
		unsigned long t = t0;

		//SHOOTER - Ramp up and shooting
		/*while(t < t0 + 28 * 1000) {
			handleShooterUpdating(0, 0, 0, 127, 0, 0, 0);
			t = millis();
		}*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//SHOOTER - Ramp down
		t = t0 = millis();
		while (t < t0 + 1000) {
			handleShooterUpdating(0, 0, 0, 0, 0, 0, 0);
			t = millis();
		}

		//BASE - Turn right to go to the other side
		old = turnRight(old, 120, 90);

		//SONAR - Get distance from the wall to correct later
		int wallDis0 = ultrasonicGet(ultrasonicRotorRight);

		//BASE - Move forward to the other side
		//old = moveForward(old, 120, 5.0f);/////////////////////////////////////////////////////////////////////////////////////////////////////
		old = moveForward(old, 120, 1.0f);

		//SONAR - Find current distance from the wall
		int wallDis = ultrasonicGet(ultrasonicRotorRight);

		//BASE - Turn left to get inline with the net
		old = turnLeft(old, 120, 90);

		//Base - Adjust for any drift
		float temp = (float)(wallDis - wallDis0) * TO_INCHES;
		temp = 0;////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(abs(temp) > 1.0f) {
			if(temp < 0)
				old = moveForward(old, 120, (-temp) / TILE_LENGTH);
			else
				old = moveBackward(old, 120, temp / TILE_LENGTH);
		}

		//SHOOTER - Ramp up and shooting
		while(true)
			handleShooterUpdating(0, 0, 0, 127, 0, 0, 0);
	}
}
