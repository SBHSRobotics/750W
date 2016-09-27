#include "autonomous_utility_methods.h"

/*
 *	check if -1 or -2
 * 		if -1 then turning left
 * 		if -2 then turning right
 *	otherwise from 0 to 359 then go to that angle
 *	beyond 359 will be modulated with 360
 *		x %= 360
 *
 *	if turning then turn to that rotation and slow down as u get to the correct degree.
 *	don't stop correcting once you get to the correct degree.
 *
 *	if moving then forward along that angle. slow down as u approach destination.
 *	Don't stop correcting once you go past the correct position.
 *
 *	if turning then mobility is angle
 *	if moving then mobility is tiles
 */
/*void baseController(Base *old, int speed, float mobility, int direction) {
	const float multi1_2 = 1.0f/2.0f;
	const float multi3_4 = 3.0f/4.0f;
	const float multi1_3 = 1.0f/3.0f;
	const float multi2_3 = 2.0f/3.0f;

	//Base now = {0, 0, (Correction){0, 0}, 0};

	if (direction < 0) {
		if (direction < -2)
			return;

		bool left = 0, right = 0;
		//stuff
		while (true) {
			old->correction.localAngle = 0;
			//TODO: use the proper angleGet method once implemented
			//make sure that function returns something modulus 360

			if (old->correction.localAngle < mobility) {//check
				left = true;
				right = false;
			}
			else {
				left = false;
				right = true;
			}

			if (abs(old->correction.localAngle - mobility) >= 2)
				*old = baseSetQuad(*old, 0, 0, false, left, false, right,
						(
							abs(old->correction.localAngle) < mobility * multi1_3 ? 	speed :
							abs(old->correction.localAngle) < mobility * multi3_4 ? 	speed * multi2_3 :
							speed * multi1_2
						)
					);
		}
	}
	else {
		direction %= 360;

		int chP = 0, chS = 0;//chP = y-axis, chS = x-Axis

		switch(direction)
		{
		case(0):
		{
			chP = speed;
			chS = 0;
			break;
		}
		case(90):
		{
			chP = 0;
			chS = -speed;
			break;
		}
		case(180):
		{
			chP = -speed;
			chS = 0;
			break;
		}
		case(270):
		{
			chP = 0;
			chS = speed;
			break;
		}
		default: {
			//TODO: find the motor speed for different angles.
		}
		}
		//stuff
		mobility = sgnSimple(mobility) * mobility;
		float distance = mobility * TILE_RATIO_ADJUSTED / WHEEL_CIRCUMFERENCE;
		float rotation = 0;

		float absRot = 0, absDis = 0;

		encoderReset(encoderBaseLeft);
		encoderReset(encoderBaseRight);

		while(true) {
			rotation = avg(encoderGet(encoderBaseLeft), encoderGet(encoderBaseRight));
			//TODO: see if there is a different way to get rotation for auton

			absRot = abs(rotation);
			absDis = abs(distance);

			if (abs(rotation - distance) < 0.5f * TILE_RATIO_ADJUSTED / WHEEL_CIRCUMFERENCE)
				*old = (absRot < absDis * multi1_3 ?	baseSetQuad(*old, chP, chS,
															false, false, false, false, 0) :
						absRot < absDis * multi3_4 ?	baseSetQuad(*old, chP * multi2_3, chS * multi2_3,
															false, false, false, false, 0) :
														baseSetQuad(*old, chP * multi1_2, chS * multi1_2,
															false, false, false, false, 0)
					);
		}
	}
}*/


Base movementBase(Base old, int speed, float tiles, int direction)
{
	if(direction % 90 != 0)
		return old;

	int chS = 0;
	int chP = 0;

	switch(direction)
	{
		case(0):
			{
				chS = 0;
				chP = 1;
				break;
			}
		case (90) :
			{
				chS = -1;
				chP = 0;
				break;
			}
		case (-90) :
			{
				chS = 1;
				chP = 0;
				break;
			}
		//case (180) :
		//case (-180) :
		default:
			{
				chS = 0;
				chP = -1;
				break;
			}
	}

	Base now = (Base){0, 0, old.correction, 0};

	chS *= speed;//abs(speed);
	chP *= speed;//abs(speed);
	tiles = (tiles >= 0 ? tiles : -tiles);

	int rotation = 0;

	resetLocalAngle(now.correction, gyroscope);
	encoderReset(encoderBaseLeft);
	encoderReset(encoderBaseRight);

	const float multi1_2 = 1.0f/2.0f;
	const float multi3_4 = 3.0f/4.0f;
	const float multi1_3 = 1.0f/3.0f;
	const float multi2_3 = 2.0f/3.0f;

	float distance = tiles * TILE_RATIO_ADJUSTED / WHEEL_CIRCUMFERENCE;
	//printf("%f\n\r", distance);
	//printf("%d, %d\n\r", ch1, ch2);

	while (rotation < distance)
	{
		////printf("%d, %d", encoderGet(encoderLeft), encoderGet(encoderRight));
		rotation = (int)avg(abs(encoderGet(encoderBaseLeft)), abs(encoderGet(encoderBaseRight)));
		now = (	rotation < distance * multi1_3 ?	baseSetQuad(now, chS, chP, false, false, false, false, 0) :
				rotation < distance * multi3_4 ?	baseSetQuad(now, chS * multi2_3, chP * multi2_3, false, false, false, false, 0) :
													baseSetQuad(now, chS * multi1_2, chP * multi1_2, false, false, false, false, 0)
				);
		////printf("%d\n\r", rotation);
	}
	//printf("stopped\n\r");
	//now = baseSetQuad(now, 0, 0, false, false, false, false, 0);
	now = baseStop(now);
	return now;
}

Base moveForward(Base old, int speed, float tiles)
{
	return movementBase(old, speed, tiles, 0);
}

Base moveBackward(Base old, int speed, float tiles)
{
	return movementBase(old, speed, tiles, 180);
}

Base moveLeft(Base old, int speed, float tiles)
{
	return movementBase(old, speed, tiles, 90);
}

Base moveRight(Base old, int speed, float tiles)
{
	return movementBase(old, speed, tiles, -90);
}

Base turn(Base old, int speed, int turnAngle, bool left, bool right)
{
	Base now = (Base){0, 0, old.correction, 0};
	now.correction = resetLocalAngle(now.correction, gyroscope);
	//printf("localAngle : %d\n\r", now.correction.localAngle);

	const float multi1_2 = 1.0f/2.0f;
	const float multi3_4 = 3.0f/4.0f;
	const float multi1_3 = 1.0f/3.0f;
	const float multi2_3 = 2.0f/3.0f;

	//printf("p 1\n\r");

	while(abs(now.correction.localAngle) < turnAngle)
	{
		//printf("p 2\r\n");
		now.correction.localAngle = getLocalAngle(gyroscope);
		//printf("localAngle : %d\n\r", now.correction.localAngle);
		//printf("turnAngle : %d\n\r", turnAngle);
		now = baseSetQuad(now, 0, 0, left, false, right, false,
			(	abs(now.correction.localAngle) < turnAngle * multi1_3 ? 	speed :
				abs(now.correction.localAngle) < turnAngle * multi3_4 ? 	speed * multi2_3 :
																speed * multi1_2)
				);
		/*now = baseSetQuad(now, 0, 0, left, false, right, false,
			(	abs(now.correction.localAngle) < turnAngle * multi1_3 ? 	speed :
																speed * multi2_3)
				);*/
	}
	//printf("p 3\n\r");
	//check to see if gyroscope actually changes
	Gyro gyro = gyroscope;
	now.correction = resetLocalAngle(now.correction, gyro);
	now.correction.localAngle = getLocalAngle(gyroscope) - turnAngle * (right ? -1 : 1);
	now = baseStop(now);
	return now;
}

Base turnLeft(Base old, int speed, int turnAngle)
{
	return turn(old, speed, turnAngle, true, false);
}

Base turnRight(Base old, int speed, int turnAngle)
{
	return turn(old, speed, turnAngle, false, true);
}

