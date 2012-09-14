#include "includes.h"
#include "ControlSystem.h"
#include "hardwareInit.h"
#if defined(IRSENSORS)
	#include "irsensors.h"
#elif defined(ACCELEROMETER)
	#include "Accelerometer.h"
#else
	#warning "Must define sensor input!"	
#endif		

/***************************************************************************************************************
*                                           GLOBAL VARIABLES
***************************************************************************************************************/
#if defined(IRSENSORS)
	/* Infrared sensors */
	int		ir_LH_accum[MAFSIZE];
	int		ir_RH_accum[MAFSIZE];
	int		irDiff;
	int 	ir_start_cnt_LH = 0;	//Global counter since this can only be initialize at startup
	int 	ir_start_cnt_RH = 0;	//Global counter since this can only be initialize at startup

#elif defined(ACCELEROMETER)
	/* Accelerometer sensor */
	float	accumulator[MAFSIZE];
	float	accelY; 
	int 	accel_accum = 0; 		//Global counter since this can only be initialize at startup

#endif		

/* Control system */
float 	integrator = 0; 	// initialize the integrator error term
float 	lasterror = 0;		// initializes lasterror to zero because lasterror is used before its value is updated
int		MotorDutyCycle;		// newly calculated motor duty cycle

/***************************************************************************************************************
*                                           MAIN PROGRAM LOOP
***************************************************************************************************************/


 int main(){
	hardwareInit();	// initialize hardware
	_delay_ms(50);
	sei();					// enable interrupts
	
	for(;;){// infinite program loop

		#if defined(IRSENSORS)
			irDiff = readIRsmoothed(IR_LEFT, ir_LH_accum, &ir_start_cnt_LH) - readIRsmoothed(IR_RIGHT, ir_RH_accum, &ir_start_cnt_RH); // calculate new accelerometer data
			controlsys(irDiff, &MotorDutyCycle, &integrator, &lasterror);	// process control system
		#elif defined(ACCELEROMETER)
			accelY = accelerometerSmoothed(accumulator, &accel_accum); 		// calculate new accelerometer data
			controlsys(accelY, &MotorDutyCycle, &integrator, &lasterror);	// process control system
		#endif
		OCR0A = MotorDutyCycle;
		//_delay_ms(1);
		/* Toggle the test pin */
		if ((PORTD & _BV(PD7)) == _BV(PD7))
			PORTD &= ~_BV(PD7);
		else
			PORTD |= _BV(PD7);
	}
	return 0;
}
