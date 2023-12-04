// date: 15/08/2023
// uitbereiding van pythoncommunicationtest,
// queue implementatie van https://www.geeksforgeeks.org/queue-linked-list-implementation/

// constanten / intstellingen
const int dirPinA = 13;
const int stepPinA = 12;
const int dirPinB = 8;
const int stepPinB = 7;
const int magnetPin = 4;

const int stepsHorizontalMove = 55;
const int stepsDiagonalMove = 110;

const int SPECIAL_OP = 17;
const int K_CALIBRATION = 0;
const int K_MAGNET_ON = 1;
const int K_MAGNET_OFF = 2;
const int K_MOVE_UP = 3;
const int K_MOVE_DOWN = 4;
const int K_MOVE_LEFT = 5;
const int K_MOVE_RIGHT = 6;
const int K_MOVE_AUP = 7;
const int K_MOVE_BUP = 8;
const int K_MOVE_BDOWN = 9;
const int K_MOVE_ADOWN = 10;


// TODO: calculate
const int stepsToCM = 115; // 200 * (cm per rotation) 

const int mmsSpeedDelayMagnetOff = 450;
const int mmsSpeedDelayMagnetOn = 1000;

enum State {
  Idle, // fully idle, not moving, ignoring input
  Queueing, // waiting for queue, not moving, accepting input
  Moving, // moving, accepting input
  // MagnetOperation, // executing a magnet operation, not moving, accepting input (not needed)
  Calibrating, // busy calibrating, moving regardless of queue, accepting input
  FinishAction, // just done doing anything (update state), accepting input
};

bool movingMotorA;
bool movingMotorB;

bool magnetBit;

// in steps:
int currentX;
int currentY;
int toMoveA;
int toMoveB;

int mmsSpeedDelay = mmsSpeedDelayMagnetOff;
State state = State::Idle;

int incomingBytes[2] = {0,0};
bool firstByteIn = false;


void fancyPrint(String msg){
  String complete_msg = "{" + msg + "}";
  char *to_send = &complete_msg[0];
  Serial.write(to_send);
}


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
	pinMode(magnetPin, OUTPUT);
	Serial.begin(9600);

  // start accepting input
  state = State::Queueing;
}

// convert xy coord (0-16) to ab location (real world)
float bCoord(float x, float y){
  return (x-y) * -stepsToCM; // (- since flipped)
}
float aCoord(float x, float y){
  return (x+y) * -stepsToCM; // (- since flipped)
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
    fancyPrint("in0: " + String(incoming));
    if(incoming >= 0 && ((incoming == SPECIAL_OP && !firstByteIn) || incoming <= 16)){
      incomingBytes[firstByteIn] = incoming;
      if(firstByteIn){
        
        // fancyPrint("inA: " + String(incomingBytes[0]));
        // fancyPrint("inB: " + String(incomingBytes[1]));
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
  // digitalWrite(magnetPin, magnetBit);
  moveQ.deQueue(); 
  fancyPrint(String(magnetBit));
  if(magnetBit){
      mmsSpeedDelay = mmsSpeedDelayMagnetOn;
      digitalWrite(magnetPin, HIGH);
  } else {
      mmsSpeedDelay = mmsSpeedDelayMagnetOff;
      digitalWrite(magnetPin, LOW);
  }

}

void allowStateSwitch(){
  // std::cout << "switch states" << std::endl;
  if(moveQ.front == nullptr){
    state = State::Queueing;
    return;
  }
  delay(100);
  if(moveQ.front->x == SPECIAL_OP){
    if(moveQ.front->y == K_MAGNET_OFF) {
      magnetBit = false;
      // state = State::MagnetOperation;
      setMagnet();
      return;
    }
    if(moveQ.front->y == K_MAGNET_ON) {
      magnetBit = true;
      // state = State::MagnetOperation;
      setMagnet();
      return;
    }
    if(moveQ.front->y == K_CALIBRATION){
      currentX = 0;
      currentY = 0;
      toMoveA = 0;
      toMoveB = 0;
      fancyPrint("hi");
      moveQ.deQueue();
      return;
    }
    if(moveQ.front->y == K_MOVE_UP){
      toMoveA += stepsHorizontalMove;
      toMoveB -= stepsHorizontalMove;    
      currentY -= stepsHorizontalMove;
      movingMotorA = true;
      movingMotorB = true;  
      state = State::Moving;
      moveQ.deQueue();
      return;
    }
    if(moveQ.front->y == K_MOVE_DOWN){
      toMoveA -= stepsHorizontalMove;
      toMoveB += stepsHorizontalMove;      
      currentY += stepsHorizontalMove;   
      movingMotorA = true;
      movingMotorB = true; 
      state = State::Moving;
      moveQ.deQueue();
      return;
    }
    if(moveQ.front->y == K_MOVE_LEFT){
      toMoveA += stepsHorizontalMove;
      toMoveB += stepsHorizontalMove;      
      currentX -= stepsHorizontalMove;  
      movingMotorA = true;
      movingMotorB = true; 
      state = State::Moving;   
      moveQ.deQueue();
      return;
    }
    if(moveQ.front->y == K_MOVE_RIGHT){
      toMoveA -= stepsHorizontalMove;
      toMoveB -= stepsHorizontalMove;      
      currentX += stepsHorizontalMove;   
      movingMotorA = true;
      movingMotorB = true; 
      state = State::Moving;  
      moveQ.deQueue();
      return;
    }
    if(moveQ.front->y == K_MOVE_AUP){
      toMoveA += stepsDiagonalMove;
      currentX -= stepsDiagonalMove/2;
      currentY -= stepsDiagonalMove/2;
      movingMotorA = true;
      state = State::Moving;
      moveQ.deQueue();
      return;
    }
    if(moveQ.front->y == K_MOVE_ADOWN){
      toMoveA -= stepsDiagonalMove;
      currentX += stepsDiagonalMove/2;
      currentY += stepsDiagonalMove/2;
      movingMotorA = true;
      state = State::Moving;
      moveQ.deQueue();
      return;
    }
    if(moveQ.front->y == K_MOVE_BUP){
      toMoveB += stepsDiagonalMove;
      currentX -= stepsDiagonalMove/2;
      currentY += stepsDiagonalMove/2;
      movingMotorB = true;
      state = State::Moving;
      moveQ.deQueue();
      return;
    }
    if(moveQ.front->y == K_MOVE_BDOWN){
      toMoveB -= stepsDiagonalMove;
      currentX += stepsDiagonalMove/2;
      currentY -= stepsDiagonalMove/2;
      movingMotorB = true;
      state = State::Moving;
      moveQ.deQueue();
      return;
    }
    // throw std::invalid_argument("non valid special operation recieved");
    fancyPrint("non valid special operation recieved");
  
  }
    
  state = State::Moving;
  // std::cout << "go te next coord" << std::endl;
  // stepperA.moveTo(aCoord(moveQ.front->x,moveQ.front->y));
  // stepperB.moveTo(bCoord(moveQ.front->x,moveQ.front->y));
  toMoveA = aCoord(moveQ.front->x,moveQ.front->y) - aCoord((float)currentX/(float)stepsToCM,(float)currentY/(float)stepsToCM);
  toMoveB = bCoord(moveQ.front->x,moveQ.front->y) - bCoord((float)currentX/(float)stepsToCM,(float)currentY/(float)stepsToCM);
  // fancyPrint("current: (" + String(aCoord(currentX,currentY) + ", " + String(bCoord(currentX,currentY)));
  currentX = moveQ.front->x * stepsToCM;
  currentY = moveQ.front->y * stepsToCM;
  // fancyPrint("next: (" + String(aCoord(moveQ.front->x,moveQ.front->y)) + ", " + String(bCoord(moveQ.front->x,moveQ.front->y)));
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
    break;
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