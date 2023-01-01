#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "input.h"
#include "lcd.h"
#include "setting.h"
#include "midi.h"
#include "serial.h"
#include "shiftRegister.h"

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// pinout:
// Shift register:
// latch:9 data:7 clock:8
// buttons:
// up:15 down:14 left:20 right:16 reset:17 volume:22
// lcd (i2c):
// sda:18 scl:19

extern const bool DEBUG_MODE = false;
int ledPin = 13;

void setup() {
  if (DEBUG_MODE) Serial.begin(38400);

  pinMode(ledPin, OUTPUT);

  usbMIDI.setHandleNoteOff(onNoteOff);
  usbMIDI.setHandleNoteOn(onNoteOn);
  usbMIDI.setHandleControlChange(onControlChange);

  initializeSettings();
  initializeInputs();
  initializeLCD();
  printHomeScreen();
  intitializeRegisters();

  digitalWrite(ledPin, HIGH);
  delay(300);
  digitalWrite(ledPin, LOW);
  if (DEBUG_MODE) Serial.println("Ready");
}

void loop() {
  checkForInput();
  checkDisplay();
  checkForMidiUSB();
  checkSchedule();

  if (DEBUG_MODE) checkSerial();

  // handle reset
  if (millis() >= nextReset) {
    if (DEBUG_MODE) Serial.printf("[%u] Resetting\n", millis());
		resetNotes();
		nextReset = millis() + Setting::autoReset * 1000;
	}
}
