/**
 * @file lcd.h
 * @file lcd.c
 *
 * @brief Lcd usage file
 *
 * This file allows for the interfacing between the lcd and the user. It allows for the lcd to change
 * its display as well as the actions of the buttons depending on what buttons the user presses. It
 * also allows certain items in the lcd to be updated every iteration like the motors so that
 * the user can observe its value in real time
 */
#ifndef LCD_H_
#define LCD_H_

/**
 * Includes
 */
#include "API.h"
#include "global_variable_initialization.h"
#include "utility_methods.h"
#include "shooter.h"
#include "FPS.h"

/**
 * This is used to update the lcd screen and respective actions of the buttons depending on the input
 * from the buttons
 *
 * @param input is the bytecode corresponding with the left, center, and right button of the lcd getting pressed
 */
void handleLcdUpdating(unsigned int input);

/**
 * This starts the lcd for when it is first supposed to start drawing to its screen
 */
void startLcdDisplay();

/**
 * This initializes the lcd and its presets to start usage from anywhere in the code
 */
void initLcdPresets();

void lcdTask(void* ignore);

#endif /* LCD_H_ */
