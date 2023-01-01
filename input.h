#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

#define UP_PIN     15
#define DOWN_PIN   14
#define LEFT_PIN   20
#define RIGHT_PIN  16
#define RESET_PIN  17
#define VOLUME_PIN 22

extern unsigned long lastPressedOverall;

extern int lastAnalog;

void initializeInputs();
void checkForInput();

#endif