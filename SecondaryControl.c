#include "ControlSystem.h"
//#include <avr/eeprom.h>

float secoffset=0;
int dutycyclevalue=60;

void seccontrolsys( float accelY, int *MotorDutyCycle, int *MotorDirection){
	
	/* Thoughts on gain levels - Since the torque to pull the bike back up is dependent on the acceleration
		and not the speed of the flywheel, I believe that the Kp value needs to be pretty large.
		This will allow the wheel to speed up quickly.
	*/

	float error;

	PORTD |= _BV(PD5);
	if ((PIND & _BV(PD5)) == _BV(PD5)){
		secoffset = accelY;
	}

	error = secoffset - accelY;	// 0 is the balanced bicycle, so feedback is 0 minus the value from accel
	
	/*	Below is a limiter. Its purpose is to set a threshold that keeps the flywheel from spinning
		when the bike is close enough to level. Otherwise the wheel will be continually trying to spin
		to make it perfect, which is a continual battle and will throw off our control. This value might
		need to be reduced to make sure that the angle is a small enough angle, but it should be large
		enough that the control system isn't consistently working.
	*/

	if (error < 0){
		*MotorDutyCycle = dutycyclevalue;
		*MotorDirection = 1;}
	else if (error > 0){
		*MotorDutyCycle = dutycyclevalue;
		*MotorDirection = 0;}
	else *MotorDutyCycle = 0;
} 

