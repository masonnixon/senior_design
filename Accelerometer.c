#include "Accelerometer.h"

volatile unsigned long AccPeriod;
volatile unsigned long AccPulseWidth;
volatile unsigned int newTCNT1, TLH;
volatile unsigned int Timer1OVcount=0;

/***************************************************************************************************************
*                             TIMER1 INPUT CAPTURE ISR (ACCELEROMETER INPUT)
***************************************************************************************************************/
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__)
SIGNAL(TIMER1_CAPT_vect){
	newTCNT1 = TCNT1;
	/* read the period of the accelerometer pwm signal (low byte first) */
	if ((TCCR1B & _BV(ICES1)) == _BV(ICES1)){// if currently rising edge triggered
		AccPeriod = (long)newTCNT1 + (65535*Timer1OVcount) - (long)TLH;
		TLH = newTCNT1;
		Timer1OVcount = 0;	// clear the overflow counter
		/* switch to falling edge triggered interrupts */
		TCCR1B &= ~_BV(ICES1);		/* Switch to falling edge triggered interrupt */
	}
	else{
		/* read the pulse width (time high) of the accelerometer pwm signal (low byte first) */
		AccPulseWidth = (long)newTCNT1 + (65535*Timer1OVcount) - (long)TLH;
		/* switch to rising edge triggered interrupts */
		TCCR1B |= _BV(ICES1);
	}

	/* Toggle the test pin
	if ((PORTD & _BV(PD7)) == _BV(PD7))
		PORTD &= ~_BV(PD7);
	else
		PORTD |= _BV(PD7); */
}
SIGNAL(TIMER1_OVF_vect){
	Timer1OVcount++;
}

#elif defined(__AVR_ATmega1280__)
SIGNAL(SIG_INPUT_CAPTURE4){

	if ((TCCR4B & _BV(ICES4)) == _BV(ICES4)){// if currently rising edge triggered
		/* read the period of the accelerometer pwm signal (low byte first) */
		AccPeriod = ICR4;
		/* switch to falling edge triggered interrupts */
		TCCR4B &= ~_BV(ICES4);
	}
	else{
		/* read the pulse width (time high) of the accelerometer pwm signal (low byte first) */
		AccPulseWidth = ICR4;
		/* switch to rising edge triggered interrupts */
		TCCR4B |= _BV(ICES4);
	}

	/* Toggle the test pin 
	if ((PORTE & _BV(PE3)) == _BV(PE3))
		PORTE &= ~_BV(PE3);
	else
		PORTE |= _BV(PE3);*/
}
#else
#warning "Chip not defined"
#endif

/***************************************************************************************************************
*                           			ACCELEROMETER PROCESSING FUNCTION
***************************************************************************************************************/
float accelerometerIn(void)	{
	float retVal;
	float a,b,c;
	cli();	// disable interrupts

	// convert the pulse width into acceleration
	// accelY is in milli-g's....earth's gravity is 1000 milli-g's, or 1g.
	a=(float)AccPulseWidth;
	b=(float)AccPeriod;
	c=a/b;
	retVal = (c - 0.5) * 8.0;	// outputs a number between -4 and 4
	//retVal = (((AccPulseWidth_local*1000L / AccPeriod_local) - 500L) * 8L);

	sei();	// enable interrupts
	return retVal;
}

float accelerometerSmoothed(float accumulator[], int *accel_accum) {
	int i=0;
	float maf;
	float sum=0;
	
	for(i=0;i<=(MAFSIZE-2);i++){
	  accumulator[i]=accumulator[i+1]; //Shift out the oldest
	}
	accumulator[MAFSIZE-1]=accelerometerIn(); //Retrieve most recent noisy data point
	
	for(i=0;i<=(MAFSIZE-1);i++){
	  sum+=accumulator[i]; //Sum of 5 terms
	}
	
	maf=sum/MAFSIZE; //Average of the 5 data points
	*accel_accum+=1; //Increment through first 5 points
	if(*accel_accum<MAFSIZE){
		return accelerometerIn();
	} //Wait for first 5 points to be accumulated
	else{
		return maf;
	}
}

