#ifndef MRSMOTORS_H
#define MRSMOTORS_H

#include <Arduino.h>
#include "MRSTasks.h"

const uint8_t motorPins[4] = {33, 25, 26, 27}, motorPowerPin = 12, LEDPin = 14;

void gpioInit();
void motorForward();
void motorBackward();
void motorLeft();
void motorRight();
void motorStop();

void move(int, float);
#endif