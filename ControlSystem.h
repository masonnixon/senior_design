#ifndef __Control_System_h__
#define __Control_System_h__

#include "includes.h"

#if defined(IRSENSORS)
void controlsys( int IRdiff, int *MotorDutyCycle, float *integrator, float *lasterror );

#elif defined(ACCELEROMETER)
void controlsys( float accelY, int *MotorDutyCycle, float *integrator, float *lasterror );

#endif

void seccontrolsys( float accelY, int *MotorDutyCycle, int *MotorDirection);

#endif
