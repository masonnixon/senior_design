#ifndef __includes_h__
#define __includes_h__

#define F_CLK_IO		16000000L
#define F_CPU			16000000L

#define IRSENSORS		1		// use IR sensors
//#define	ACCELEROMETER	1	// uncomment to use accelerometer

#define MAFSIZE			12		// size of the moving average filters

#define IR_LEFT			0
#define IR_RIGHT		1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define sbi(port, bit) (port) |= (1 << (bit))

#endif
