#ifndef __irsensors_h__
#define __irsensors_h__

#include "includes.h"

int readIR(int sensor);
int readIRsmoothed(int sensor, int ir_accum[], int *accum);

#endif
