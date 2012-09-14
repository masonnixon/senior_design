#include "irsensors.h"

int sensorValue;

int readIR(int sensor) {
	if (sensor == IR_LEFT){
		// read the value from the left IR sensor:
		ADMUX = _BV(REFS0) | 0;		// select channel ADC0
		ADCSRA |= _BV(ADSC);		// start the conversion
		loop_until_bit_is_clear(ADCSRA, ADSC);	// wait for conversion to complete
		sensorValue = ADC;
	}else if (sensor == IR_RIGHT){
		// read the value from the right IR sensor:
		ADMUX = _BV(REFS0) | 1;		// select channel ADC1
		ADCSRA |= _BV(ADSC);		// start the conversion
		loop_until_bit_is_clear(ADCSRA, ADSC);	// wait for conversion to complete
		sensorValue = ADC;
	}
	return sensorValue;
}

int readIRsmoothed(int sensor, int ir_accum[], int *accum){
	int i=0;
	int maf;	
	int sum=0;
	
	for(i=0;i<=(MAFSIZE-2);i++){
	  ir_accum[i]=ir_accum[i+1]; //Shift out the oldest
	}
	ir_accum[MAFSIZE-1]=readIR(sensor); //Retrieve most recent noisy data point

	for(i=0;i<=(MAFSIZE-1);i++){
	  sum+=ir_accum[i]; //Sum of 5 terms
	}

	maf=sum/MAFSIZE; //Average of the 5 data points
	*accum+=1; //Increment through first 5 points
	if(*accum<MAFSIZE){
		return readIR(sensor);
	} //Wait for first 5 points to be accumulated
	else{
		return maf;
	}
}
