#include "utility_methods.h"

int sgn (float i)
{
	return (i > 0 ? 1 : i < 0 ? -1 : 0);
}

int sgnSimple(float i)
{
	return (i < 0 ? -1 : 1);
}

int sgnSimpleOther(float i)
{
	return (i > 0 ? 1 : -1);
}

float avg(int a, int b)
{
	return (a + b) / 2.0f + 0.5f;
}

float avgNums(int nums[]) {
	int size = sizeof(nums) / sizeof(nums[0]);
	int sum = 0;
	for (int i = 0; i < size; i++)
		sum += nums[i];
	return ((float)sum) / size;
}

float absAvg(int a, int b) {
	return (abs(a) + abs(b)) / 2.0f;
}

float absAvgNums(int nums[]) {
	int size = sizeof(nums) / sizeof(nums[0]);
	int sum = 0;
	for (int i = 0; i < size; ++i)
		sum += abs(nums[i]);
	return ((float)sum) / size;
}

int* absNums(int nums[]) {
	int size = sizeof(nums) / sizeof(nums[0]);
	for (int i = 0; i < size; ++i)
		nums[i] = abs(nums[i]);
	return nums;
}

bool adjustSpeedSpecific(int* speed, int old,
		int minIncFast, int maxIncFast,
		int minIncSlow, int maxIncSlow) {

	int direction = ( sgn(*speed) == sgn(old) ? sgnSimpleOther(abs(*speed) - abs(old)) : -1 );// ( sgn(*speed) ? (1) : (-1) ) );

	if (direction == 1) {//fast change, going further from zero
		return adjustSpeed(speed, old, minIncFast, maxIncFast);
	}

	//slow change, going toward zero
	return adjustSpeed(speed, old, minIncSlow, maxIncSlow);
}

bool adjustSpeed(int* speed, int old, int minInc, int maxInc) {
	int diffS = *speed - old;
	int sgnS = sgn(diffS);

	if (abs(diffS) > minInc){
		if (abs(diffS) > maxInc)
			diffS = sgnS * maxInc;
		*speed += diffS;
		if (abs(*speed) > MAX_SPEED)
			*speed = sgnS * MAX_SPEED;
		if (abs(*speed) < INPUT_THRESHOLD)
			*speed = 0;
		return true;
	}
	return false;
}

int adjustInputBtns(int input, bool* b1Pressed, bool* b2Pressed, bool* b3Pressed, bool* b4Pressed) {

	if (!*b1Pressed)
	{
		if (input & HOTKEY_POS_0)
			*b1Pressed = true;
	}
	else if (*b1Pressed)
	{
		if (!(input & HOTKEY_POS_0))
			*b1Pressed = false;
		else
			input ^= HOTKEY_POS_0;
	}

	if (!*b2Pressed)
	{
		if (input & HOTKEY_POS_1)
			*b2Pressed = true;
	}
	else if (*b2Pressed)
	{
		if (!(input & HOTKEY_POS_1))
			*b2Pressed = false;
		else
			input ^= HOTKEY_POS_1;
	}

	if (!*b3Pressed)
	{
		if (input & HOTKEY_POS_2)
			*b3Pressed = true;
	}
	else if (*b3Pressed)
	{
		if (!(input & HOTKEY_POS_2))
			*b3Pressed = false;
		else
			input ^= HOTKEY_POS_2;
	}

	if (!*b4Pressed)
	{
		if (input & HOTKEY_POS_3)
			*b4Pressed = true;
	}
	else if (*b4Pressed)
	{
		if (!(input & HOTKEY_POS_3))
			*b4Pressed = false;
		else
			input ^= HOTKEY_POS_3;
	}

	return input;
}
