// 25/08/2023 testen van de motor + driver
// van https://lastminuteengineers.com/drv8825-stepper-motor-driver-arduino-tutorial/?utm_content=cmp-true 

// Define pin connections & motor's steps per revolution
const int dirPinA = 2;
const int stepPinA = 3;
const int stepsPerRevolution = 200;

void setup()
{
	// Declare pins as Outputs
	pinMode(stepPinA, OUTPUT);
	pinMode(dirPinA, OUTPUT);
}
void loop()
{
	// Set motor direction clockwise
	digitalWrite(dirPinA, HIGH);

	// Spin motor slowly
	for(int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPinA, HIGH);
		delayMicroseconds(2000);
		digitalWrite(stepPinA, LOW);
		delayMicroseconds(2000);
	}
	delay(1000); // Wait a second
	
	// Set motor direction counterclockwise
	digitalWrite(dirPinA, LOW);

	// Spin motor quickly
	for(int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPinA, HIGH);
		delayMicroseconds(1000);
		digitalWrite(stepPinA, LOW);
		delayMicroseconds(1000);
	}
	delay(1000); // Wait a second
}