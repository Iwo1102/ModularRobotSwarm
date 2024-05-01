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
    float secs1Meter = 2.3;
    float secs360Degrees = 0.75;
    float timeToMove = secs1Meter * distance;
    float timeToTurn = secs360Degrees * (distance / 360);
    float timeto90 = secs360Degrees * (90 / 360);
    float timeTo20cm = secs1Meter * 0.2;

    switch (direction) {
        case forward:
            motorForward();
            Serial.printf("time to move: %.2f\r\n", timeToMove);
            delay(timeToMove * 1000);
            motorStop();
            break;
        break;
        case backward:
            motorBackward();
            delay(timeToMove * 1000);
            motorStop();
            break;
        case left:
            motorLeft();
            delay(timeToTurn * 1000);
            motorStop();
            break;
        case right:
            motorRight();
            delay(timeToTurn * 1000);
            motorStop();
            break;
        case stop:
            motorStop();
            break;
        case sweep:
            motorForward();
            delay(timeToMove * 1000);
            motorStop();
            motorRight();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeToMove * 1000);
            motorStop();
            motorRight();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeToMove * 1000);
            motorStop();
            motorRight();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeToMove * 1000);
            motorStop();
            break;
        case perimiter:
            motorForward();
            delay(timeToMove * 1000);
            motorStop();
            motorRight();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeTo20cm * 1000);
            motorStop();
            motorRight();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeToMove * 1000);
            motorStop();
            motorLeft();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeTo20cm * 1000);
            motorStop();
            motorLeft();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeToMove * 1000);
            motorStop();
            motorRight();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeTo20cm * 1000);
            motorStop();
            motorRight();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeToMove * 1000);
            motorStop();
            motorLeft();
            delay(timeto90 * 1000);
            motorStop();
            motorForward();
            delay(timeTo20cm * 1000);
            motorStop();
            motorLeft();
            delay(timeto90 * 1000);
            motorStop();
            break;
        default:
            motorStop();
    }
}

