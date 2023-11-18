// 25/08/2023 testen van de motor + driver
// van https://lastminuteengineers.com/drv8825-stepper-motor-driver-arduino-tutorial/?utm_content=cmp-true 
// voor 2 motoren

// Define pin connections & motor's steps per revolution
const int dirPinA = 13;
const int stepPinA = 12;
const int dirPinB = 8;
const int stepPinB = 7;
const int stepsPerRevolution = 200;

void setup()
{
	// Declare pins as Outputs
	pinMode(stepPinA, OUTPUT);
	pinMode(dirPinA, OUTPUT);
	pinMode(stepPinB, OUTPUT);
	pinMode(dirPinB, OUTPUT);
}
void loop()
{
	// Set motor direction clockwise
	digitalWrite(dirPinA, HIGH);
	digitalWrite(dirPinB, LOW);

	// Spin motor slowly
	for(int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPinA, HIGH);
		digitalWrite(stepPinB, HIGH);
		delayMicroseconds(2000);
		digitalWrite(stepPinA, LOW);
		digitalWrite(stepPinB, LOW);
		delayMicroseconds(2000);
	}
	delay(1000); // Wait a second
	
	// Set motor direction counterclockwise
	digitalWrite(dirPinA, LOW);
	digitalWrite(dirPinB, HIGH);

	// Spin motor quickly
	for(int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPinA, HIGH);
		digitalWrite(stepPinB, HIGH);
		delayMicroseconds(1000);
		digitalWrite(stepPinA, LOW);
		digitalWrite(stepPinB, LOW);
		delayMicroseconds(1000);
	}
	delay(1000); // Wait a second
}