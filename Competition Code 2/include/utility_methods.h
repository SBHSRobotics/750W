/**
 * @file utility_methods.h
 * @file utility_methods.c
 *
 * @brief Allows for simple user utility functions to perform ubiquitous operations
 *
 * This class should be included in most of the files used by Team 750W. It allows
 * for the usage of algorithms and methods that are used unanimously across the code
 * as well as well as general methods that are needed for coding.
 */

#ifndef UTILITY_METHODS_H_
#define UTILITY_METHODS_H_

/**
 * Includes
 */
#include "API.h"
#include "global_variable_initialization.h"

/**
 * This will inform you about whether the input number
 * is positive, negative or zero
 *
 * @param i is the value to be decided if positive, negative or zero
 * @return the sign of the input
 */
int sgn (float i);

/**
 * This will inform you about whether the input number
 * is positive or negative and assumes it not to be zero
 *
 * If the number is zero then it will return a 1
 *
 * @param i is the value to be decided if positive or negative
 * @return the sign of the input
 */
int sgnSimple(float i);

/**
 * This will inform you about whether the input number
 * is positive or negative and assumes it not to be zero
 *
 * If the number is zero then it will return a -1
 *
 * @param i is the value to be decided if positive or negative
 * @return the sign of the input
 */
int sgnSimpleOther(float i);

/**
 * This methods returns the average value between two integers
 *
 * @param a is the first integer to take the average from
 * @param b is the second integer to take the average from
 *
 * @return the average as a float from the two integers
 */
float avg(int a, int b);

/**
 * This method gets the average from an array of numbers
 *
 * @param nums the array of integers that have to have the average taken of
 *
 * @return the average of the integers as a float
 */
float avgNums(int nums[]);

/**
 * This returns the average of the absolute value of both integers
 *
 * @param a the first integer to take the absolute value of and then the average of
 * @param b the second integer to take the absolute value of and then the average of
 *
 * @return the the average of taking the absolute value of both numbers
 */
float absAvg(int a, int b);

/**
 * This returns the average of taking the absolute value of multiple numbers
 *
 * @param nums are the integers that need to have the average taken of each elements
 * absolute value
 *
 * @return the average value of the absolute value of all the integers as a float
 */
float absAvgNums(int nums[]);

/**
 * Generates a list of integers which is of the absolute value of each element
 * in the input
 *
 * @param nums is array of integers that needs the absolute value of all of its integers
 *
 * @return an array that has the absolute value of each element from the input
 */
int* absNums(int nums[]);

/**
 * This method allows for the smooth skewing of a motor from its current value towards
 * its target value while ignoring differences that are negligible. It allows for the
 * specification of when the motors are speeding up to when they are slowing down.
 */
bool adjustSpeedSpecific(int* speed, int old,
		int minIncFast, int maxIncFast,
		int minIncSlow, int maxIncSlow);

/**
 * This method allows for the smooth skewing of a motor from its current value towards
 * its target value while ignoring differences that are negligible
 *
 * @param speed is the speed that the motor should end up as
 * @param old is the speed of the motor from the last iteration
 * @param minInc is the minimum amount of change from the old motor value that
 * is required. Otherwise, it does not change the motor speed
 * @param maxInc is the maximum amount of change from the old motor value. If the
 * change is too large then it will only change by the amount specified by maxInc.
 * Otherwise, it will change by the amount specified.
 */
bool adjustSpeed(int* speed, int old, int minInc, int maxInc);

/**
 * This method was made to allow for the smooth interpretation of button presses that
 * associate with certain presets. The interpretation is to only allow for one button to be
 * pressed at a time and for only when the button is initially pressed to be returned
 *
 * @param input is the bytecode made from several buttons akin to how the lcd returns a
 * single value to represent the value of the multiple buttons
 * @param b1Pressed is whether or not the first button is being held down
 * @param b2Pressed is whether or not the second button is being held down
 * @param b3Pressed is whether or not the third button is being held down
 * @param b4Pressed is whether or not the forth button is being held down
 *
 * @return which button has now been initally pressed down and not currently held down
 * mutually exclusive of any other button also being held down
 */
int adjustInputBtns(int input, bool* b1Pressed, bool* b2Pressed, bool* b3Pressed, bool* b4Pressed);

#endif /* UTILITY_METHODS_H_ */
