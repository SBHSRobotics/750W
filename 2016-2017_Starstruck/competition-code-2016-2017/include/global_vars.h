#pragma once

/**
 *  Includes
 */
#include <math.h>
#include "API.h"


/*
 *  TIME_DELAY expresses the time in iterations of milliseconds in opcontrol.c
 *  to allow the Cortex time to rest and not overheat.
 */
#define TIME_DELAY 20

/*
 *  THRESH defines the minimum the joysticks must be past the base position to
 *  be considered on.
 */
#define THRESH 20

/*
 *  controlMode defines the type of movement the bot will be in for the operator.
 *  It allows iteration between multiple control modes for precision in multiple
 *  axes.
 */
extern unsigned int controlMode
