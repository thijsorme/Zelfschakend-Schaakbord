// 16/11/2023 combinatie van pythoncommunicationtest en motordriver test, upgrade controlmotortest2

const int dirPinA = 8;
const int stepPinA = 7;
const int dirPinB = 13;
const int stepPinB = 12;
int toMoveX;
int toMoveY;
const int stepsPerLoop = 100;
const int moveAmount = 1;

const int mmsDelaySpeed = 500;


void setup()
{
	// Declare pins as Outputs
	pinMode(stepPinA, OUTPUT);
	pinMode(dirPinA, OUTPUT);
	pinMode(stepPinB, OUTPUT);
	pinMode(dirPinB, OUTPUT);
	Serial.begin(9600);
}

void stepA()
{
  digitalWrite(stepPinA, HIGH);
  delayMicroseconds(mmsDelaySpeed);
  digitalWrite(stepPinA, LOW);
  delayMicroseconds(mmsDelaySpeed);
}
void stepB()
{
  digitalWrite(stepPinB, HIGH);
  delayMicroseconds(mmsDelaySpeed);
  digitalWrite(stepPinB, LOW);
  delayMicroseconds(mmsDelaySpeed);
	
}

void moveX(){
	if(toMoveX == 0){
		delayMicroseconds(2*mmsDelaySpeed);
		return;
	}
	if (toMoveX < 0){
		digitalWrite(dirPinA, LOW);
	}
	else if (toMoveX > 0){
		digitalWrite(dirPinA, HIGH);
	}
	stepA();
	toMoveX -= abs(toMoveX)/toMoveX;
}

void moveY(){
	if(toMoveY == 0){
		delayMicroseconds(2*mmsDelaySpeed);
		return;
	}
	if (toMoveY < 0){
		digitalWrite(dirPinB, LOW);
	}
	else if (toMoveY > 0){
		digitalWrite(dirPinB, HIGH);
	}
	stepB();
	toMoveY -= abs(toMoveY)/toMoveY;
}

void loop() {
	if (Serial.available() > 0) {

		// read the incoming byte:
		int deltaMove = Serial.read() - 35;

		if(deltaMove == 0){
			toMoveX -= moveAmount * stepsPerLoop;
			toMoveY += moveAmount * stepsPerLoop;
			Serial.print(1);
		}
		else if(deltaMove == 1){
			toMoveX -= moveAmount * stepsPerLoop;
			toMoveY -= moveAmount * stepsPerLoop;
			Serial.print(1);
		}
		else if(deltaMove == 2){
			toMoveX += moveAmount * stepsPerLoop;
			toMoveY -= moveAmount * stepsPerLoop;
			Serial.print(1);
		}
		else if(deltaMove == 3){
			toMoveX += moveAmount * stepsPerLoop;
			toMoveY += moveAmount * stepsPerLoop;
			Serial.print(1);
		}
	}

	moveX();
	moveY();
}
