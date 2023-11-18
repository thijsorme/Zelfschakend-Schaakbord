class AccelStepper {
public:
    AccelStepper(int type, int stepPin, int dirPin);


    void setMaxSpeed(int speed);
    void setAcceleration(int acc);
    int distanceToGo();
    void moveTo(int position);
    void run();


private:
    int abs(int in);
    int maxSpeed;
    int acceleration;
    int actualeration;

    int speed;
    int target;
    int position;
};

class Serial {
    public:
    static int index;
    static int available();
    static int read();
};