// date: 15/08/2023
// uitbereiding van pythoncommunicationtest,
// queue implementatie van https://www.geeksforgeeks.org/queue-linked-list-implementation/
#include <AccelStepper.h>

int incomingBytes[2] = {0,0};
bool firstByteIn = false;
bool movingMotorA = false; // (-1,1)
bool movingMotorB = false; // (1,1)

const int magnetCode = 18;

const int dirPinA = 2;
const int stepPinA = 3;
const int dirPinB = 4;
const int stepPinB = 5;

// Creates an instance
AccelStepper stepperA(1, stepPinA, dirPinA);
AccelStepper stepperB(1, stepPinB, dirPinB);

struct QNode {
    int x;
    int y;
    QNode* next;
    QNode(int xCoord, int yCoord)
    {
        x = xCoord;
        y = yCoord;
        next = NULL;
    }
};
 
struct Queue {
    QNode *front, *rear;
    Queue() { front = rear = NULL; }
 
    void enQueue(int x, int y)
    {
        // Create a new LL node
        QNode* temp = new QNode(x,y);
 
        // If queue is empty, then
        // new node is front and rear both
        if (rear == NULL) {
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
        if (front == NULL)
            return;
 
        // Store previous front and
        // move front one node ahead
        QNode* temp = front; // waarom is deze temp een ding??
        front = front->next;
 
        // If front becomes NULL, then
        // change rear also as NULL
        if (front == NULL)
            rear = NULL;
 
        delete (temp);
    }
};
 
Queue moveQ;

void setup() {
  Serial.begin(9600);
	stepperA.setMaxSpeed(100);
	stepperA.setAcceleration(10);
  stepperB.setMaxSpeed(100);
	stepperB.setAcceleration(10);
}

const int stepsToCM = -1; // 200 * (cm per rotation) 

int aCoord(int x, int y){
  // 200 steps per revolution
  // x cm per revolution

  return 1;
}

int bCoord(int x, int y){
  // 200 steps per revolution
  // x cm per revolution

  return 1;
}


void loop() {
  if (Serial.available() > 0) {
    int incoming = Serial.read() - ' ';

    if(incoming >= 0 && incoming <= 16){
      incomingBytes[firstByteIn] = incoming;
      if(firstByteIn){
        moveQ.enQueue(incomingBytes[0],incomingBytes[1]);
        incomingBytes[0] = 0;
        incomingBytes[1] = 0;
        movingMotorA = true;
        movingMotorB = true;
      }
      firstByteIn = !firstByteIn;
      
    } else {
      incomingBytes[0] = 0;
      incomingBytes[1] = 0;
      firstByteIn = false;
    }
  }

  if(movingMotorA || movingMotorB){
    if (stepperA.distanceToGo() == 0 && stepperB.distanceToGo() == 0) {
      if(moveQ.front == NULL){
        movingMotorA = false;
        movingMotorB = false;
      } else if(moveQ.front->x == magnetCode){
        // turn magnet on/off based on front->y
      } else {
        stepperA.moveTo(aCoord(moveQ.front->x,moveQ.front->y));
        stepperB.moveTo(bCoord(moveQ.front->x,moveQ.front->y));
        moveQ.deQueue();
      }
    } else {
	    stepperA.run();
	    stepperB.run();
    }
  }

}
