// date: 15/08/2023
// uitbereiding van pythoncommunicationtest,
// queue implementatie van https://www.geeksforgeeks.org/queue-linked-list-implementation/
#include <string>
int incomingBytes[2] = {0,0};
bool firstByteIn = false;

bool magnetBit;

const int specialOperationCode = 17;
const int calibrationKey = 0;
const int magnetOnKey = 1;
const int magnetOffKey = 2;

const int dirPinA = 8;
const int stepPinA = 7;
const int dirPinB = 13;
const int stepPinB = 12;

int currentA;
int currentB;
int toMoveA;
int toMoveB;

const int stepsPerLoop = 100;
const int moveAmount = 1;

const int mmsSpeedDelayMagnetOff = 600;
const int mmsSpeedDelayMagnetOn = 1000;
int mmsSpeedDelay = mmsSpeedDelayMagnetOff;
// TODO: calculate
const int stepsToCM = 50; // 200 * (cm per rotation) 

enum State {
  Idle, // fully idle, not moving, ignoring input
  Queueing, // waiting for queue, not moving, accepting input
  Moving, // moving, accepting input
  // MagnetOperation, // executing a magnet operation, not moving, accepting input (not needed)
  Calibrating, // busy calibrating, moving regardless of queue, accepting input
  FinishAction, // just done doing anything (update state), accepting input
};

bool movingMotorA = false;
bool movingMotorB = false;

State state = State::Idle;

struct QNode {
    int x;
    int y;
    QNode* next;
    QNode(int xCoord, int yCoord)
    {
        x = xCoord;
        y = yCoord;
        next = nullptr;
    }
};
 
struct Queue {
    QNode *front, *rear;
    Queue() { front = rear = nullptr; }
 
    void enQueue(int x, int y)
    {
        // Create a new LL node
        QNode* temp = new QNode(x,y);
 
        // If queue is empty, then
        // new node is front and rear both
        if (rear == nullptr) {
            front = rear = temp;
            return;
        }
 
        // Add the new node at
        // the end of queue and change rear
        rear->next = temp;
        rear = temp;
    }
 
    // Function to remove
    // a key from given queue q
    void deQueue()
    {
        // If queue is empty, return NULL.
        if (front == nullptr)
            return;
 
        // Store previous front and
        // move front one node ahead
        QNode* temp = front; // waarom is deze temp een ding??
        front = front->next;
 
        // If front becomes NULL, then
        // change rear also as NULL
        if (front == nullptr)
            rear = nullptr;
 
        delete (temp);
    }
};
 
Queue moveQ;

void setup() {
 	pinMode(stepPinA, OUTPUT);
	pinMode(dirPinA, OUTPUT);
	pinMode(stepPinB, OUTPUT);
	pinMode(dirPinB, OUTPUT);
	Serial.begin(9600);

  // start accepting input
  state = State::Queueing;
}

void printWithFormat(String message){
  String completeMsg = "{" + message + "}";
  char *toSend = &completeMsg[0];
  Serial.write(toSend);
}

// convert xy coord (0-16) to ab location (real world)
int aCoord(int x, int y){
  // 200 steps per revolution - x cm per revolution
  // TODO: calculation
  int value = stepsToCM*(x+y)/2;
  printWithFormat("a");
  printWithFormat(String(value));
  // std::cout << "a " << value << std::endl;
  return value;
}
int bCoord(int x, int y){
  // 200 steps per revolution - x cm per revolution
  // TODO: calculation
  int value = stepsToCM*(x-y)/2;
  printWithFormat("B");
  printWithFormat(String(value));
  // std::cout << "b " << value << std::endl;

  return value;
}
void stepA()
{
  digitalWrite(stepPinA, HIGH);
  delayMicroseconds(mmsSpeedDelay);
  digitalWrite(stepPinA, LOW);
  delayMicroseconds(mmsSpeedDelay);
}
void stepB()
{
  digitalWrite(stepPinB, HIGH);
  delayMicroseconds(mmsSpeedDelay);
  digitalWrite(stepPinB, LOW);
  delayMicroseconds(mmsSpeedDelay);
}

void moveA(){
	if(toMoveA == 0){
		delayMicroseconds(2*mmsSpeedDelay);
		return;
	}
	if (toMoveA < 0){
		digitalWrite(dirPinA, LOW);
	}
	else if (toMoveA > 0){
		digitalWrite(dirPinA, HIGH);
	}
	stepA();
  int delta = abs(toMoveA)/toMoveA;
	toMoveA -= delta;
}

void moveB(){
	if(toMoveB == 0){
		delayMicroseconds(2*mmsSpeedDelay);
		return;
	}
	if (toMoveB < 0){
		digitalWrite(dirPinB, LOW);
  }
	else if (toMoveB > 0){
		digitalWrite(dirPinB, HIGH);
	}
	stepB();
  int delta = abs(toMoveB)/toMoveB;
  
	toMoveB -= delta;
}

// TODO: moving motor A, moving motor B.
void checkForInput(){
  if (Serial.available() > 0) {
    int incoming = Serial.read() - ' ';
    // std::cout << "incoming info: " << incoming << std::endl;
    if(incoming >= 0 && ((incoming == specialOperationCode && !firstByteIn) || incoming <= 16)){
      incomingBytes[firstByteIn] = incoming;
      if(firstByteIn){
        printWithFormat(String(incomingBytes[0]));
        printWithFormat(String(incomingBytes[1]));
        moveQ.enQueue(incomingBytes[0],incomingBytes[1]);
        incomingBytes[0] = 0;
        incomingBytes[1] = 0;
      }
      firstByteIn = !firstByteIn;
    } 
    else {
      incomingBytes[0] = 0;
      incomingBytes[1] = 0;
      firstByteIn = false;
    }
  }
}

void setMagnet(){
  // turn on or off magnet based on magnetBit
}

void allowStateSwitch(){
  // std::cout << "switch states" << std::endl;
  if(moveQ.front == nullptr){
    state = State::Queueing;
    return;
  }
  if(moveQ.front->x == specialOperationCode){
    if(moveQ.front->y == magnetOffKey) {
      magnetBit = false;
      mmsSpeedDelay = mmsSpeedDelayMagnetOff;
      // state = State::MagnetOperation;
      setMagnet();
      return;
    }
    if(moveQ.front->y == magnetOnKey) {
      magnetBit = true;
      mmsSpeedDelay = mmsSpeedDelayMagnetOn;
      // state = State::MagnetOperation;
      setMagnet();
      return;
    }
    if(moveQ.front->y == calibrationKey){
      state = State::Calibrating;
      return;
    } else {
      // throw std::invalid_argument("non valid special operation recieved");
      printWithFormat("non valid special operation recieved");
    }
  }
    
  state = State::Moving;
  // std::cout << "go te next coord" << std::endl;
  // stepperA.moveTo(aCoord(moveQ.front->x,moveQ.front->y));
  // stepperB.moveTo(bCoord(moveQ.front->x,moveQ.front->y));
  int newCoordA = aCoord(moveQ.front->x,moveQ.front->y);
  int newCoordB = bCoord(moveQ.front->x,moveQ.front->y);
  toMoveA = newCoordA - currentA;
  toMoveB = newCoordB - currentB;
  currentA = newCoordA;
  currentB = newCoordB;
  movingMotorA = true;
  movingMotorB = true;
  moveQ.deQueue(); 
}

void runMotors(){
  if(movingMotorA){
    moveA();
    if(toMoveA == 0) {
      movingMotorA = false;
    }
  }
  if(movingMotorB){
    moveB();
    if(toMoveB == 0) {
      movingMotorB = false;
    }
  }
  if(!movingMotorA && !movingMotorB) {
    // std::cout << "done moving" << std::endl;
    state = State::FinishAction;
  }
}

void calibrating(){
  // check for bump, reset motors (idk how tho)
  bool movingX = true;
  bool movingY = true;
  while(true){
    // check still have to move xy
    // move xy accordingly
    // both done? exit loop
  }
  // set 0 0
  runMotors();
}

void loop() {
  switch (state)
  {
  case State::Idle:
    // do absolutly the fuck nothing (here for good measure)
    break;
  case State::Queueing:
    // accept and process input ONLY
    checkForInput();
    allowStateSwitch();
    break;
  case State::Moving:
    // accept input, step motor, 
    // there yet? update state -> finishmoving
    runMotors();
    checkForInput();
    break;
  case State::FinishAction:
    // set next target from Q (accept input? do we need to? for one frame?)
    // update state -> moving/magnetoperation/calibration (/queueing)
    allowStateSwitch();
    break;
  case State::Calibrating:
    // keep moving (advance by tiny steps) until hit limit switches, 
    // there? -> reset + update state -> finishmoving
    calibrating();
    break;
  // case State::MagnetOperation:
  //   // turn on or off magnet (accept input? do we need to? for one frame?)
  //   // update state -> finishmoving
  //   break;
  
  default:
    break;
  }
}