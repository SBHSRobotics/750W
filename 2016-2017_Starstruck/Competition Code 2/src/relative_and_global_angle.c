#include "relative_and_global_angle.h"

int gyroOffset;

void initGyroscope()
{
	gyroOffset = 0;
}

int getAngle(int left, int right)
{
	//printf("%d %d ", left, right);

	float x = M_SQRT2 * (left - right);
	float y = M_SQRT2 * (left + right);

	if (x == 0)
	{
		if (y >= 0)
			return 0;
		return 180;
	}
	int i = ((int)(-sgn(x) * TO_DEGREE * acos(y / sqrt(x * x + y * y))));
	//printf("%d\n\r", i);
	return i;
}

inline int getLocalAngle(Gyro gyro)
{
	return gyroOffset + gyroGet(gyro) % 360;//avg(encoderGet(left), encoderGet(right));
}

inline int getGlobalAngle(Correction correction, Gyro gyro)
{
	return (correction.globalAngle + getLocalAngle(gyro)) % 360;//avg(encoderGet(left), encoderGet(right));
}

Correction resetLocalAngle(Correction correction, Gyro gyro)
{
	correction.globalAngle = getGlobalAngle(correction, gyro);
	correction.localAngle = 0;
	gyroReset(gyro);

	gyroOffset = 0;


	return correction;
}

Correction resetGlobalAngle(Correction correction, Gyro gyro)
{
	correction.globalAngle = 0;
	correction.localAngle = 0;
	gyroReset(gyro);

	gyroOffset = 0;

	return correction;
}