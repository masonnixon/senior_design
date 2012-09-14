#ifndef __Accelerometer_h__
#define __Accelerometer_h__

#include "includes.h"

float accelerometerIn(void);
float accelerometerSmoothed(float accumulator[], int *accel_accum);

#endif
