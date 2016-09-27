//Includes
#include "shooter.h"

/**
 * @var oldShooterSpeedL
 * @var oldShooterSpeedR
 * This is the speed of the shooter at the last iteration
 */
int oldShooterSpeedL = 0;
int oldShooterSpeedR = 0;

/**
 * @var oldEncoderLeft
 * @var oldEncoderRight
 * This is the encoder value from the last iteration
 */
int oldEncoderLeft = 0;
int oldEncoderRight = 0;

/**
 * @var leftShooterRotHistory
 * @var rightShooterRotHistory
 * This tracks the shooters current rotation from several iterations
 */
//int leftShooterRotHistory[SHOOTER_ITERATIONS];
//int rightShooterRotHistory[SHOOTER_ITERATIONS];

//This is the shooterIndex in the shooterArray that it is currently at and is modifying
int shooterIndex = 0;

/**
 * @var leftShooterSpdHistory
 * @var rightShooterSpdHistory
 * This tracks the shooters current speed from several iterations
 */
int leftShooterSpdHistory[SHOOTER_ITERATIONS];
int rightShooterSpdHistory[SHOOTER_ITERATIONS];

/**
 * @var avgLeftShooterSpd
 * @var avgRightShooterSpd
 * This is the current average shooter speed
 */
float avgLeftShooterSpd = 0;
float avgRightShooterSpd = 0;

//This is the target RPM that the shooter should be at
int targetRPM;

/*
 * @var holdPressed
 * @var adjUPressed
 * @var adjDPressed
 * These control if the button is currently being pressed.
 * @var joystickCtrl
 * This controls if the input is coming from the joystick or from the buttons
 * @var joystickSpd
 * This controls the input from the joystick
 */
bool holdPressed = false;
bool adjUPressed = false;
bool adjDPressed = false;
bool joystickCtrl = true;
int joystickSpd = 0;

//This is the amount of speed that is to be added to the shooter to adjust it minutely
int shooterAddition = 0;

/**
 * @var pos0PressedS
 * @var pos1PressedS
 * @var pos2PressedS
 * These control if the preset button is being held
 */
bool pos0PressedS = false;
bool pos1PressedS = false;
bool pos2PressedS = false;

//This is the way the inputs should be interpreted
int memPosS = -1;

//This is the input proportion from 127 that is allowed from the joystick
const float maxInput = 110.0f;

/**
 * This method gets the average shooter speed and rotation and sets it to setLeft and setRight
 * @param setLeft is the average speed of the left fly wheel
 * @param setRight is the average speed of the right fly wheel
 * @param encoderLeft is the encoder value of the left fly wheel
 * @param encoderRight is the encoder value of the right fly wheel
 */
void getAvgShooterRotation(float *setLeft, float *setRight,
		int encoderLeft, int encoderRight){

	leftShooterSpdHistory[shooterIndex] =
			encoderLeft - oldEncoderLeft;
	rightShooterSpdHistory[shooterIndex] =
			encoderRight - oldEncoderRight;

	//leftShooterRotHistory[shooterIndex] = abs(encoderLeft);
	//rightShooterRotHistory[shooterIndex] = abs(encoderRight);

	avgLeftShooterSpd = avgNums(leftShooterSpdHistory);
	avgRightShooterSpd = avgNums(rightShooterSpdHistory);

	*setLeft = avgLeftShooterSpd * (50 / 6.0f) * 5.0f;
	*setRight = avgRightShooterSpd * (50 / 6.0f) * 5.0f;

	shooterIndex++;
	if (shooterIndex == SHOOTER_ITERATIONS) {
		shooterIndex = 0;
	}
}

/**
 * This adjusts the shooter speeds of the left and right fly wheels to allow them to slew
 * @param speedL This is the end speed of the left shooter
 * @param speedR This is the end speed of the right shooter
 * @return Whether or not either of the fly wheels values got changed
 */
bool adjustedShooterSet(int *speedL, int *speedR){
	bool ret = false;

	ret =	adjustSpeedSpecific(speedL, oldShooterSpeedL,
			SHOOTER_SPEED_INCREASE_MINIMUM_FAST, SHOOTER_SPEED_INCREASE_MAXIMUM_FAST,
			SHOOTER_SPEED_INCREASE_MINIMUM_SLOW, SHOOTER_SPEED_INCREASE_MAXIMUM_SLOW);
	ret |=	adjustSpeedSpecific(speedR, oldShooterSpeedR,
				SHOOTER_SPEED_INCREASE_MINIMUM_FAST, SHOOTER_SPEED_INCREASE_MAXIMUM_FAST,
				SHOOTER_SPEED_INCREASE_MINIMUM_SLOW, SHOOTER_SPEED_INCREASE_MAXIMUM_SLOW);
	return ret;
}

/**
 * This method changes the speeds of the left and right fly wheels to allow for it to slew and go towards its proper final value with the addition
 * @param speedL This is the input that the left flywheel should be around
 * @param speedR This is the input that the right flywheel should be around
 */
void setFinalShooterSpeeds(int speedL, int speedR){
	//Get the shooter encoder values
	int encoderLeft = encoderGet(encoderShooterLeft);
	int encoderRight = encoderGet(encoderShooterRight);

	//Get the average of the left and right shooter encoders
	float avgLeft = 0, avgRight = 0;
	getAvgShooterRotation(&avgLeft, &avgRight,
			encoderLeft, encoderRight);

	//TODO: check the values and algorithm

	//If the left or right shooter input is greater than the deadzone then actuate the set speed using the desired RPM
	if (abs(speedL) > INPUT_THRESHOLD) {
		if (abs(avgLeft) > MIN_SHOOTER_SPEED_6) {
			int diff = targetRPM - abs(avgLeft);
			if (abs(diff) > MIN_SHOOTER_DIFFERENCE) {
				diff /= SHOOTER_DIV;

				speedL += diff * sgnSimple(speedL);
			}
		}
	}
	if (abs(speedR) > INPUT_THRESHOLD) {
		if (abs(avgRight) > MIN_SHOOTER_SPEED_6) {
			int diff = targetRPM - abs(avgRight);
			if (abs(diff) > MIN_SHOOTER_DIFFERENCE) {
				diff /= SHOOTER_DIV;

				speedR += diff * sgnSimple(speedR);
			}
		}
	}
	//If the left or right shooter input is greater than the deadzone then actuate the set speeds of the two shooters towards their collective higher RPM
	if (abs(speedL) > INPUT_THRESHOLD || abs(speedR) > INPUT_THRESHOLD) {
		if (abs(avgLeft) > MIN_SHOOTER_SPEED_6 || abs(avgRight) > MIN_SHOOTER_SPEED_6) {
			int diff = abs(avgLeft) - abs(avgRight);
			if (abs(diff) > MIN_SHOOTER_DIFFERENCE) {
				diff /= 2 * SHOOTER_DIV;

				if (sgnSimple(diff))
					speedR += diff * sgnSimple(speedR);
				else
					speedL -= diff * sgnSimple(speedL);
			}
		}
	}

	//Make memory of the left and right shooter encoders
	oldEncoderLeft = encoderLeft;
	oldEncoderRight = encoderRight;

	//If the left or right shooter input is greater than the deadzone then skew the left and right shooters towards their respective speeds
	if (abs(speedL) > INPUT_THRESHOLD || abs(speedR) > INPUT_THRESHOLD) {
		if(!adjustedShooterSet(&speedL, &speedR))
			return;
	}

	//Adjust the shooter speeds of the left and right shooters so as to account for small deliberate changes
	speedL += shooterAddition;
	speedR += shooterAddition;

	//Sets left and right fly wheels to their respective speeds after it has been properly actuated and skewed
	motorSet(MOTOR_SHOOTER_LEFT_PORT, MOTOR_SHOOTER_LEFT_MULTIPLIER * (abs(speedL) < INPUT_THRESHOLD ? 0 : abs(speedL) > MAX_SPEED ? sgnSimple(speedL) * MAX_SPEED : speedL));
	motorSet(MOTOR_SHOOTER_RIGHT_PORT, MOTOR_SHOOTER_RIGHT_MULTIPLIER * (abs(speedR) < INPUT_THRESHOLD ? 0 : abs(speedR) > MAX_SPEED ? sgnSimple(speedR) * MAX_SPEED : speedR));
	//Remember the left and right speeds that are set to the shooter
	oldShooterSpeedL = speedL;
	oldShooterSpeedR = speedR;
}

/**
 * This method takes the input from the joystick and actuates it to be properly in line with the range set for the shooter
 * @param speed is the speed from the joystick that the shooter should be set to
 */
void setShooter(int speed){
	//Setting the proper range and its corresponding RPM
	const float shooterRatioSpd = maxInput / 127.0f;
	const float shooterRatioRPM = 2300.0f / maxInput;//Rotations per minute = RPM

	targetRPM = speed * shooterRatioRPM;

	speed *= shooterRatioSpd;
	//speed += shooterAddition;


	if (abs(speed) > INPUT_THRESHOLD){
		if (abs(speed) > MAX_SPEED)
			speed = sgnSimple(speed) * MAX_SPEED;
		setFinalShooterSpeeds(speed, speed);
	}
	else{
		setFinalShooterSpeeds(0, 0);
	}
}

/**
 * This method updates the action and reaction of the shooter corresponding to the input from the controller.
 * @param hold is whether or not to toggle the holding of the shooter speed
 * @param adjU is whether or not to add a minute positive change to the speed
 * @param adjD is whether or not to add a minute negative change to the speed
 * @param joy is the current value of the joystick
 * @param pos0 is whether or not to toggle going towards the first preset
 * @param pos1 is whether or not to toggle going towards the second preset
 * @param pos2 is whether or not to toggle going towards the third preset
 */
void handleShooterUpdating(bool hold, bool adjU, bool adjD, int joy,
		bool pos0, bool pos1, bool pos2) {

	//dummy variable used for the adjustInputBtns method
	bool posResetPressedS = 0;

	//adjusts the inputs from the presets and with if they are currently being held to only register their initial touch
	int input = 4 * pos2 + 2 * pos1 + pos0;
	input = adjustInputBtns(input, &pos0PressedS, &pos1PressedS, &pos2PressedS, &posResetPressedS);

	//This is to register if only one button has been pressed during this iteration
	int temp =
			(input & HOTKEY_POS_0 ? 1 : 0) +
			(input & HOTKEY_POS_1 ? 1 : 0) +
			(input & HOTKEY_POS_2 ? 1 : 0);

	if (temp == 1) {

		//This is to set the presets to hold it for the preset values
		hold = true;
		holdPressed = false;
		joystickCtrl = true;

		//Value that the joystickSpd has to be set to has to be (x * 127 / maxInput)

		if (input & HOTKEY_POS_0) {//start tile//pch7D
			joystickSpd = 127;//pch7D
		}
		else if (input & HOTKEY_POS_1) {//middle tile//pch7R
			joystickSpd = 90;//68;
		}
		else if (input & HOTKEY_POS_2) {//auton//pch7U
			joystickSpd = 78;//74;//72;//81;//83;//87;
		}
	}

	//Adjust the input from the hold button to only register it when it is first pressed
	if(!holdPressed) {
		if (hold) {
			holdPressed = true;

			//adjUPressed = false;
			//adjDPressed = false;
		}
	}
	else if (holdPressed) {
		if (!(hold))
			holdPressed = false;
		else
			hold = false;
	}

	//
	if (hold)
		joystickCtrl ^= 1;
	if (joystickCtrl) {
		setShooter((int)abs(joystickSpd = joy));
		shooterAddition = 0;
	} else {
		if(!adjUPressed) {
			if (adjU)
				adjUPressed = true;
		}
		else if (adjUPressed) {
			if (!adjU)
				adjUPressed = false;
			else
				adjU = false;
		}
		if(!adjDPressed) {
			if (adjD)
				adjDPressed = true;
		}
		else if (adjDPressed) {
			if (!adjD)
				adjDPressed = false;
			else
				adjD = false;
		}

		if (adjU ^ adjD) {
			if (adjU) {
				shooterAddition += 4;
			}
			if (adjD) {
				shooterAddition -= 4;
			}
			/*if (abs(joystickSpd) > 127)
				joystickSpd = sgn(joystickSpd) * 127;*/
		}

		setShooter((int)abs(joystickSpd));
	}
}

void shooterStop() {
	targetRPM = 0;

	holdPressed = false;
	adjUPressed = false;
	adjDPressed = false;
	joystickCtrl = true;
	joystickSpd = 0;

	shooterAddition = 0;

	setFinalShooterSpeeds(0, 0);
}

int oldLinearPuncher = 0;

void linearPuncherUpdating(int speed)
{
	adjustSpeed(&speed, oldLinearPuncher, GENERAL_SPEED_INCREASE_MINIMUM, GENERAL_SPEED_INCREASE_MAXIMUM);
	motorSet(5, speed);
	motorSet(6, speed);
}

void shooterTask(void* ignore) {

	bool ch7L = false;
	bool ch7U = false;
	bool ch7D = false;
	int ch2 = false;
	int ch3 = false;
	bool ch8L = false;
	bool ch8R = false;

	while(true) {

		ch7L = joystickGetDigital(1, 7, JOY_LEFT);
		ch7U = joystickGetDigital(1, 7, JOY_UP);
		ch7D = joystickGetDigital(1, 7, JOY_DOWN);
		ch2 = joystickGetAnalog(1, 2);
		ch3 = joystickGetAnalog(1, 3);
		ch8L = joystickGetDigital(1, 8, JOY_LEFT);
		ch8R = joystickGetDigital(1, 8, JOY_RIGHT);

		if (opMode == 0) {

			/*handleShooterUpdating(ch7L, ch7U, ch7D, ch2,
				ch8L, ch8R, false);
				//false, false, false);*/
			linearPuncherUpdating(ch2);

		}
		else if (opMode == 2) {

			//setFinalShooterSpeeds(abs(ch3), abs(ch2));
			/*setFinalShooterSpeeds(ch3, ch2);*/
			linearPuncherUpdating(ch2);

		}

		wait(TIME_DELAY);
	}
}

