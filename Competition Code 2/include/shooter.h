/**
 * @file shooter.h
 * @file shooter.c
 *
 * @brief This handles setting the speed of the shooter
 *
 * These methods allow for the changing of the shooter speeds as well as locking the shooter
 * at that specific speed. These speeds can be for certain presets or for setting the speed
 * of the shooter at your own specifications. You can also then perform incremental speed
 * Adjustment changes once the speed is locked.
 */

#ifndef SHOOTER_H_
#define SHOOTER_H_

/**
 * Includes
 */
#include "API.h"
#include "global_variable_initialization.h"
#include "utility_methods.h"

extern int oldShooterSpeed;

void setFinalShooterSpeeds(int speedL, int speedR);

/**
 * Sets the speed of the shooter without having any correction done to it.
 *
 * @param speed This is the speed the shooter should be set to
 */
void setShooter(int speed);

/**
 * This allows for the updating of the shooter speed to allow for it to be aligned with the
 * joystick input. It can then be locked from the joystick or from one of the buttons to be set to a preset.
 * It then can also have its speed incremented once the shooter is locked.
 *
 * @param hold This is to toggle holding the shooter speed at some value
 * @param adjU This allows for the incremental of the shooter speed up
 * @param adjD This allows for the decremental of the shooter speed down
 * @param joy This is the input from the joystick to be used for the shooter speed
 * @param pos0 This is the setting of the shooter speed to the first preset
 * @param pos1 This is the setting of the shooter speed to the second preset
 * @param pos2 This is the setting of the shooter speed to the third preset
 */
void handleShooterUpdating(bool hold, bool adjU, bool adjD, int joy,
		bool pos0, bool pos1, bool pos2);

void shooterStop();

void shooterTask(void* ignore);

#endif /* SHOOTER_H_ */
