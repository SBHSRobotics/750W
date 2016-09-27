#include "base.h"

/*
 * Used to adjust the input from a controllers joystick towards 127.
 */
int adjustInput(int in);

/*
 * Used to control the setting of motor values of the base if the base is an x-drive
 * or holonomic drive
 */
Base motorSetQuad(Base old, Base now);

/*
 * Used to control the setting of motor values of the base if the base is a tank drive
 */
Base motorSetNorm(Base old, Base now);

/*
 * Used to adjust the speed depending on its change from one Axis to another Axis as well as
 * facilitate the speed change if the base is point turning
 */
Base adjustedBaseSet(Base old, Base now);



int adjustInput(int in){
	in = (int)(-127.0f + 254.0f / (1.0f + exp(-0.025f * in)));
	in = abs(in) > MAX_SPEED ? sgnSimple(in) * MAX_SPEED : in;
	return in;
}
/*now.left = (int)(-MAX_SPEED.0f + 254.0f / (1.0f + exp(-0.025f * now.left)));
	now.right = (int)(-MAX_SPEED.0f + 254.0f / (1.0f + exp(-0.025f * now.right)));
	now = (Base){	(abs(now.left) > MAX_SPEED ? sgnSimple(now.left) * MAX_SPEED : now.left),
					(abs(now.right) > MAX_SPEED ? sgnSimple(now.right) * MAX_SPEED : now.right),
					now.correction, now.axis};*/

Base motorSetQuad(Base old, Base now)
{
	//if (millis() - time < timeDelay)
	//	return old;
	currentTime = millis();
	old = (Base){	(abs(old.left) > MAX_SPEED ? sgnSimple(old.left) * MAX_SPEED : abs(old.left) < INPUT_THRESHOLD ? 0 : old.left),
					(abs(old.right) > MAX_SPEED ? sgnSimple(old.right) * MAX_SPEED : abs(old.right) < INPUT_THRESHOLD ? 0 : old.right),
					old.correction, old.axis, old.direction};
	now = (Base){	(abs(now.left) > MAX_SPEED ? sgnSimple(now.left) * MAX_SPEED : abs(now.left) < INPUT_THRESHOLD ? 0 : now.left),
					(abs(now.right) > MAX_SPEED ? sgnSimple(now.right) * MAX_SPEED : abs(now.right) < INPUT_THRESHOLD ? 0 : now.right),
					now.correction, now.axis, now.direction};
	if ((now.left == now.right) == 0)
		now.axis = none;
	bool change = true;
	if (sgn(now.left) == sgn(old.left) && sgn(now.right) == sgn(old.right))
	{
		if (abs(now.left - old.left) <= BASE_DIV && abs(now.right - old.right) <= BASE_DIV)
			change = false;
	}

	//printf("hereFQuad %d, %d\n\r", now.left, now.right);
	if (change)
	{
		now = adjustedBaseSet(old, now);
		now.left = abs(now.left) > MAX_SPEED ? sgnSimple(now.left) * MAX_SPEED :
				abs(now.left) < INPUT_THRESHOLD ? 0 : now.left;
		now.right = abs(now.right) > MAX_SPEED ? sgnSimple(now.right) * MAX_SPEED :
				abs(now.right) < INPUT_THRESHOLD ? 0 : now.right;
		motorSet(MOTOR_BASE_TOP_LEFT_PORT, MOTOR_BASE_TOP_LEFT_MULTIPLIER * now.left);
		motorSet(MOTOR_BASE_TOP_RIGHT_PORT, MOTOR_BASE_TOP_RIGHT_MULTIPLIER * now.right);
		motorSet(MOTOR_BASE_BOT_LEFT_PORT, MOTOR_BASE_BOT_LEFT_MULTIPLIER * now.right);
		motorSet(MOTOR_BASE_BOT_RIGHT_PORT, MOTOR_BASE_BOT_RIGHT_MULTIPLIER * now.left);
		return now;
	}
	return old;
}

Base motorSetNorm(Base old, Base now)
{
	//if (millis() - time < timeDelay)
	//	return old;
	currentTime = millis();
	old = (Base){	(abs(old.left) > MAX_SPEED ? sgnSimple(old.left) * MAX_SPEED : abs(old.left) < INPUT_THRESHOLD ? 0 : old.left),
					(abs(old.right) > MAX_SPEED ? sgnSimple(old.right) * MAX_SPEED : abs(old.right) < INPUT_THRESHOLD ? 0 : old.right),
					old.correction, old.axis, old.direction};
	now = (Base){	(abs(now.left) > MAX_SPEED ? sgnSimple(now.left) * MAX_SPEED : abs(now.left) < INPUT_THRESHOLD ? 0 : now.left),
					(abs(now.right) > MAX_SPEED ? sgnSimple(now.right) * MAX_SPEED : abs(now.right) < INPUT_THRESHOLD ? 0 : now.right),
					now.correction, now.axis, now.direction};
	bool change = true;
	if (sgn(now.left) == sgn(old.left) && sgn(now.right) == sgn(old.right))
	{
		if (abs(now.left - old.left) <= BASE_DIV && abs(now.right - old.right) <= BASE_DIV)
			change = false;
	}

	//printf("hereFNorm %d, %d\n\r", now.left, now.right);
	if (change)
	{
		now = adjustedBaseSet(old, now);
		now.left = abs(now.left) > MAX_SPEED ? sgnSimple(now.left) * MAX_SPEED :
				abs(now.left) < INPUT_THRESHOLD ? 0 : now.left;
		now.right = abs(now.right) > MAX_SPEED ? sgnSimple(now.right) * MAX_SPEED :
				abs(now.right) < INPUT_THRESHOLD ? 0 : now.right;
		motorSet(MOTOR_BASE_TOP_LEFT_PORT, MOTOR_BASE_TOP_LEFT_MULTIPLIER * now.left);
		motorSet(MOTOR_BASE_TOP_RIGHT_PORT, MOTOR_BASE_TOP_RIGHT_MULTIPLIER * now.right);
		motorSet(MOTOR_BASE_BOT_LEFT_PORT, MOTOR_BASE_BOT_LEFT_MULTIPLIER * now.left);
		motorSet(MOTOR_BASE_BOT_RIGHT_PORT, MOTOR_BASE_BOT_RIGHT_MULTIPLIER * now.right);
		return now;
	}
	return old;
}

Base adjustedBaseSet(Base old, Base now)
{
	if(now.axis != axisTurn)
	{
		if (abs(now.left) > INPUT_THRESHOLD && (sgn(now.left) == sgn(old.left)))
		{
			int sgnL = sgn(now.left - old.left);
			now.left = 	sgnL == 1 	? 	(old.left + BASE_LINEAR_SPEED_INCREASE_MAXIMUM  > MAX_SPEED		? MAX_SPEED 	: old.left + BASE_LINEAR_SPEED_INCREASE_MAXIMUM) :
						sgnL == -1 	? 	(old.left - BASE_LINEAR_SPEED_INCREASE_MAXIMUM  < -MAX_SPEED 	? -MAX_SPEED	: old.left - BASE_LINEAR_SPEED_INCREASE_MAXIMUM) :
						old.left;
		}
		if (abs(now.right) > INPUT_THRESHOLD && (sgn(now.right) == sgn(old.right)))
		{
			int sgnR = sgn(now.right - old.right);
			now.right = sgnR == 1 	? 	(old.right + BASE_LINEAR_SPEED_INCREASE_MAXIMUM > MAX_SPEED 	? MAX_SPEED 	: old.right + BASE_LINEAR_SPEED_INCREASE_MAXIMUM) :
						sgnR == -1 	? 	(old.right - BASE_LINEAR_SPEED_INCREASE_MAXIMUM < -MAX_SPEED	? -MAX_SPEED	: old.right - BASE_LINEAR_SPEED_INCREASE_MAXIMUM) :
						old.right;
		}
	}
	else {
		//change this so that it now speed changes based directly on old speed
		//should not be linear but rather a (1/x) from 0 to MAX_SPEED on x axis
		//flipped over 64 and instead going from 0 to MAX_SPEED on y axis
		if (abs(now.left) > INPUT_THRESHOLD && (sgn(now.left) == sgn(old.left)))
		{
			int sgnL = sgn(now.left - old.left);
			now.left = 	sgnL == 1 	? 	(old.left + BASE_TURN_SPEED_INCREASE_MAXIMUM  > MAX_SPEED	? MAX_SPEED 	: old.left + BASE_TURN_SPEED_INCREASE_MAXIMUM / 2) :
						sgnL == -1 	? 	(old.left - BASE_TURN_SPEED_INCREASE_MAXIMUM  < -MAX_SPEED  ? -MAX_SPEED 	: old.left - BASE_TURN_SPEED_INCREASE_MAXIMUM / 2) :
						old.left;
		}
		if (abs(now.right) > INPUT_THRESHOLD && (sgn(now.right) == sgn(old.right)))
		{
			int sgnR = sgn(now.right - old.right);
			now.right = sgnR == 1 	? 	(old.right + BASE_TURN_SPEED_INCREASE_MAXIMUM > MAX_SPEED 	? MAX_SPEED 	: old.right + BASE_TURN_SPEED_INCREASE_MAXIMUM / 2) :
						sgnR == -1 	? 	(old.right - BASE_TURN_SPEED_INCREASE_MAXIMUM < -MAX_SPEED	? -MAX_SPEED 	: old.right - BASE_TURN_SPEED_INCREASE_MAXIMUM / 2) :
						old.right;
		}
	}
	return now;
}

Base baseStop(Base old)
{
	old.left = 0;
	old.right = 0;
	old.axis = none;
	motorSet(MOTOR_BASE_TOP_LEFT_PORT, 0);
	motorSet(MOTOR_BASE_TOP_RIGHT_PORT, 0);
	motorSet(MOTOR_BASE_BOT_LEFT_PORT, 0);
	motorSet(MOTOR_BASE_BOT_RIGHT_PORT, 0);
	return old;
}

Base baseSetQuad(Base old, int chS, int chP,
		bool leftFast, bool leftSlow, bool rightFast, bool rightSlow,
		int turnSpeed)
{
	//printf("%d, %d\n\r", ch1, ch2);
	//gyroscope = (Gyro)(old.offset);
	//gyroReset(gyroscope);
	Base now = (Base){0, 0, (Correction){0, 0}, 0, (Direction){0, 0}};

	chS = adjustInput(chS);
	chP = adjustInput(chP);

	//printf("5U : %d, 5D : %d, 6U : %d, 6D : %d\n\r", ch5U, ch5D, ch6U, rightSlow);

	int dif = 0;
	int sgnCh = 0;
	int sgnOffset = 0;

	int leftMulti = 0;

	int angle = 0;

	bool diag = true;

	int halfTurnSpeed = turnSpeed / 2;

	/*
	if ((abs(ch1) <= INPUT_THRESHOLD && abs(ch2) <= INPUT_THRESHOLD) || ch5 || ch6)
	{
		now.offset = 0;
		gyroReset(gyroscope);
	}*/

	//("ch1 : %d, ch2 : %d, ch5 : %d, ch6 : %d\n\r", ch1, ch2, ch5, ch6);

	if ((leftFast || leftSlow) ^ (rightFast || rightSlow))
	{
		diag = false;
		now.axis = axisTurn;
		if (leftFast || leftSlow)
		{
			if (leftSlow)
			{
				now.left = -halfTurnSpeed;
				now.right = halfTurnSpeed;
			}
			else// if(ch5U)
			{
				now.left = -turnSpeed;
				now.right = turnSpeed;
			}
		}
		else
		{
			if (rightSlow)
			{
				now.left = halfTurnSpeed;
				now.right = -halfTurnSpeed;
			}
			else// if (rightFast)
			{
				now.left = turnSpeed;
				now.right = -turnSpeed;
			}
		}
	}

	/*if ((ch5U ^ ch6U) ^ (ch5D ^ ch6D))
	{
		diag = false;
		now.axis = axisTurn;
		if (ch5U ^ ch6U)
		{
			if (ch5U)
			{
				now.left = -turnSpeed;
				now.right = turnSpeed;
			}
			else
			{
				now.left = turnSpeed;
				now.right = -turnSpeed;
			}
		}
		else
		{
			if (ch5D)
			{
				now.left = -halfTurnSpeed;
				now.right = halfTurnSpeed;
			}
			else
			{
				now.left = halfTurnSpeed;
				now.right = -halfTurnSpeed;
			}
		}
	}*/
	/*
	if (ch5 ^ ch6)
	{
		diag = false;
		now.axis = axisTurn;
		if (ch5)
		{
			//("left\n\r");
			now.left = -turnSpeed;
			now.right = turnSpeed;
		}
		else
		{
			//("right\n\r");
			now.left = turnSpeed;
			now.right = -turnSpeed;
		}
	}*/
	else if ((abs(chS) > INPUT_THRESHOLD) || (abs(chP) > INPUT_THRESHOLD))
	{
		if ((abs(chS) > INPUT_THRESHOLD) ^ (abs(chP) > INPUT_THRESHOLD))
		{
			if (abs(chS) > INPUT_THRESHOLD)
			{
				//("horizontal\n\r");
				now.axis = axisX;
				now.right = -(now.left = chS);
				sgnCh = sgnSimple(chS);
				leftMulti = -1;
			}
			else
			{
				now.axis = axisY;
				now.right = now.left = chP;
				sgnCh = sgnSimple(chP);
				leftMulti = 1;
				//printf("here1 %d, %d\n\r", now.left, now.right);
				//("verticle %d %d\n\r", now.left, now.right);
			}
			//gyroscope = (old.offset == now.offset ? gyroscope : (Gyro)0);
			if (old.axis != now.axis) now.correction = resetLocalAngle(now.correction, gyroscope);
			else now.correction.localAngle = getLocalAngle(gyroscope);
			angle = now.correction.localAngle;
			//printf("here2 %d, %d\n\r", now.left, now.right);
			/*if (old.axis != now.axis) now.correction = resetLocalAngle(now.correction, encoderLeft, encoderRight);
			else now.correction.localAngle = getLocalAngle(encoderLeft, encoderRight);
			angle = now.correction.localAngle - (leftMulti == 1 ? (ch2 > 0 ? 0 : 180 * sgnSimple(now.correction.localAngle)) : (ch1 > 0 ? -90 : 90));
			angle = 0;/////////////////testing*/
			//printf("angle:%d\n\r", angle);
			if (abs(angle) > BASE_DIV)
			{
				dif = (int)((float)((abs(angle)))/BASE_DIV + 0.5f);
				sgnOffset = sgnSimple(angle);

				now.right = leftMulti * (now.left -= (sgnCh * dif));
				now.right -= (sgnCh * sgnOffset * dif);
				now.left += leftMulti * (sgnCh * sgnOffset * dif);
			}
			//printf("here3 %d, %d\n\r", now.left, now.right);
		}
		else
		{
			//("both\n\r");
			now.axis = axisBoth;
			//now.axis = none;

			//int targetAngle = 0;

			if (sgnSimple(chS) == sgnSimple(chP))
			{
				now.left = sgnSimple(chP) * (int)sqrt((float)(chS * chS + chP * chP));
				now.right = chP - chS;/*
				sgnCh = sgnSimple(now.right > 0 ? ch2 : now.right < 0 ? ch1 : 1);

				targetAngle = getAngle(now.left, now.right);
				now.correction.localAngle = getLocalAngle(gyroscope) - targetAngle;
				angle = now.correction.localAngle;
				if(abs(angle) > div)
				{
					dif = (int)((float)((abs(angle)))/div + 0.5f);
					sgnOffset = sgnSimple(angle);

					int sgnCh2 = sgnSimple(ch2);

					float substractionRatio = ((float)(now.right)) / now.left;
					int sgnSubRatio = sgnSimple(substractionRatio);

					now.right -= sgnCh2 * sgnSubRatio * sgnCh * dif;
					now.left -= (int)(sgnCh2 * substractionRatio * sgnSubRatio * sgnCh * dif);
					now.right -= (int)(sgnCh2 * substractionRatio * sgnSubRatio * sgnCh * sgnOffset * dif);
					now.left += (int)(sgnCh2 * substractionRatio * sgnCh * sgnOffset * dif);
				}*/
			}
			else
			{
				now.left = chP + chS;
				now.right = sgnSimple(chP) * (int)sqrt((float)(chS * chS + chP * chP));
				//targetAngle = getAngle(now.left, now.right);
			}
			/*if (old.axis != now.axis) now.correction = resetLocalAngle(now.correction, gyroscope);
			else now.correction.localAngle = getLocalAngle(gyroscope);
			angle = abs(targetAngle) - abs(now.correction.localAngle);
			if (abs(angle) > div)
			{
				dif = (int)((float)((abs(angle))) / div + 0.5f);

			}*/
		}
	}
	else
	{
		//("nothing\n\r");
		now.axis = none;
		now.left = 0;
		now.right = 0;
	}

	now.direction.chX = abs(chS) > INPUT_THRESHOLD ? chS : 0;
	now.direction.chY = abs(chP) > INPUT_THRESHOLD ? chP : 0;

	//printf("here4 %d, %d\n\r", now.left, now.right);
	//("%d %d %d\n\r", now.left, now.right, powerLevelMain());
	//("%d %d\n\r", -now.correction.localAngle, -now.correction.globalAngle);
	return (diag ? motorSetQuad(old, now) : motorSetNorm(old, now));
}


/***********************************************************************************************************************
 * This method or motorSetNorm or some connection to it has a bug in it.
 * This bug is that either the left or right side oscilates between the value it should be at and maybe towards either
 * 0 or its negative value. Cannot tell, not enough testing but can be circumvented if you use the baseSetQuad method
 * instead.
 *
 **********************************************************************************************************************/
Base baseSetNorm(Base old, int chP, bool left, bool right, int turnSpeed)
{
	//gyroscope = (Gyro)(old.offset);
	//gyroReset(gyroscope);
	Base now = (Base){0, 0, old.correction, 0, (Direction){0, 0}};

	chP = adjustInput(chP);

	int dif = 0;
	int sgnCh = 0;
	int sgnOffset = 0;

	int angle = 0;

	/*if (abs(ch2) <= INPUT_THRESHOLD || ch5 || ch6)
	{
		now.offset = 0;
		gyroReset(gyroscope);
	}*/

	if (left ^ right)
	{
		now.axis = axisTurn;
		if(left)
		{
			now.left = -turnSpeed;
			now.right = turnSpeed;
		}
		else
		{
			now.left = turnSpeed;
			now.right = -turnSpeed;
		}
	}
	else if (abs(chP) > INPUT_THRESHOLD)
	{
		now.axis = axisY;
		now.left = now.right = chP;
		sgnCh = sgnSimple(chP);

		if (old.axis != now.axis) now.correction = resetLocalAngle(now.correction, gyroscope);
		else now.correction.localAngle = getLocalAngle(gyroscope);//-now.correction.localAngle = getLocalAngle(gyroscope);
		angle = now.correction.localAngle;

		/*if (old.axis != now.axis) now.correction = resetLocalAngle(now.correction, encoderLeft, encoderRight);
		else -now.correction.localAngle = getLocalAngle(encoderLeft, encoderRight);
		angle = -now.correction.localAngle - (ch2 > 0 ? 0 : 180 * sgnSimple(-now.correction.localAngle));
		*/
		if (abs(angle) > BASE_DIV)
		{
			dif = (int)((float)((abs(angle)))/BASE_DIV + 0.5f);
			sgnOffset = sgnSimple(angle);

			now.left = now.right -= (sgnCh * dif);//if counter clock wise
			now.left += (sgnCh * sgnOffset * dif);
			now.right -= (sgnCh * sgnOffset * dif);
		}
	}
	else
	{
		now.axis = none;
		now.left = 0;
		now.right = 0;
	}
	return motorSetNorm(old, now);
}

void baseSpeed(Base old)
{
	//TODO: Implement baseSpeed
}