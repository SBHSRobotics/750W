#include "rotor.h"

int oldRotorSpeedL = 0;
int oldRotorSpeedR = 0;

bool pos0PressedR = false;
bool pos1PressedR = false;
bool pos2PressedR = false;

bool posResetPressedR = false;

int memPosR = -1;

//added
bool imeSwitch;
int imeLMem;
int imeRMem;
//end added

//prototype
bool adjustRotation(int* spdL, int* spdR, int targetPos, int L, int R);

bool adjustedRotorSet(int *speedL, int *speedR){
	bool ret = false;
	ret = adjustSpeed(speedL, oldRotorSpeedL, GENERAL_SPEED_INCREASE_MINIMUM, GENERAL_SPEED_INCREASE_MAXIMUM);
	ret |= adjustSpeed(speedR, oldRotorSpeedR, GENERAL_SPEED_INCREASE_MINIMUM, GENERAL_SPEED_INCREASE_MAXIMUM);
	return ret;
}

void setFinalRotorSpeeds(int speedL, int speedR, int L, int R, bool correct){
	if (correct) {
		//int L = 0;//encoderGet(encoderRotorL);
		//int R = 0;//encoderGet(encoderRotorR);

		/*bool leftCont = true, rightCont = true;;

		leftCont = imeGet(1, &L);
		rightCont = imeGet(0, &R);

		if (!leftCont || !rightCont) {
			L = 0;
			R = 0;
		}*/

		int threshold=30;


		int rotorDiffL = L - R;

		const int rotorDiv = 2;

		if (abs(rotorDiffL) > threshold){
			if (rotorDiffL>0){
				if (speedL>0 && speedR>0){
					speedL /= rotorDiv;
					//setFinalRotorSpeeds((int)(((double)(speedL))*0.5),speedR);
				}
				else if (speedL<0 && speedR<0){
					speedR /= rotorDiv;
					//setFinalRotorSpeeds(speedL,(int)(((double)(speedR))*0.5));
				}
			}
			if (rotorDiffL<0){
				if (speedL>0 && speedR>0){
					speedR /= rotorDiv;
					//setFinalRotorSpeeds(speedL,(int)(((double)(speedR))*0.5));
				}
				if (speedL<0 && speedR<0){
					speedL /= rotorDiv;
					//setFinalRotorSpeeds((int)(((double)(speedL))*0.5),speedR);
				}
			}
		}

		if (speedL == 0 && speedR == 0)
			if (abs(rotorDiffL) > threshold)
				adjustRotation(&speedL, &speedR, avg(L, R), L, R);

	}

	//if(millis() - time < timeDelay)
	//	return;
	if(!(adjustedRotorSet(&speedL, &speedR)))
		return;
	speedL = abs(speedL) > MAX_SPEED ? sgnSimple(speedL) * MAX_SPEED :
			abs(speedL) < INPUT_THRESHOLD ? 0 : speedL;
	speedR = abs(speedR) > MAX_SPEED ? sgnSimple(speedR) * MAX_SPEED :
			abs(speedR) < INPUT_THRESHOLD ? 0 : speedR;
	motorSet(MOTOR_ROTOR_LEFT_PORT, MOTOR_ROTOR_LEFT_MULTIPLIER * speedL);
	motorSet(MOTOR_ROTOR_RIGHT_PORT, MOTOR_ROTOR_RIGHT_MULTIPLIER * speedR);
	oldRotorSpeedL = speedL;
	oldRotorSpeedR = speedR;
}

void setRotorRaw(int speedL, int speedR) {
	setFinalRotorSpeeds(speedL, speedR, 0, 0, false);
}

bool adjustRotation(int* spdL, int* spdR, int targetPos, int L, int R) {
	bool change = false;

	int threshold = 40;

	bool changeLeft = false;
	bool changeRight = false;

	int diff = targetPos - L;
	int sgnS = sgn(diff);

	const int speedSlow = 30;
	const int speedFast = 120;

	if (abs(diff) > threshold) {
		change = true;
		changeLeft = true;
		if (abs(diff) < 2 * threshold)
			*spdL = sgnS * speedSlow;
		else
			*spdL = sgnS * speedFast;
	}

	diff = targetPos - R;
	sgnS = sgn(diff);
	if (abs(diff) > threshold) {
		change = true;
		changeRight = true;
		if (abs(diff) < 2 * threshold)
			*spdR = sgnS * speedSlow;
		else
			*spdR = sgnS * speedFast;
	}

	if ((changeLeft == false) || (changeRight == false))
	{
		*spdL = 0;
		*spdR = 0;
	}

	return change;
}

/*
 * fieldPos
 * -2 = be zero
 * -1 = go towards val
 * 0 is far
 * 1 is middle
 * 2 is close
 */
void setRotor(bool adjU, bool adjD, bool pos0, bool pos1, bool pos2, bool posReset) {//int speedL, int speedR){
	int spdL = 0, spdR = 0;
	int targetPos = 0;

	int L = 0;//encoderGet(encoderRotorL);
	int R = 0;//encoderGet(encoderRotorR);

	//bool leftCont = true, rightCont = true;

	//TODO: Fix the implementation and need for leftCont and rightCont and replace them potentially with L and R

	//leftCont = imeLMem;//imeGet(IME_LEFT_CHAIN_NUMBER, &L);
	//rightCont = imeRMem;//imeGet(IME_RIGHT_CHAIN_NUMBER, &R);
	L = imeLMem;
	R = imeRMem;

	L *= IME_LEFT_MULTIPLIER;
	R *= IME_RIGHT_MULTIPLIER;

	/*if (!leftCont || !rightCont) {
		if (adjU ^ adjD) {
			if (adjU)
				setRotorRaw(120, 120);
			else
				setRotorRaw(-120, -120);
		}
		else
			setRotorRaw(0, 0);
	}*/

	int input = 8 * posReset + 4 * pos2 + 2 * pos1 + pos0;
	input = adjustInputBtns(input, &pos0PressedR, &pos1PressedR, &pos2PressedR, &posResetPressedR);

	if (adjU ^ adjD) {
		memPosR = -2;
		if (adjU)
			spdL = spdR = 120;
		else
			spdL = spdR = -120;
	}
	else if (input & HOTKEY_POS_0)//start tile//pch7D
		memPosR = 0;
	else if (input & HOTKEY_POS_1)//middle//pch7R
		memPosR = 1;
	else if (input & HOTKEY_POS_2)//auton-1//pch7D
		memPosR = 2;
	else if (input & HOTKEY_POS_3)//auton-2
		memPosR = 3;
	else if (memPosR == -2)
		spdL = spdR = 0;

	if (memPosR >= 0) {
		if (memPosR == 0)//starting tile
			targetPos = 4740;//1605 * 4 / 5;
		else if (memPosR == 1)//middle tile
			targetPos = 0;//2125;//pch7R
		else if (memPosR == 2)//auton-1
			targetPos = 0;//1360;//pch7U
		else if (memPosR == 3)//auton-2
			targetPos = 300;//ch7L
		/*if (!adjustRotation(&spdL, &spdR, targetPos, L, R))
			memPosR = -1;*/
		adjustRotation(&spdL, &spdR, targetPos, L, R);
	}

	/*if (memPosR == -1)
		spdL = spdR = 0;*/

	setFinalRotorSpeeds(spdL, spdR, L, R, true);
}

void rotorTask(void* ignore) {
	bool ch5D = false;
	bool ch6D = false;
	bool ch8L = false;
	bool ch8R = false;

	bool ch5U = false;
	bool ch6U = false;

	while(true) {
		ch5D = joystickGetDigital(1, 5, JOY_DOWN);
		ch6D = joystickGetDigital(1, 6, JOY_DOWN);
		ch8L = joystickGetDigital(1, 8, JOY_LEFT);
		ch8R = joystickGetDigital(1, 8, JOY_RIGHT);

		ch5U = joystickGetDigital(1, 5, JOY_UP);
		ch6U = joystickGetDigital(1, 6, JOY_UP);

		if (opMode == 0) {
			setRotor(ch5D, ch6D,
				//false, false, false, false);
				ch8L, ch8R, false, false);
		}
		else if (opMode == 1) {
			int leftRotor = 0;
			int rightRotor = 0;
			if (ch5U ^ ch5D) {
				if (ch5U)
					leftRotor = 80;
				else
					leftRotor = -80;
			}
			if (ch6U ^ ch6D) {
				if (ch6U)
					rightRotor = 80;
				else
					rightRotor = -80;
			}
			setRotorRaw(leftRotor, rightRotor);
		}

		wait(TIME_DELAY);
	}
}
