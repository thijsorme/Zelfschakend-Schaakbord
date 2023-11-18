#include "accelsimulator.h"
#include <iostream>
#include <chrono>
#include <thread>

AccelStepper::AccelStepper(int type, int stepPin, int dirPin)
{
}

void AccelStepper::setMaxSpeed(int speed)
{
    maxSpeed = speed;
}

void AccelStepper::setAcceleration(int acc)
{
    acceleration = acc;
    actualeration = acc;
}

int AccelStepper::distanceToGo()
{
    return target-position;
}

void AccelStepper::moveTo(int pos)
{   
    target = pos;
    if(position < target){
        actualeration = acceleration;
    } else {
        actualeration = -acceleration;
    }
}

void AccelStepper::run()
{
    // std::cout << "RunningMotor" << std::endl;
    if((actualeration < 0 && target > position) || (actualeration > 0 && target < position)){
        // std::cout << "target: " << target << ",  acceleration: " << actualeration << std::endl;
        std::cout << "verkeerde kant op" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        return;
    }
    if((speed < 0 && position+speed <= target) || (speed > 0 && position+speed >= target)){
        std::cout << "at target (also prevent overshooting position)" << std::endl;
        position = target;
        speed = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        return;
    }


    position += speed;
    if(abs(speed) < maxSpeed) {
        speed += actualeration/5;
    }
    std::cout << "position: " << position << ",  taget: " << target << ",   speed: " << speed << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

int AccelStepper::abs(int in)
{
    return in > 0 ? in : -in;
}

int Serial::index = 0;
int Serial::available()
{
    index ++;
    return index < 90;
}

int Serial::read()
{
    char listy[94] = {'"', '.', '!', ',', '/', '+', '.', '-', ' ', '$', '$', '%', '$', '.', '!', '/', '-', ')', '(', '#', '&', '%', '-', '.', '-', '"', '&', ' ', '*', '&', ')', '"', ' ', '*', '$', '*', '.', '$', '*', '+', '!', '+', '-', '"', '!', ' ', ' ', '(', ')', '/', ' ', '"', '*', '*', '*', '%', ' ', ',', '/', ',', '/', '+', '+', '!', '+', ',', '(', '+', ')', '&', ',', ',', '&', '"', '.', '#', '*', ')', ' ', '$', '&', ' ', '#', '-', '(', '*', '#', '!', ' ', '(', '%', '-', '+', '+'};
    return listy[index/2];
}
