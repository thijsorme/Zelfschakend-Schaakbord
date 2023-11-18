// 15/11/2023 combinatie van pythoncommunicationtest en motordriver test

// Define pin connections & motor's steps per revolution

const int dirPinA = 8;
const int stepPinA = 7;
const int dirPinB = 13;
const int stepPinB = 12;
const int stepsPerLoop = 100;
int toMoveX;
int toMoveY;
const int moveAmount = 1;

const int mmsDelaySpeed = 1000;


void setup()
{
	// Declare pins as Outputs
	pinMode(stepPinA, OUTPUT);
	pinMode(dirPinA, OUTPUT);
	pinMode(stepPinB, OUTPUT);
	pinMode(dirPinB, OUTPUT);
	Serial.begin(9600);
}

void spinA()
{
	for(int x = 0; x < stepsPerLoop; x++)
	{
		digitalWrite(stepPinA, HIGH);
		delayMicroseconds(mmsDelaySpeed);
		digitalWrite(stepPinA, LOW);
		delayMicroseconds(mmsDelaySpeed);
	}
}
void spinB()
{
	for(int x = 0; x < stepsPerLoop; x++)
	{
		digitalWrite(stepPinB, HIGH);
		delayMicroseconds(mmsDelaySpeed);
		digitalWrite(stepPinB, LOW);
		delayMicroseconds(mmsDelaySpeed);
	}
}

void moveX(){
	if(toMoveX == 0){
		delay(40);
		return;
	}
	if (toMoveX < 0){
		digitalWrite(dirPinA, LOW);
		delay(20);
	}
	else if (toMoveX > 0){
		digitalWrite(dirPinA, HIGH);
		delay(20);
	}
	spinA();
	toMoveX -= abs(toMoveX)/toMoveX;
}

void moveY(){
	if(toMoveY == 0){
		delay(40);
		return;
	}
	if (toMoveY < 0){
		digitalWrite(dirPinB, LOW);
		delay(20);
	}
	else if (toMoveY > 0){
		digitalWrite(dirPinB, HIGH);
		delay(20);
	}
	spinB();
	toMoveY -= abs(toMoveY)/toMoveY;
}

void loop() {
	if (Serial.available() > 0) {

		// read the incoming byte:
		int deltaMove = Serial.read() - 35;

		if(deltaMove == 0){
			toMoveX -= moveAmount;
			toMoveY += moveAmount;
			Serial.print(1);
		}
		else if(deltaMove == 1){
			toMoveX -= moveAmount;
			toMoveY -= moveAmount;
			Serial.print(1);
		}
		else if(deltaMove == 2){
			toMoveX += moveAmount;
			toMoveY -= moveAmount;
			Serial.print(1);
		}
		else if(deltaMove == 3){
			toMoveX += moveAmount;
			toMoveY += moveAmount;
			Serial.print(1);
		}
	}

	moveX();
	moveY();
}
