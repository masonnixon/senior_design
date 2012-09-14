#include "hardwareInit.h"

void hardwareInit(void){

		/* Set up timer1 for accelerometer */
		TCCR1B |= _BV(CS10); 	// prescale F_CLK_IO by 8
		TIMSK1 |= _BV(TOIE1);	// enable timer overflow interrupts
		TIFR1 |= 1 << TOV1;		// clear pending overflow interrupts
		/* Set up timer0 for motor control */
		TCCR0A |= _BV(COM0A1);  // inverted pwm outputs
		TCCR0A |= _BV(WGM01) | _BV(WGM00);	// Phase correct mode - ICR0 is top
		TCCR0B |= _BV(CS01); 	// prescale F_CLK_IO by 8

		ADCSRA |= _BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);	// ADC prescale factor is 128 [Fcpu/128]
		DIDR1 |= _BV(AIN1D) | _BV(AIN0D);			// disable digital input buffers on ADC pins
		ADCSRA |= _BV(ADEN);						// enable ADC conversions
		ADMUX  |= _BV(REFS0); 						// AVCC with external capacitor at AREF pin

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__)

		/* Set up pin 6 for motor controller PWM output. */
		DDRD |= _BV(PD6); 			//Set PD6 for output
		PORTD |= _BV(PD6);			//Set  PD6 to Output High
		
		/* This sets the input pins for Pin 8. Pin 8 is our Accelerometer PWM input. */
		DDRB &= ~_BV(PB0);
		PORTB &= ~_BV(PB0);

		/* Set up pin 10 for motor controller direction output. */
		DDRB |= _BV(PB2);			//Set PB2 to output
		PORTB |= _BV(PB2);			//Set PB2 to Output high

		/* Enable Timer1 interrupts. Rising edge triggered initially */
		TIMSK1 |= _BV(ICIE1);
		TCCR1B |= _BV(ICES1);

		/* Set up pin 7 for accelerometer interrupt test pin. */
		DDRD |= _BV(PD7);			//Set PD7 to output
		PORTD |= _BV(PD7);			//Set PD7 to Output high

#elif defined(__AVR_ATmega1280__)
		
		/* Set up pin 11 for motor controller PWM output. */
		DDRB |= _BV(PB5); 			//Set PB5 for output
		PORTB |= _BV(PB5);			//Set PB5 to Output High

		/* This sets the input pins for Pin 49. Pin 49 is our Accelerometer PWM input. */
		DDRL&= ~_BV(PL0);
		PORTL&= ~_BV(PL0);
		
		/* Set up pin 12 for motor controller direction output. */
		DDRB |= _BV(PB6);			//Set PB6 to output
		PORTB |= _BV(PB6);			//Set PB6 to Output high	

		/* Set up timer4 */
		TCCR4A |= _BV(COM4A0);
		TCCR4B |= _BV(CS41);

		/* Enable Timer4 interrupts. Rising edge triggered initially */
		TIMSK4 |= _BV(ICIE4);
		TCCR4B |= _BV(ICES4);
#else
#warning "Chip not defined"		
#endif
/*
OCR0A = 40;
for (i=0;i<=100;i++){
	PORTB |= _BV(PB2);
	_delay_ms(300);
	PORTB &= ~_BV(PB2);
	_delay_ms(300);
	}
*/
}
