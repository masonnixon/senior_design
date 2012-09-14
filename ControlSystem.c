#include "ControlSystem.h"
#include "math.h"

float offset=30;

float Kp;
float Ki;
float Kd;

#if defined(IRSENSORS)
void controlsys( int IRdiff, int *MotorDutyCycle, float *integrator, float *lasterror ){

#elif defined(ACCELEROMETER)
void controlsys( float accelY, int *MotorDutyCycle, float *integrator, float *lasterror ){

#endif
	/* Thoughts on gain levels - Since the torque to pull the bike back up is dependent on the acceleration
		and not the speed of the flywheel, I believe that the Kp value needs to be pretty large.
		This will allow the wheel to speed up quickly.
	*/

	// read the value from kp potentiometer:
	ADMUX = _BV(REFS0) | 2;		// select channel ADC2
	ADCSRA |= _BV(ADSC);		// start the conversion
	loop_until_bit_is_clear(ADCSRA, ADSC);	// wait for conversion to complete
#if defined(IRSENSORS)
	Kp = (float)ADC / 1023 * 5;
#elif defined(ACCELEROMETER)
	Kp = (float)ADC / 1023 * 11500;
#endif

	// read the value from ki potentiometer:
	ADMUX = _BV(REFS0) | 3;		// select channel ADC3
	ADCSRA |= _BV(ADSC);		// start the conversion
	loop_until_bit_is_clear(ADCSRA, ADSC);	// wait for conversion to complete
#if defined(IRSENSORS)
	Ki = (float)ADC / 1023 * 2;
#elif defined(ACCELEROMETER)
	Ki = (float)ADC / 1023 * 150;
#endif
	
	// read the value from kd potentiometer:
	ADMUX = _BV(REFS0) | 4;		// select channel ADC4
	ADCSRA |= _BV(ADSC);		// start the conversion
	loop_until_bit_is_clear(ADCSRA, ADSC);	// wait for conversion to complete
#if defined(IRSENSORS)
	Kd = (float)ADC / 1023 * 2;
#elif defined(ACCELEROMETER)
	Kd = (float)ADC / 1023 * 0;
#endif
	
	int dt = 364;	//dt = sampling frequency
	float derivative;		// no need to initialize because its value is set before it is used
	float error;			// no need to initialize because its value is set before it is used

	PORTD |= _BV(PD5);
	if ((PIND & _BV(PD5)) == _BV(PD5)){
		#if defined(IRSENSORS)
			offset = IRdiff;
		#elif defined(ACCELEROMETER)
			offset = accelY;
		#endif
	}

	#if defined(IRSENSORS)
		error = offset - IRdiff;	// 0 is the balanced bicycle, so feedback is 0 minus the difference of the IR sensors
	#elif defined(ACCELEROMETER)
		error = offset - accelY;	// 0 is the balanced bicycle, so feedback is 0 minus the value from accel
	#endif

	/*	Below is a limiter. Its purpose is to set a threshold that keeps the flywheel from spinning
		when the bike is close enough to level. Otherwise the wheel will be continually trying to spin
		to make it perfect, which is a continual battle and will throw off our control. This value might
		need to be reduced to make sure that the angle is a small enough angle, but it should be large
		enough that the control system isn't consistently working.
	*/

	if (fabs(error) <= 4){
		*MotorDutyCycle  = 0; }
	else{
		*integrator = *integrator + (error/dt);	//integrator term sums all previous errors
/*		if (fabs(*integrator) > .5){	//limit integrator term. This will keep the integrator
			if (*integrator < 0 )		//from interfering with direction while still letting the 
				*integrator = -.5;		//integrator change the speed at which the control system corrects
			else
				*integrator = .5;}
*/			derivative = (error - (*lasterror))*dt;		//derivative term subtracts lasterror by current error
		*lasterror = error;
		*MotorDutyCycle =  Kp*error + Ki*(*integrator) + Kd*derivative;
		}

	/*This portion of the code checks to see if the duty cycle is negative. We will output a positive
	  duty cycle with a sign variable, thus we will take the absolute value of the negavtive duty cycle
	  and set our direction flag.
	*/
	if (*MotorDutyCycle>0x80)		//limits duty cycle to 128 as maximum
	*MotorDutyCycle = 0x80;
	else if (*MotorDutyCycle<-128)	//limits duty cycle to -128 as minimum
	*MotorDutyCycle = -128;
 
	*MotorDutyCycle = *MotorDutyCycle + 128 + 33;

	if (*MotorDutyCycle>0xFF)		//limits duty cycle to 255 as maximum
	*MotorDutyCycle = 0xFF;

} 

