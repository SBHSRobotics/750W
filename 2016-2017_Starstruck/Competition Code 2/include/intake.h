/**
 * @file intake.h
 * @file intake.c
 *
 * @brief Intake method declaration
 *
 * This file allows for the use of the intake smoothly for the desired speeds of the front and back intake
 */

#ifndef INTAKE_H_
#define INTAKE_H_

/**
 * Includes
 */
#include "API.h"
#include "global_variable_initialization.h"
#include "utility_methods.h"

/**
 * Used to set the desired speed of the front and back intake
 *
 * @param speedFront is the speed the front intake should be set to
 * @param speedBack is the speed the back intake should be set to
 */
void setIntake(int speedFront, int speedBack);

#endif /* INTAKE_H_ */
