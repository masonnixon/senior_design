//Daniel, we can use the function accelerometerIn() to pass the tiltX and tiltY
//angles to the motor control function. I did not get to test this, but it should work.
//We just need to pass the values in.

int ledPin = 9;        // LED connected to digital pin 9
int dirPin = 10;       // DIR connected to digital pin 10
boolean motorDir;      // direction of motor spinning
int maxTestPWM;        // max test PWM - MUST BE EVEN #

void setup()  { 
  motorDir = HIGH;
  maxTestPWM = 80;    // must be an even number
  // initialize serial communications:
  Serial.begin(9600);
  // initialize the pins connected to the accelerometer as inputs:
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
} 

void accelerometerIn()	{
// these constants won't change:
const int xPin = 2;		// X output of the accelerometer
const int yPin = 3;		// Y output of the accelerometer

  // variables to read the pulse widths:
  int pulseX, pulseY;
  // variables to contain the resulting accelerations
  int accelerationX, accelerationY;
  long z1, z2;
  float tiltX, tiltY;
  
  // read pulse from x- and y-axes:
  pulseX = pulseIn(xPin,HIGH);  
  pulseY = pulseIn(yPin,HIGH);
 
  // convert the pulse width into acceleration
  // accelerationX and accelerationY are in milli-g's: 
  // earth's gravity is 1000 milli-g's, or 1g.
  
  accelerationX = ((pulseX / 10) - 500) * 8;
  accelerationY = ((pulseY / 10) - 500) * 8;
 
  // Since the ouput from the accelerometer is a number between 0 and 1 which
  // corresponds to 0 to 90 degrees, one can multiply the acceleration calculated
  // by 9 and divide by 100.0 and obtain an angle out to up to 4 digits.
 
  //z1 = (accelerationX * 9); 
  //z2 = (accelerationY * 9);

  //Calculate the angle to 2 decimal places by making a long into a float.

  //tiltX = (float)z1 / 100.0;
  //tiltY = (float)z2 / 100.0;
  
  
  return tiltY;
 // Serial.print("tiltX: ");
 // Serial.print(tiltX);
 // Serial.print("\t");
 // Serial.print("tiltY: "); 
 // Serial.println(tiltY);
 // Serial.print("\n");
  
}

void loop()  {
  // change the motor direction every accel/decel cycle
/*  motorDir = !motorDir;
  digitalWrite(dirPin,motorDir);
  
  // fade in from min to some max test speed in increments of 2 points:
  for(int fadeValue = 0 ; fadeValue <= maxTestPWM; fadeValue +=2) { 
    // sets the pwm (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 20 milliseconds to limit the motor acceleration    
    delay(20);     
  } 
  
  // hold motor speed for 2 seconds
  delay(2000);
  
  // fade out from some max test speed to min in increments of 2 points:
  for(int fadeValue = maxTestPWM ; fadeValue >= 0; fadeValue -=2) { 
    // sets the pwm (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 20 milliseconds to limit the motor deceleration 
    delay(20);      
  } 
  
  // keep motor off for 1 second
  delay(1000);
*/

tiltValue=accelerometerIn();
pwmOut=abs(tiltValue)*255;

analogWrite(ledPin, pwmOut);

if (tiltValue < 0)
  motorDir = LOW;
else if (tiltValue >= 0)
  motorDir = HIGH;

digitalWrite(dirPin,motorDir);

}

//Do I need to loop this, or does the name loop do that automatically?
//Is there a rounding function for Arduino?
//How do you add includes to an Arduino sketch?
//Does this implementation seem acceptable/are there any suggestions for a test run?
