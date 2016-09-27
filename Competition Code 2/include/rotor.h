/**
 * @file rotor.h
 * @file rotor.c
 *
 * @brief Allows for the changing of the robots pitch
 *
 * These methods allows for the individual adjustment of the rotors to allow for
 * precise pitch changes as well as calibration that should be done before any match
 */

#ifndef ROTOR_H_
#define ROTOR_H_

/**
 * Includes
 */
#include "API.h"
#include "global_variable_initialization.h"
#include "utility_methods.h"

/**
 * This allows for the position change of the left and right rotor without trying to have the
 * two motors adjust for one another. This is used for calibration of the rotors before any match
 * or practice scenario.
 *
 * @param speedL This is the speed the left motor should be set to
 * @param speedR This is the speed the right motor should be set to
 */
void setRotorRaw(int speedL, int speedR);

/**
 * This method changes the speeds of the left and right rotors to allow for the rotors to be moved outward
 * or brought inward. It also allows for the rotors to adjust to four different presets. It also corrects the
 * rotors so that they are aligned.
 *
 * @param adjU This adjusted the rotors by having them go outward.
 * @param adjD This adjusted the rotors by having them go inward.
 * @param pos0 This moves the rotor into the first preset position
 * @param pos1 This moves the rotor into the second preset position
 * @param pos2 This moves the rotor into the third preset position
 * @param posRest This moves the rotor into the initial position that it should start at
 */
void setRotor(bool adjU, bool adjD, bool pos0, bool pos1, bool pos2, bool posReset);

void rotorTask(void* ignore);

#endif /* ROTOR_H_ */
