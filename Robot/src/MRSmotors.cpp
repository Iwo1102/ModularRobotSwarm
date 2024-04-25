#include "MRSmotors.h"

void gpioInit() {
    pinMode(motorPins[0], OUTPUT);
    pinMode(motorPins[1], OUTPUT);
    pinMode(motorPins[2], OUTPUT);
    pinMode(motorPins[3], OUTPUT);
    pinMode(motorPowerPin, OUTPUT);
    pinMode(LEDPin, OUTPUT);

    digitalWrite(motorPowerPin, HIGH);
    digitalWrite(LEDPin, HIGH);
}

void motorForward() {
    //Right
    digitalWrite(motorPins[0], HIGH);
    digitalWrite(motorPins[1], LOW);
    //Left
    digitalWrite(motorPins[2], HIGH);
    digitalWrite(motorPins[3], LOW);
}

void motorBackward() {
    //Right
    digitalWrite(motorPins[0], LOW);
    digitalWrite(motorPins[1], HIGH);
    //Left
    digitalWrite(motorPins[2], LOW);
    digitalWrite(motorPins[3], HIGH);
}

void motorLeft() {
    //Right
    digitalWrite(motorPins[0], LOW);
    digitalWrite(motorPins[1], HIGH);
    //Left
    digitalWrite(motorPins[2], HIGH);
    digitalWrite(motorPins[3], LOW);
}

void motorRight() {
    //Right
    digitalWrite(motorPins[0], HIGH);
    digitalWrite(motorPins[1], LOW);
    //Left
    digitalWrite(motorPins[2], LOW);
    digitalWrite(motorPins[3], HIGH);
}

void motorStop() {
    //Right
    digitalWrite(motorPins[0], LOW);
    digitalWrite(motorPins[1], LOW);
    //Left
    digitalWrite(motorPins[2], LOW);
    digitalWrite(motorPins[3], LOW);
}

void move(int direction, float distance) {
    float secs1Meter = 10;
    float secs360Degrees = 10;
    float timeToMove = secs1Meter * distance;
    float timeToTurn = secs360Degrees * (distance / 360);

    switch (direction) {
        case 1:
            motorForward();
            delay(timeToMove * 1000);
            break;
        break;
        case 2:
            motorBackward();
            delay(timeToMove * 1000);
            break;
        case 3:
            motorLeft();
            delay(timeToTurn * 1000);
            break;
        case 4:
            motorRight();
            delay(timeToTurn * 1000);
            break;
        case 5:
            motorStop();
            break;
        default:
            motorStop();
    }
}

