#ifndef INCLUDE_AUTONOMOUS_UTILITY_METHODS_H_
#define INCLUDE_AUTONOMOUS_UTILITY_METHODS_H_

#include <math.h>
#include "global_variable_initialization.h"
#include "API.h"
#include "base.h"
#include "relative_and_global_angle.h"
#include "shooter.h"
#include "rotor.h"
#include "intake.h"

Base movementBase(Base old, int speed, float tiles, int direction);

Base moveForward(Base old, int speed, float tiles);

Base moveBackward(Base old, int speed, float tiles);

Base moveLeft(Base old, int speed, float tiles);

Base moveRight(Base old, int speed, float tiles);

Base turn(Base old, int speed, int turnAngle, bool left, bool right);

Base turnLeft(Base old, int speed, int turnAngle);

Base turnRight(Base old, int speed, int turnAngle);


#endif /* INCLUDE_AUTONOMOUS_UTILITY_METHODS_H_ */
