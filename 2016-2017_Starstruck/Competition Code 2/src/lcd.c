#include "lcd.h"

/*
 * currentPage is the specific lcd display and corresponding logic that will be looked
 * up when referencing this number.
 */
int currentPage;

/*
 * currentMotorNum is used to describe the current motor being specified
 *
 * currentMotorSpeed is the current motor speed of the motor specified from
 * 		currentMotorNum
 */
int currentMotorNum;
int currentMotorSpeed;

/*
 * Knowledge about the left button on the lcd being held down
 * Knowledge about the center button on the lcd being held down
 * Knowledge about the right button on the lcd being held down
 */
bool leftPressed;
bool centerPressed;
bool rightPressed;

/*
 * startLink describes the start of the pages that correspond to the link established for
 * 		cortex
 * startBattery describes the start of the pages that correspond to the millivoltage level
 * 		of the primary battery on the cortex, the secondary battery on the power expander,
 * 		and the backup 9 volt battery.
 * startSensor describes the start of the pages that correspond to the different sensor values
 * 		from the shaft-encoders, ime's, gyroscope, and accelerometer.
 * startMotor describes the start of the pages that correspond to the different motor values
 * 		that make up all the motors attached to the cortex.
 * startAuton describes the start of the pages that correspond to the different autonMode's
 * 		that can be selected from
 * startMode describes the start of the pages that correspond to the different opMode's that can be
 * 		selected
 */
#define startLink 0
#define startBattery 100
#define startSensor 200
#define startMotor 300
#define startAuton 400
#define startMode 500
#define startFPS 600

/*
 * currentGroupNum describes the motor group that is currently being selected
 * currentGroundIndex specifies the index of the motor within the group currently selected
 * currentGroupSpeed is the speed of the motor selected by currentGroupIndex
 */
int currentGroupNum;
int currentGroupIndex;
int currentGroupSpeed;

/*
 * NUM_GROUPS defines the number of groups that are currently accessible
 * MAXIMUM_MOTORS_GROUP describes the maximum motors that can be in a group
 * Group describes the group as well as the motors within the group
 */
#define NUM_GROUPS 4
#define MAXIMUM_MOTORS_GROUP 5
typedef struct Group
{
	const char* groupName;//the groupName can have a max of 5 characters
	int validMotors;//number of motors out of the MAXIMUM_MOTORS_GROUP slots allocated are being used
	int motors[MAXIMUM_MOTORS_GROUP];//array of motors that can be used.
}Group;

/*
 * groups are the groups that can be accessed and are created
 */
Group groups[NUM_GROUPS];

//added

int sensIndex = 1;
int imeNum = 0;
Gyro gyroscope;

//end added


//temp

int quadNum = 0;

Encoder encoderRotorL;
Encoder encoderRotorR;
Encoder encoderShooterL;
Encoder encoderShooterR;

unsigned int opMode;
unsigned int autonMode;

//added
bool imeSwitch;
int imeLMem;
int imeRMem;

int leftShooterSpdHistory[SHOOTER_ITERATIONS];
int rightShooterSpdHistory[SHOOTER_ITERATIONS];
//end added

Base FPSBase;
Location position;
Gyro gyroscope;

//TODO: change all the member descriptions so that they include the returns and parameters

//

/*
 * Initializes all the data that will be used by the lcd
 */
void initLcdPresets();

/*
 * Initializes the lcd display for the display when it boots
 */
void startLcdDisplay();

/*
 * adjust the input from the controller to allow for only the initial press of a button
 * to be registered
 */
unsigned int checkClickLcdInput(unsigned int input);

/*
 * Updates the top and bottom row of the lcd
 */
void updateLcdScreen(const char* top, const char* bot);

/*
 * Allows for the updating of non-static pages
 */
void handleLcdUpdateExceptions();

/*
 * resolves the logic specified for the specific page
 */
void handleLcdInput();

/*
 * resolves the display for the specific page
 */
void handleLcdScreen();


void initLcdPresets()
{
	currentPage = 0;

	leftPressed = false;
	centerPressed = false;
	rightPressed = false;

	currentMotorNum = 1;
	currentMotorSpeed = 0;

	currentGroupNum = 0;
	currentGroupIndex = 0;
	currentGroupSpeed = 0;

	groups[0] = (Group){
			"Base",
			4,
			{2, 3, 8, 9, 0}
	};
	groups[1] = (Group){
		"In",
		2,
		{1, 10, 0, 0, 0}
	};
	groups[2] = (Group){
		"Shoot",
		2,
		{5, 6, 0, 0, 0}
	};
	groups[3] = (Group){
		"rotor",
		2,
		{2, 9, 0, 0, 0}
	};

	lcdSetBacklight(uart1, true);
}

void startLcdDisplay()
{
	lcdClear(uart1);
	updateLcdScreen("  Link Status", "<      --      >");
}

unsigned int checkClickLcdInput(unsigned int input)
{
	if (!leftPressed)
	{
		if (input & LCD_BTN_LEFT)
			leftPressed = true;
	}
	else if (leftPressed)
	{
		if (!(input & LCD_BTN_LEFT))
			leftPressed = false;
		else
			input ^= LCD_BTN_LEFT;
	}

	if (!centerPressed)
	{
		if (input & LCD_BTN_CENTER)
			centerPressed = true;
	}
	else if (centerPressed)
	{
		if (!(input & LCD_BTN_CENTER))
			centerPressed = false;
		else
			input ^= LCD_BTN_CENTER;
	}

	if (!rightPressed)
	{
		if(input & LCD_BTN_RIGHT)
			rightPressed = true;
	}
	else if (rightPressed)
	{
		if (!(input & LCD_BTN_RIGHT))
			rightPressed = false;
		else
			input ^= LCD_BTN_RIGHT;
	}

	return input;
}

void updateLcdScreen(const char* top, const char* bot)
{
	lcdPrint(uart1, 1, "%s", top);
	lcdPrint(uart1, 2, "%s", bot);
}

void handleLcdUpdating(unsigned int input)
{
	input = checkClickLcdInput(input);

	int temp =
			(input & LCD_BTN_LEFT ? 1 : 0) +
			(input & LCD_BTN_RIGHT ? 1 : 0) +
			(input & LCD_BTN_CENTER ? 1 : 0);

	//if (((input & LCD_BTN_LEFT) ^ (input & LCD_BTN_RIGHT)) ^ (input & LCD_BTN_CENTER))
	if (temp == 1)
	{
		handleLcdInput(input);

		//lcd can only hold 16 characters per line
		handleLcdScreen();
	}

	handleLcdUpdateExceptions();
}

void handleLcdUpdateExceptions()
{
	switch(currentPage)
	{
	case(startLink + 1):
	case(startLink + 2):
	{
		currentPage = startLink + (isJoystickConnected(1) ? 1 : 2);
		break;
	}
	case(startBattery + 2):
	{
		lcdPrint(uart1, 1, "Main mVolt: Secn");
		lcdPrint(uart1, 2, "%u   Rtn  %d", powerLevelMain(), (int)((analogRead(BATTERY_SECOND_PORT) * 1000)/280.0f));//70.0f * 4.0f
		break;
	}
	case(startBattery + 3):
	{
		lcdPrint(uart1, 1, "Back mVolt: %u", powerLevelBackup());
		lcdPrint(uart1, 2, "       Rtn");
		break;
	}
	case(startSensor + 2):
	{
		/*
		 * QUADRATURE: Rapid Updating
		 *
		 * 	Reads the data recieved from the Encoder.
		 * It then is able to handle any exceptions that
		 * are thrown by the Encoder. It prints the
		 * index of the Encoder (it's location) followed
		 * by the data that it is recieving. On the
		 * second line it prints buttons that are to
		 * iterate between the Encoders.
		 *
		 * *( See handleLcdInput() )*
		 *
		 */

		int quad = 0;
		switch(quadNum)
		{
		case(0):
		{
			quad = encoderGet(encoderBaseLeft);
			lcdPrint(uart1, 1, "Quad bL : %d", quad );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		case(1):
		{
			quad = encoderGet(encoderBaseRight);
			lcdPrint(uart1, 1, "Quad bR : %d", quad );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		case(2):
		{
			quad = encoderGet(encoderShooterLeft);
			lcdPrint(uart1, 1, "Quad sL : %d", quad );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		case(3):
		{
			quad = encoderGet(encoderShooterRight);
			lcdPrint(uart1, 1, "Quad sR : %d", quad );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		}
		break;
	}
	case(startSensor + 3):
	{
		/*
		 * IME: Rapid Updating
		 *
		 * 	Reads the data from the IME. It then is able to
		 * handle any exceptions thrown by the IME. It then
		 * prints the IME's index (location) and the data
		 * recieved by the IME. On the second line it prints
		 * the buttons needed to iterate between the IME's.
		 *
		 * *(See handleLcdUpdating() )*
		 *
		 */
		int ime;
		switch(imeNum){
		case(0):
		{
			ime = imeLMem;
			ime *= IME_LEFT_MULTIPLIER;
			lcdPrint(uart1, 1, "IME bL : %d", ime );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		case(1):
		{
			ime = imeRMem;
			ime *= IME_RIGHT_MULTIPLIER;
			lcdPrint(uart1, 1, "IME bR : %d", ime );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		}
		break;
	}
	case(startSensor + 4):
	{
		/*
		 * GYROSCOPE: Rapid Updating
		 *
		 * 	Reads the data recieved from the Gyroscope.
		 * It then is able to handle any exceptions that
		 * are thrown by the Gyroscope. It prints "Gyro"
		 * followed by the data that it is recieving. On the
		 * second line it prints the button to return back
		 * to the Sensor Selection.
		 *
		 * *( See handleLcdInput() )*
		 *
		 */
		int gy = gyroGet(gyroscope) % 360;
		lcdPrint(uart1, 1, "Gyroscope : %d", gy);
		lcdPrint(uart1, 2, "      Rtn");
		break;
	}
	case(startSensor + 5):
	{
		/*
		 * Shooting Information: Rapid Updating
		 *
		 * Reads the data from the rotor IME's and from the shooter shaft encoder.
		 * It then processes that information to give all the relavent information
		 * for shooting on one screen. Does not have room to display the return
		 * button text. Does not have any action for the left and right buttons.
		 * The center button returns to the startSensor page.
		 */
		int iL = imeLMem;
		int iR = imeRMem;

		iL *= IME_LEFT_MULTIPLIER;
		iR *= IME_RIGHT_MULTIPLIER;

		float sL = avgLeftShooterSpd * (50 / 6.0f) * 5.0f;//25.0f;
		float sR = avgRightShooterSpd * (50 / 6.0f) * 5.0f;//25.0f;

		lcdPrint(uart1, 1, "l:%5d, r:%5d", (int)sL, (int)sR);
		lcdPrint(uart1, 2, "L%6d  R%6d", iL, iR);

		break;
	}
	case(startMotor + 4):
	{
		int avg = 0;
		int counter = 0;
		for (int i = 0; i < groups[currentGroupNum].validMotors; i++)
		{
			avg += motorGet(groups[currentGroupNum].motors[i]);
			counter++;
		}
		if (counter != 0)
		{
			avg /= counter;
		}
		if (currentGroupSpeed != avg)
		{
			currentGroupSpeed = avg;
			lcdPrint(uart1, 1, "Grp Avg Spd:%d", currentGroupSpeed);
		}
		break;
	}
	case(startMotor + 7):
	{
		int temp = motorGet(groups[currentGroupNum].motors[currentGroupIndex]);
		if (currentGroupSpeed != temp)
		{
			currentGroupSpeed = temp;
			lcdPrint(uart1, 1, "Spd-GrIn-%s:%d",
					groups[currentGroupNum].groupName,
					currentGroupSpeed);
		}
		break;
	}
	case(startMotor + 10):
	{
		int temp = motorGet(currentMotorNum);
		if (currentMotorSpeed != temp)
		{
			currentMotorSpeed = temp;
			lcdPrint(uart1, 1, "Spd-Ind-%d:%d",
					currentMotorNum,
					currentMotorSpeed);
		}
		break;
	}
	case(startAuton + 1):
	{
		char* temp1 = "15S";
		char* temp2 = "1Min";

		char* s1 = (void*)"";
		char* s2 = (void*)"";

		if (autonMode == 0) {
			s1 = (void*)temp1;
			s2 = (void*)temp2;
		}
		else {
			s1 = (void*)temp2;
			s2 = (void*)temp1;
		}


		lcdPrint(uart1, 1, " AutonMode: %s", s1);
		lcdPrint(uart1, 2, "%s  Rtn %s", s2, s2);
		break;
	}
	case(startMode + 1):
	{
		char* temp1 = "comp";
		char* temp2 = "rotor";
		char* temp3 = "shoot";//max 6 characters

		char* s1 = "";
		char* s2 = "";
		char* s3 = "";

		if (opMode == 0)
		{
			s1 = (void*)temp1;
			s2 = (void*)temp2;
			s3 = (void*)temp3;
		}
		else if (opMode == 1)
		{
			s1 = (void*)temp2;
			s2 = (void*)temp3;
			s3 = (void*)temp1;
		}
		else if (opMode == 2)
		{
			s1 = (void*)temp3;
			s2 = (void*)temp1;
			s3 = (void*)temp2;
		}

		lcdPrint(uart1, 1, "OpMode: %s", s1);
		lcdPrint(uart1, 2, "%s Rtn %s", s3, s2);
		break;
	}
	case(startFPS + 1):
	{
		lcdPrint(uart1, 1, "x:%5d, y:%5d", position.x, position.y);
		lcdPrint(uart1, 2, "g:%4d, a:%d",
				//getLocalAngle(gyroscope), getGlobalAngle(FPSBase.correction, gyroscope));
				getGlobalAngle(FPSBase.correction, gyroscope), FPSBase.axis);
		break;
	}
	}
}

void handleLcdInput(unsigned int input)
{
	switch(currentPage)
	{
	case(startLink)://Link Status
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentPage = startFPS;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startBattery;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			//check link status
			//if connected go to connected
			//else go to not connected
			if (isJoystickConnected(1))
			{
				currentPage = startLink + 1;
			}
			else
			{
				currentPage = startLink + 2;
			}
			break;
		}
		}
		break;
	}
	case(startBattery)://Battery Status
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentPage = startLink;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startSensor;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startBattery + 1;
			break;
		}
		}
		break;
	}
	case(startSensor)://Sensor Status
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentPage = startBattery;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startMotor;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			quadNum = 0;
			imeNum = 0;
			sensIndex = 1;

			currentPage = startSensor + 1;
			break;
		}
		}
		break;
	}
	case(startMotor)://Motor Status
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentPage = startSensor;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startAuton;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentMotorNum = 1;
			currentPage = startMotor + 1;
			break;
		}
		}
		break;
	}
	case(startAuton)://Auton Status
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentPage = startMotor;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startMode;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startAuton + 1;
			break;
		}
		}
		break;
	}
	case(startMode)://Op Mode status
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentPage = startAuton;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startFPS;;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMode + 1;
			break;
		}
		}
		break;
	}
	case(startFPS):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentPage = startMode;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startLink;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startFPS + 1;
			break;
		}
		}
		break;
	}
	case(startLink + 1)://Link Status - Link Est.
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			//nothing
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			//nothing
			break;
		}
		case(LCD_BTN_CENTER):
		{
			//returns to top of branch
			currentPage = startLink;
			break;
		}
		}
		break;
	}
	case(startLink + 2)://Link Status - Link Not Est.
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			//nothing
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			//nothing
			break;
		}
		case(LCD_BTN_CENTER):
		{
			//return to top of branch
			currentPage = startLink;
			break;
		}
		}
		break;
	}
	case(startBattery + 1)://Battery Status - Battery Selector
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentPage = startBattery + 2;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startBattery + 3;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startBattery;
			break;
		}
		}
		break;
	}
	case(startBattery + 2)://Battery Status - Battery Selector - Prims Battery Status
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			//nothing
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			//nothing
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startBattery;
			break;
		}
		}
		break;
	}
	case(startBattery + 3)://Battery Status - Battery Selector - Backup
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			//nothing
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			//nothing
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startBattery;
			break;
		}
		}
		break;
	}
	case(startSensor + 1):
	{
		/*
		 * SENSOR SELECTION MENU
		 *
		 * 	Menu for selecting individual types of sensors such
		 * which can then have individual sensors. This screen
		 * will display the name of the Sensor Group along with
		 * the Buttons to select the sensor group and iterate
		 * between individual sensor groups.
		 *
		 */
		switch(input)
		{
			/*
			 * LEFT BUTTON: Previous Sensor Group
			 * CENTER BUTTON: Select Current Sensor Group
			 * RIGHT BUTTON: Next Sensor Group
			 *
			 */
			case(LCD_BTN_LEFT):
			{
				sensIndex--;
				if(sensIndex < 1){
					sensIndex = 4;
				}
				break;
			}
			case(LCD_BTN_RIGHT):
			{
				sensIndex++;
				if(sensIndex > 4){
					sensIndex = 1;
				}
				break;
			}
			case(LCD_BTN_CENTER):
			{
				//the +1 must be added to go beyond this page(startSensor + 1)
				currentPage = startSensor + sensIndex + 1;
				break;
			}
		}
		break;
	}
	case(startSensor + 2):
	{
		/*
		 * QUADRATURE: Button Control
		 *
		 * 	There can be multiple Quadratures meaning the
		 * case has functions to iterate between the
		 * individual Quadratures. The screen will display
		 * both the data of the current Quadrature and
		 * the buttons to iterate between the other
		 * individual Quadratures.
		 *
		 */
		switch(input)
		{
			/*
			 * LEFT BUTTON: Previous Quadrature
			 * CENTER BUTTON: Return to Main Page
			 * RIGHT BUTTON: Next Quadrature
			 *
			 */
			case(LCD_BTN_LEFT):
			{
				quadNum--;
				if(quadNum < 0)
					quadNum = 3;
				break;
			}
			case(LCD_BTN_RIGHT):
			{
				quadNum++;
				if(quadNum > 3)
					quadNum = 0;

				break;
			}
			case(LCD_BTN_CENTER):
			{
				currentPage = startSensor;
				break;
			}
		}
		break;
	}
	case(startSensor + 3):
	{
		/*
		 * IME: Button Control
		 *
		 * 	There can be multiple IME's meaning the IME
		 * case has functions to iterate between the
		 * individual IME's. The screen will display
		 * both the data and the buttons to iterate
		 * between the individual IME's.
		 *
		 */
		switch(input)
		{
			/*
			 * LEFT BUTTON: Previous IME
			 * CENTER BUTTON: Return to Main Page
			 * RIGHT BUTTON: Next IME
			 *
			 */
			case(LCD_BTN_LEFT):
			{
				imeNum--;
				if(imeNum < 0)
					imeNum = 1;
				break;
			}
			case(LCD_BTN_RIGHT):
			{
				imeNum++;
				if(imeNum > 1)
					imeNum = 0;
				break;
			}
			case(LCD_BTN_CENTER):
			{
				currentPage = startSensor;
				break;
			}
		}
		break;
	}
	case(startSensor + 4):
	{
		/*
		 * GYROSCOPE: Button Control
		 *
		 * 	There will only be one Gyroscope at all times,
		 * meaning the side buttons will have to do nothing.
		 * This results in the only usage of the middle
		 * button to be returning to the main Sensor screen.
		 *
		 */
		switch(input)
		{
			/*
			 * LEFT BUTTON: No Function
			 * CENTER BUTTON: Return to aMain Page
			 * RIGHT BUTTON: No Function
			 *
			 */
			case(LCD_BTN_LEFT):
			{
				break;
			}
			case(LCD_BTN_RIGHT):
			{
				break;
			}
			case(LCD_BTN_CENTER):
			{
				currentPage = startSensor;
				break;
			}
		}
		break;
	}
	case(startSensor + 5):
	{
		switch(input)
		{
			case(LCD_BTN_LEFT):
			{
				break;
			}
			case(LCD_BTN_RIGHT):
			{
				break;
			}
			case(LCD_BTN_CENTER):
			{
				currentPage = startSensor;
				break;
			}
		}
		break;
	}
	case(startMotor + 1):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentGroupNum = 0;
			currentPage = startMotor + 2;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentMotorNum = 1;
			currentPage = startMotor + 9;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor;
			break;
		}
		}
		break;
	}
	case(startMotor + 2):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentGroupNum--;
			if(currentGroupNum < 0)
			{
				currentGroupNum = NUM_GROUPS - 1;
			}
			//currentPage = startMotor + 2;
			//stay on to this page
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentGroupNum++;
			if(currentGroupNum == NUM_GROUPS)
			{
				currentGroupNum = 0;
			}
			//currentPage = startMotor + 2;
			//stay on to this page
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor + 3;
			break;
		}
		}
		break;
	}
	case(startMotor + 3):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			/*int avg = 0;
			int counter = 0;
			for (int i = 0; i < groups[currentGroupNum].validMotors; i++)
			{
				avg += motorGet(groups[currentGroupNum].motors[i]);
				counter++;
			}
			if(counter != 0)
			{
				avg /= counter;
			}
			currentGroupSpeed = avg;*/
			currentPage = startMotor + 4;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentGroupIndex = 0;
			currentPage = startMotor + 6;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor + 1;
			break;
		}
		}
		break;
	}
	case(startMotor + 4):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			for (int i = 0; i < groups[currentGroupNum].validMotors; i++)
			{
				motorStop(groups[currentGroupNum].motors[i]);
			}
			//currentGroupSpeed = 0;
			//currentPage = startMotor + 4;
			//stay on this page
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startMotor + 5;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor + 3;
			break;
		}
		}
		break;
	}
	case(startMotor + 5):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			for (int i = 0; i < groups[currentGroupNum].validMotors; i++)
			{
				motorSet(groups[currentGroupNum].motors[i], -127);
			}
			//currentGroupSpeed = -127;
			currentPage = startMotor + 4;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			for (int i = 0; i < groups[currentGroupNum].validMotors; i++)
			{
				motorSet(groups[currentGroupNum].motors[i], 127);
			}
			//currentGroupSpeed = 127;
			currentPage = startMotor + 4;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor + 4;
			break;
		}
		}
		break;
	}
	case(startMotor + 6):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentGroupIndex--;
			if (currentGroupIndex < 0)
			{
				currentGroupIndex = groups[currentGroupNum].validMotors - 1;
			}
			//currentPage = startMotor + 6;
			//stay on this page
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentGroupIndex++;
			if (currentGroupIndex == groups[currentGroupNum].validMotors)
			{
				currentGroupIndex = 0;
			}
			//currentPage = startMotor + 6;
			//stay on this page
			break;
		}
		case(LCD_BTN_CENTER):
		{
			//currentGroupSpeed = 0;
			currentPage = startMotor + 7;
			break;
		}
		}
		break;
	}
	case(startMotor + 7):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			motorStop(groups[currentGroupNum].motors[currentGroupIndex]);
			//currentGroupSpeed = 0;
			//currentPage = startMotor + 7;
			//stay on this page
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startMotor + 8;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor + 3;
			break;
		}
		}
		break;
	}
	case(startMotor + 8):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			motorSet(groups[currentGroupNum].motors[currentGroupIndex], -127);
			currentPage = startMotor + 7;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			motorSet(groups[currentGroupNum].motors[currentGroupIndex], 127);
			currentPage = startMotor + 7;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor + 7;
			break;
		}
		}
		break;
	}
	case(startMotor + 9):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			currentMotorNum--;
			if(currentMotorNum < 1)
			{
				currentMotorNum = 10;
			}
			//currentPage = startMotor + 9
			//stay on this page
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentMotorNum++;
			if(currentMotorNum > 10)
			{
				currentMotorNum = 1;
			}
			//currentPage = startMotor + 9
			//stay on this page
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentMotorSpeed = motorGet(currentMotorNum);
			currentPage = startMotor + 10;
			break;
		}
		}
		break;
	}
	case(startMotor + 10):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			motorStop(currentMotorNum);
			//currentMotorSpeed = 0;
			//currentPage = startMotor + 10;
			//stay on this page
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			currentPage = startMotor + 11;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor + 1;
			break;
		}
		}
		break;
	}
	case(startMotor + 11):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			//currentMotorSpeed = -127;
			motorSet(currentMotorNum, -127);
			currentPage = startMotor + 10;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			//currentMotorSpeed = 127;
			motorSet(currentMotorNum, 127);
			currentPage = startMotor + 10;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMotor + 10;
			break;
		}
		}
		break;
	}
	case(startAuton + 1):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			autonMode--;
			if(autonMode < 0)
				autonMode = 1;
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			autonMode++;
			if(autonMode > 1)
				autonMode = 0;
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startAuton;
			break;
		}
		}
		break;
	}
	case(startMode + 1):
	{
		if (input == LCD_BTN_LEFT || input == LCD_BTN_RIGHT) {
			if (opMode == 1) {
				imeReset(IME_RIGHT_CHAIN_NUMBER);
				imeReset(IME_LEFT_CHAIN_NUMBER);
			}
			else if (opMode == 2) {
				setShooter(0);
			}
		}

		switch(input)
		{
		case(LCD_BTN_LEFT):
		{
			opMode--;
			if (opMode == -1) {
				opMode = 2;
			}
			break;
		}
		case(LCD_BTN_RIGHT):
		{
			opMode++;
			if (opMode == 3) {
				opMode = 0;
			}
			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startMode;
			break;
		}
		}
		break;
	}
	case(startFPS + 1):
	{
		switch(input)
		{
		case(LCD_BTN_LEFT):
		{

			break;
		}
		case(LCD_BTN_RIGHT):
		{

			break;
		}
		case(LCD_BTN_CENTER):
		{
			currentPage = startFPS;
			break;
		}
		}
		break;
	}
	}
}

void handleLcdScreen()
{
	switch(currentPage)
	{
	case(startLink)://Link Status
	{
		updateLcdScreen("  Link Status", "<      --      >");
		break;
	}
	case(startBattery)://Battery Status
	{
		updateLcdScreen(" Battery Status", "<      --      >");
		break;
	}
	case(startSensor)://Sensor Status
	{
		updateLcdScreen(" Sensor Status", "<      --      >");
		break;
	}
	case(startMotor)://Motor Status
	{
		updateLcdScreen("  Motor Status", "<      --      >");
		break;
	}
	case(startAuton)://Auton Status
	{
		updateLcdScreen("  Auton Status", "<      --      >");
		break;
	}
	case(startMode)://OpMode status
	{
		updateLcdScreen(" OpMode Status", "<      --      >");
		break;
	}
	case(startFPS):
	{
		updateLcdScreen("   FPS Status", "<      --      >");
		break;
	}
	case(startLink + 1)://Link Status - Link Est.
	{
		updateLcdScreen("   Link Est.", "       Rtn");
		break;
	}
	case(startLink + 2)://Link Status - Link Not Est.
	{
		updateLcdScreen(" Link Not Est.", "       Rtn");
		break;
	}
	case(startBattery + 1)://Battery Status - Battery Selector
	{
		updateLcdScreen("Prims --- Backup", "---    Rtn   ---");
		break;
	}
	case(startBattery + 2)://Battery Status - Battery Selector - Prims Battery Status
	{
		lcdPrint(uart1, 1, "Main mVolt: Secn");
		lcdPrint(uart1, 2, "%u   Rtn  %d", powerLevelMain(), (int)((analogRead(BATTERY_SECOND_PORT) * 1000)/280.0f));//70.0f * 4.0f
		break;
	}
	case(startBattery + 3)://Battery Status - Battery Selector - Backup
	{
		lcdPrint(uart1, 1, "Back mVolt: %u", powerLevelBackup());
		lcdPrint(uart1, 2, "       Rtn");
		break;
	}
	case(startSensor + 1):
	{
		char* name = (void*)"";

		switch(sensIndex)
		{
		case(1):
		{
			name = (void*)"Quads";
			break;
		}
		case(2):
		{
			name = (void*)"IMEs";
			break;
		}
		case(3):
		{
			name = (void*)"Gyro";
			break;
		}
		case(4):
		{
			name = (void*)"S-R Info";
			break;
		}
		}

		lcdPrint(uart1, 1, " Group %s", name);
		lcdPrint(uart1, 2, "Last   --   Next");
		break;
	}
	case(startSensor + 2):
	{
		/*
		 * QUADRATURE: Rapid Updating
		 *
		 * 	Reads the data recieved from the Encoder.
		 * It then is able to handle any exceptions that
		 * are thrown by the Encoder. It prints the
		 * index of the Encoder (it's location) followed
		 * by the data that it is recieving. On the
		 * second line it prints buttons that are to
		 * iterate between the Encoders.
		 *
		 * *( See handleLcdInput() )*
		 *
		 */

		int quad = 0;
		switch(quadNum)
		{
		case(0):
		{
			quad = encoderGet(encoderBaseLeft);
			lcdPrint(uart1, 1, "Quad bL : %d", quad );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		case(1):
		{
			quad = encoderGet(encoderBaseRight);
			lcdPrint(uart1, 1, "Quad bR : %d", quad );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		case(2):
		{
			quad = encoderGet(encoderShooterLeft);
			lcdPrint(uart1, 1, "Quad sL : %d", quad );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		case(3):
		{
			quad = encoderGet(encoderShooterRight);
			lcdPrint(uart1, 1, "Quad sR : %d", quad );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		}
		break;
	}
	case(startSensor + 3):
	{
		/*
		 * IME: Rapid Updating
		 *
		 * 	Reads the data from the IME. It then is able to
		 * handle any exceptions thrown by the IME. It then
		 * prints the IME's index (location) and the data
		 * recieved by the IME. On the second line it prints
		 * the buttons needed to iterate between the IME's.
		 *
		 * *(See handleLcdUpdating() )*
		 *
		 */
		int ime;
		switch(imeNum){
		case(0):
		{
			ime = imeLMem;
			ime *= IME_LEFT_MULTIPLIER;
			lcdPrint(uart1, 1, "IME bL : %d", ime );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		case(1):
		{
			ime = imeRMem;
			ime *= IME_RIGHT_MULTIPLIER;
			lcdPrint(uart1, 1, "IME bR : %d", ime );
			lcdPrint(uart1, 2, "Prev  Rtn  Next");
			break;
		}
		}
		break;
	}
	case(startSensor + 4):
	{
		/*
		 * GYROSCOPE: Rapid Updating
		 *
		 * 	Reads the data recieved from the Gyroscope.
		 * It then is able to handle any exceptions that
		 * are thrown by the Gyroscope. It prints "Gyro"
		 * followed by the data that it is recieving. On the
		 * second line it prints the button to return back
		 * to the Sensor Selection.
		 *
		 * *( See handleLcdInput() )*
		 *
		 */
		int gy = gyroGet(gyroscope) % 360;
		lcdPrint(uart1, 1, "Gyroscope : %d", gy);
		lcdPrint(uart1, 2, "      Rtn");
		break;
	}
	case(startSensor + 5):
	{
		/*
		 * Shooting Information: Rapid Updating
		 *
		 * Reads the data from the rotor IME's and from the shooter shaft encoder.
		 * It then processes that information to give all the relavent information
		 * for shooting on one screen. Does not have room to display the return
		 * button text. Does not have any action for the left and right buttons.
		 * The center button returns to the startSensor page.
		 */
		int iL = imeLMem;
		int iR = imeRMem;

		iL *= IME_LEFT_MULTIPLIER;
		iR *= IME_RIGHT_MULTIPLIER;

		float sL = avgLeftShooterSpd * (50 / 6.0f) * 5.0f;//25.0f;
		float sR = avgRightShooterSpd * (50 / 6.0f) * 5.0f;//25.0f;

		lcdPrint(uart1, 1, "l:%5d, r:%5d", (int)sL, (int)sR);
		lcdPrint(uart1, 2, "L%6d  R%6d", iL, iR);

		break;
	}
	case(startMotor + 1):
	{
		updateLcdScreen("Grouping Select", "Group Rtn Indiv");
		break;
	}
	case(startMotor + 2):
	{
		int lastNum;
		int nextNum;
		if(currentGroupNum == 0)
		{
			lastNum = NUM_GROUPS - 1;
			nextNum = currentGroupNum + 1;
		}
		else if(currentGroupNum == NUM_GROUPS - 1)
		{
			lastNum = currentGroupNum - 1;
			nextNum = 0;
		}
		else
		{
			lastNum = currentGroupNum - 1;
			nextNum = currentGroupNum + 1;
		}
		lcdPrint(uart1, 1, "%d   Grp:%s  %d",
				lastNum,
				groups[currentGroupNum].groupName,
				nextNum);
		lcdPrint(uart1, 2, "Last   --   Next");
		break;
	}
	case(startMotor + 3):
	{
		switch(groups[currentGroupNum].validMotors)
		{
		case(1):
		{
			lcdPrint(uart1, 1, "%s:%d",
					groups[currentGroupNum].groupName,
					groups[currentGroupNum].motors[0]);
			break;
		}
		case(2):
		{
			lcdPrint(uart1, 1, "%s:%d,%d",
					groups[currentGroupNum].groupName,
					groups[currentGroupNum].motors[0],
					groups[currentGroupNum].motors[1]);
			break;
		}
		case(3):
		{
			lcdPrint(uart1, 1, "%s:%d,%d,%d",
					groups[currentGroupNum].groupName,
					groups[currentGroupNum].motors[0],
					groups[currentGroupNum].motors[1],
					groups[currentGroupNum].motors[2]);
			break;
		}
		case(4):
		{
			lcdPrint(uart1, 1, "%s:%d,%d,%d,%d",
					groups[currentGroupNum].groupName,
					groups[currentGroupNum].motors[0],
					groups[currentGroupNum].motors[1],
					groups[currentGroupNum].motors[2],
					groups[currentGroupNum].motors[3]);
			break;
		}
		case(5):
		{
			lcdPrint(uart1, 1, "%s:%d,%d,%d,%d,%d",
					groups[currentGroupNum].groupName,
					groups[currentGroupNum].motors[0],
					groups[currentGroupNum].motors[1],
					groups[currentGroupNum].motors[2],
					groups[currentGroupNum].motors[3],
					groups[currentGroupNum].motors[4]);
			break;
		}
		}
		lcdPrint(uart1, 2, "Speed Rtn  Indiv");
		break;
	}
	case(startMotor + 4):
	{
		lcdPrint(uart1, 1, "Grp Avg Spd:%d", currentGroupSpeed);
		lcdPrint(uart1, 2, "Stop   Rtn   Max");
		break;
	}
	case(startMotor + 5):
	{
		lcdPrint(uart1, 1, "Grp St Max:%s",
				groups[currentGroupNum].groupName);
		lcdPrint(uart1, 2, "-127 Grp-Spd 127");
		break;
	}
	case(startMotor + 6):
	{
		int lastNum;
		int nextNum;
		int temp = groups[currentGroupNum].validMotors;
		if (currentGroupIndex  == 0)
		{
			lastNum = groups[currentGroupNum].motors[temp - 1];
			nextNum = groups[currentGroupNum].motors[currentGroupIndex + 1];
		}
		else if (currentGroupIndex == temp - 1)
		{
			lastNum = groups[currentGroupNum].motors[currentGroupIndex - 1];
			nextNum = groups[currentGroupNum].motors[0];
		}
		else
		{
			lastNum = groups[currentGroupNum].motors[currentGroupIndex - 1];
			nextNum = groups[currentGroupNum].motors[currentGroupIndex + 1];
		}
		lcdPrint(uart1, 1, "%d Grp Mtr: %d %d",
				lastNum,
				groups[currentGroupNum].motors[currentGroupIndex],
				nextNum);
		lcdPrint(uart1, 2, "<      --      >");
		break;
	}
	case(startMotor + 7):
	{
		lcdPrint(uart1, 1, "Spd-GrIn-%s:%d",
				groups[currentGroupNum].groupName,
				currentGroupSpeed);
		lcdPrint(uart1, 2, "Stop Rtn-Grp Max");
		break;
	}
	case(startMotor + 8):
	{
		lcdPrint(uart1, 1, "Grp Ind Max:%d",
				groups[currentGroupNum].motors[currentGroupIndex]);
		lcdPrint(uart1, 2, "-127 Ind-Spd 127");
		break;
	}
	case(startMotor + 9):
	{
		int lastNum;
		int nextNum;
		if (currentMotorNum == 1)
		{
			lastNum = 10;
			nextNum = currentMotorNum + 1;
		}
		else if (currentMotorNum == 10)
		{
			lastNum = currentMotorNum - 1;
			nextNum = 1;
		}
		else
		{
			lastNum = currentMotorNum - 1;
			nextNum = currentMotorNum + 1;
		}
		lcdPrint(uart1, 1, "%d Ind Mtr: %d  %d",
				lastNum,
				currentMotorNum,
				nextNum);
		lcdPrint(uart1, 2, "<      --      >");
		break;
	}
	case(startMotor + 10):
	{
		lcdPrint(uart1, 1, "Spd-Ind-%d: %d",
				currentMotorNum,
				currentMotorSpeed);
		lcdPrint(uart1, 2, "Stop   Rtn   Max");
		break;
	}
	case(startMotor + 11):
	{
		lcdPrint(uart1, 1, "Indiv Max: %d",
				currentMotorNum);
		lcdPrint(uart1, 2, "-127  Speed  127");
		break;
	}
	case(startAuton + 1):
	{
		char* temp1 = "15S";
		char* temp2 = "1Min";

		char* s1 = (void*)"";
		char* s2 = (void*)"";

		if (autonMode == 0) {
			s1 = (void*)temp1;
			s2 = (void*)temp2;
		}
		else {
			s1 = (void*)temp2;
			s2 = (void*)temp1;
		}


		lcdPrint(uart1, 1, " AutonMode: %s", s1);
		lcdPrint(uart1, 2, "%s  Rtn %s", s2, s2);
		break;
	}
	case(startMode + 1):
	{
		char* temp1 = "comp";
		char* temp2 = "rotor";
		char* temp3 = "shoot";//max 6 characters

		char* s1 = "";
		char* s2 = "";
		char* s3 = "";

		if (opMode == 0)
		{
			s1 = (void*)temp1;
			s2 = (void*)temp2;
			s3 = (void*)temp3;
		}
		else if (opMode == 1)
		{
			s1 = (void*)temp2;
			s2 = (void*)temp3;
			s3 = (void*)temp1;
		}
		else if (opMode == 2)
		{
			s1 = (void*)temp3;
			s2 = (void*)temp1;
			s3 = (void*)temp2;
		}

		lcdPrint(uart1, 1, "OpMode: %s", s1);
		lcdPrint(uart1, 2, "%s Rtn %s", s3, s2);
		break;
	}
	case(startFPS + 1):
	{
		lcdPrint(uart1, 1, "x:%5d, y:%5d", position.x, position.y);
		lcdPrint(uart1, 2, "g:%4d, a:%d",
				//getLocalAngle(gyroscope), getGlobalAngle(FPSBase.correction, gyroscope));
				getGlobalAngle(FPSBase.correction, gyroscope), FPSBase.axis);
		break;
	}
	}
}

void lcdTask(void* ignore) {

	int input;

	while(true) {

		//printf("In LCDTask\n\r");

		/*
		printf("x:%5d, y:%5d", position.x, position.y);
		printf("gL:%4d, gG:%4d",
				FPSBase.correction.localAngle, FPSBase.correction.globalAngle);
		*/
		//int asdf = (int)(gyroscope*);
		//printf("gyro O: %4d \n\t", gyroGet(gyroscope));
		//printf("gyro D: %4d \n\r", asdf);
		//printf("axis:%d\n\r", FPSBase.axis);

		input = lcdReadButtons(uart1);

		handleLcdUpdating(input);

		wait(TIME_DELAY);
	}
}
