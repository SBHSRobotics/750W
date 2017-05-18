#include "FPS.h"

/** integer adjustment for the x coordinate of the robot so that position is recorded from the center of the robot, not the sides */
int toCenterX=9;

/** integer adjustment for the y coordinate of the robot so that position is recorded from the center of the robot, not the sides */
int toCenterY=9;

int getFPSGlobalAngle(Correction correction, Gyro gyro){
	int thetaAddition = 0;

	int thetaTemp = (int)(atan(((float)FPSBase.direction.chY)/FPSBase.direction.chX) * TO_DEGREE);
	
	//checking against zero has to be added because this version of atan does not properly
	//interpret x = 0, y != 0 as 90 or -90 but instead as 89 and -89
	if (FPSBase.direction.chX == 0)
	{
		thetaAddition = FPSBase.direction.chY > 0 ? 90 : 
						FPSBase.direction.chY < 0 ? 270 : 0;
	}
	else
	{
		if (FPSBase.direction.chX >= 0)
		{
			thetaAddition = thetaTemp + 360;
		}
		else 
		{
			thetaAddition = thetaTemp + 180;
		}
	}

	/*printf("2[chX:%4d, chY:%4d]\n\r", FPSBase.direction.chX, FPSBase.direction.chY);
	printf("angle : %d\n\r", (thetaAddition % 360));
	printf("     +: %d\n\r", getGlobalAngle(correction, gyroscope) % 360);
	printf("angle+: %d\n\r", (thetaAddition + getGlobalAngle(correction, gyro)) % 360);*/

	return (thetaAddition + getGlobalAngle(correction, gyro)) % 360;
}

int encoderValue(){

	/** The value of the left base encoder */
	int l=encoderGet(encoderBaseLeft);

	/** The value of the right base encoder */
	int r=encoderGet(encoderBaseRight);

	/** The weighted average value of the base encoders */

	float lw = 1.0f;//left weighting
	float rw = 1.0f;//right weighting
	float tot = 1.0f;//totat weighting

	int chP = FPSBase.direction.chY;
	int chS = FPSBase.direction.chX;

	if (abs(chP) > 0 || abs(chS) > 0)
	{
		if (sgnSimple(chS) == sgnSimple(chP))
		{
			lw = sgnSimple(chP) * sqrt((float)(chS * chS + chP * chP));
			rw = chP - chS;
			//lw = sqrt((float)(chS * chS + chP * chP));
			//rw = abs(chP - chS);
		}
		else
		{
			lw = chP + chS;
			rw = sgnSimple(chP) * sqrt((float)(chS * chS + chP * chP));
			//lw = abs(chP + chS);
			//rw = sqrt((float)(chS * chS + chP * chP));
			
		}

		lw = abs(lw);
		rw = abs(rw);
		tot = lw + rw;
		//tot = lw + rw;
	}

	lw /= tot;
	rw /= tot;

	printf("l:%5d, r:%5d", l, r);
	printf("lw:%4f, rw:%4f", lw, rw);
	printf("l*:%4f, r*:%4f", (l * lw), (r * rw));

	//return (l+r)/2;
	return (int)(l * lw + r * rw);
}

int disMoved(int m){

	/** converts encoder values to Inches */
	int dist=(m/360.0)*3.25*M_PI*(1/sqrt(2));
	return dist;
}

int baseAngle(Base old){

	/** returns the angle of the robot with respect to the field; the side of the field with the red tile is the +x direction and the side of the field with the nets is in the +y direction; the center of the field is the origin */
	return getFPSGlobalAngle(old.correction,gyroscope);
}

int getSonarLeft()
{

	/** gets the value of the left ultrasonic sensor */
	return ultrasonicGet(ultraLeft) * TO_INCHES + toCenterX;
}

int getSonarBack()
{

	/** gets the value of the back ultrasonic sensor */
	return ultrasonicGet(ultraBack) * TO_INCHES + toCenterY;
}

int getSonarRight()
{

	/** gets the value of the right ultrasonic sensor */
	return ultrasonicGet(ultraRight) * TO_INCHES - toCenterX;
}

bool leftSide(){

	/** checks which side the robot is starting on */
	if (getSonarLeft()<72){

		return true;
	}
	else{

		return false;
	}
}

int determineXInit(bool lside)
{

	int x=0;

	/** converts the starting position ultrasonic x direction values to a coordinate on the field */
	if (lside){

		x=getSonarRight()-72;
	}
	else{

		x=72+getSonarRight();
	}

	return x;
}

int determineYInit()
{

	/** converts the starting position ultrasonic y direction values to a coordinate on the field */
	int y;
	y=getSonarBack()-72;

	return y;
}

Location netPos(bool lside, int x, int y){
	Location goal;

	/** sets the coordinates of the desired goal based on which team we are on (red or blue) */
	if (lside){

		goal=(Location){x,y};
	}
	else{

		goal=(Location){-x,y};
	}

	return goal;
}

int distance(Location robotpos, Location goal){
	int dist=0;

	/** uses the distance formula with the coordinates of the goal and the robot to determine the distance from the robot to the goal */
	dist=sqrt(((goal.x-robotpos.x)*(goal.x-robotpos.x))+((goal.y-robotpos.y)*(goal.y-robotpos.y)));

	return dist;
}
int shooterAngle_Xaxis(Location robotpos, int distance, Location net){

	int theta=0;

	/** calculates the angle between the net and the robot and the goal to determine what angle the robot needs to be on with respect to the field */
	theta=acos((net.x-robotpos.x)/distance);

	return theta;
}

/*Base* setBase(Base* base)
{
	return base;
}*/
int phi(int distance){
	int angle=0;
	switch(distance){

	case(6) : break;

	case(12) : break;

	case(18) : break;

	case(24) : break;

	case(30) : break;

	case(36) : break;

	case(42) : break;

	case(48) : break;

	case(54) : break;

	case(60) : break;

	case(66) : break;

	case(72) : break;

	case(78) : break;

	case(84) : break;

	case(90) : break;

	case(96) : break;

	case(102) : break;

	case(108) : break;

	case(114) : break;

	case(120) : break;

	case(126) : break;

	case(132) : break;

	case(138) : break;

	case(144) : break;
	}
	return angle;
}

Base FPSBase;
Location position;

void FPSTask(void* ignore)
{

	/** x coordinate of the center of the goal */
	int netx=72;

	/** y coordinate of the center of the goal */
	int nety=72;

	/** checks what side we are starting on */
	bool lside=leftSide();

	/** sets the starting position of the robot*/
	position=(Location){0,0};//determineXInit(lside), determineYInit()};

	/** sets the starting position of the net*/
	Location net=netPos(lside, netx, nety);

	/** variable that increments whenever we move a linear distance */
	int movement=0;

	/** variable that keeps track of what the encoder value was a certain number of iterations prior */
	int previousEncoder=0;

	/** variable that sets the minimum amount the encoder value needs to change for us to count it as a definite movement */
	int thresEncoder=5;

	/** variable that sets the minimum amount the gyroscope value needs to change for us to count it as a definite turn */
	int thresGyro=5;

	/** variable that keeps track of what the gyroscope value was a certain number of iterations prior */
	int previousGyro=0;

	/** variable that keeps track of how many times the main code has looped */
	int iteration=0;

	/** variable for the angle between the robot and the net */
	int angle=0;

	/** variable that sets the number of iterations we let pass before checking for movement */
	int refreshrate=10;

	while(1)
	{

		//printf("In FPSTask\n\r");
		//FPSBase.correction.localAngle = getLocalAngle(gyroscope);

		/** increments the iteration variable */
		iteration++;

		/** checks if a certain number of iterations has passed to check for a change */
		if (iteration%refreshrate==0){

			printf("FPS 1\n\r");
			printf("1[chX:%4d, chY:%4d]\n\r", FPSBase.direction.chX, FPSBase.direction.chY);
			//printf("axisTurn: %d\n\r", (int)axisTurn);

			/** checks if we have not turned and/or are not turning since the last check for movement */
			//if (abs(baseAngle(FPSBase)-previousGyro)<=thresGyro){
			if (FPSBase.axis != axisTurn){

				/** checks if we have moved a linear distance since the last check for movement */
				//if (abs(encoderValue()-previousEncoder)>thresEncoder){
				//if (FPSBase.axis != none){
				if (true){

					/** increments the movement value by the difference between the current encoder value and the encoder value from the previous check for movement */
					movement+=encoderValue()-previousEncoder;

					/** changes the x coordinate of the robot by the movement in the x direction */
					position.x+=disMoved(movement)*cos(baseAngle(FPSBase));

					/** changes the y coordinate of the robot by the movement in the y direction */
					position.y+=disMoved(movement)*sin(baseAngle(FPSBase));

					/** sets the current value of the encoder to the previous encoder value*/
					previousEncoder=encoderValue();
					printf("encoders:[left:%6d, right:%6d]\n\r", encoderGet(encoderBaseLeft), encoderGet(encoderBaseRight));
					printf("2[chX:%4d, chY:%4d]\n\r", FPSBase.direction.chX, FPSBase.direction.chY);
					printf("movement:%6d, X:%4d, Y:%4d\n\r", movement, position.x, position.y);
				}
			}
			else{

				/** sets the change in linear movement to 9 */
				movement=0;
				previousEncoder=encoderValue();
			}

			/** sets the current value of the gyroscope to the previous gyroscope value*/
			previousGyro=baseAngle(FPSBase);
		}

		/** sets the angle variable to the angle between the current position of the robot and the net */
		angle=shooterAngle_Xaxis(position, distance(position, net), net);

		/** checks if the 7L or 7R buttons on the controller have been pressed; one or the other, not both */
		if ((joystickGetDigital(1,7,JOY_LEFT)) ^ (joystickGetDigital(1,7,JOY_RIGHT))){

			/** if the 7L button has been pressed, the position is reset by the ultrasonic sensors (left and back) to minimize error */
			if ((joystickGetDigital(1,7,JOY_LEFT))){

				position.x=getSonarLeft();
				position.y=getSonarBack();
			}

			/** if the 7R button has been pressed, the position is reset by the ultrasonic sensors (right and back) to minimize error */
			else{

				position.x=getSonarRight();
				position.y=getSonarBack();
			}
		}


		printf("\n\r");
		wait(TIME_DELAY);
	}

}
