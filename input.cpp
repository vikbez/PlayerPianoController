#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "input.h"
#include "lcd.h"
#include "setting.h"
#include "shiftRegister.h"

extern LiquidCrystal_I2C lcd;

void handleInput(int inputPin);

struct LastPressed {
  unsigned long up     = 0;
  unsigned long down   = 0;
  unsigned long left   = 0;
  unsigned long right  = 0;
  unsigned long reset1 = 0;
};

LastPressed    lastPressed;
unsigned long  lastPressedOverall = 0;
int            lastAnalog;

void initializeInputs() {
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);
  lastAnalog = (1023 - analogRead(VOLUME_PIN)) / 5;
}

void checkForInput() {
  unsigned long ms = millis();
  const int BUTTON_COOLDOWN = 200;
  extern const bool DEBUG_MODE;

  if (digitalRead(UP_PIN) == LOW && lastPressed.up + BUTTON_COOLDOWN <= ms)
    handleInput(UP_PIN);
  else if (digitalRead(DOWN_PIN) == LOW && lastPressed.down + BUTTON_COOLDOWN <= ms)
    handleInput(DOWN_PIN);
  else if (digitalRead(LEFT_PIN) == LOW && lastPressed.left + BUTTON_COOLDOWN <= ms)
    handleInput(LEFT_PIN);
  else if (digitalRead(RIGHT_PIN) == LOW && lastPressed.right + BUTTON_COOLDOWN <= ms)
    handleInput(RIGHT_PIN);
  else if (digitalRead(RESET_PIN) == LOW && lastPressed.reset1 + BUTTON_COOLDOWN <= ms)
    handleInput(RESET_PIN);
  else {
    const int ANALOG_CHANGE_RATE = 5;
    const int ANALOG_WITHIN_RANGE = 20;
    const int ANALOG_CHANGE_TIME = BUTTON_COOLDOWN * 2;
    // conform analog value to 0-200
    int analogValue = (1023 - analogRead(VOLUME_PIN)) / 5;
    // if (DEBUG_MODE) Serial.println(analogValue);
    // if volume has changed enough,
    // if volume hasn't changed too much for it to be a glitch,
    // and if another button hasn't been pressed within a certain period for it to affect the reading
    if ((analogValue >= (lastAnalog + ANALOG_CHANGE_RATE) || analogValue <= (lastAnalog - ANALOG_CHANGE_RATE)) &&
      (analogValue < (lastAnalog + ANALOG_WITHIN_RANGE) && analogValue >(lastAnalog - ANALOG_WITHIN_RANGE)) &&
      lastPressedOverall + ANALOG_CHANGE_TIME <= ms) {

      //conform analog to certain values if they are within range
      if (analogValue <= 105 && analogValue >= 95)
        analogValue = 100;
      else if (analogValue > 195)
        analogValue = 200;
      else if (analogValue < 5)
        analogValue = 0;
      lastAnalog = analogValue;
      handleInput(VOLUME_PIN); //implied that lastAnalog has also been changed
    }
  }
}

void handleInput(int inputPin)
{
  unsigned long ms = millis();
  extern const bool DEBUG_MODE;

  // if (DEBUG_MODE) Serial.printf("Input: %i\n", inputPin);
  switch (inputPin) {
    case UP_PIN:
    case DOWN_PIN:
      // prevent settings from changing while not showing
      if (menuState == MS_SETTINGS) {
        if (inputPin == UP_PIN) {
          lastPressed.up = ms;
        } else {
          lastPressed.down = ms;
        }
        lastPressedOverall = ms;

        if (currentMenu == S_TEST_NOTES) {
          lcd.setCursor(0, 1);
          lcd.print("Testing...      ");
          testNotes();
          lcd.setCursor(0, 1);
          lcd.print("Up/Down to run  ");
        } else {
          changeSetting(inputPin == UP_PIN ? 1 : -1);
        }
        updateDisplay();
      }
      break;

    case LEFT_PIN:
      lastPressed.left = ms;
      lastPressedOverall = ms;
      currentMenu--;
      if (currentMenu < 0)
        currentMenu = NUM_OF_MENUS - 1;
      menuState = MS_SETTINGS;
      updateDisplay();
      break;

    case RIGHT_PIN:
      lastPressed.right = ms;
      lastPressedOverall = ms;
      currentMenu++;
      if (currentMenu >= NUM_OF_MENUS)
        currentMenu = 0;
      menuState = MS_SETTINGS;
      updateDisplay();
      break;

    case RESET_PIN:
      lastPressed.reset1 = ms;
      exitScreen = ms + SPECIAL_MENU_TIMEOUT;
      resetNotes();
      menuState = MS_RESET;
      updateDisplay();
      break;

    case VOLUME_PIN:
      exitScreen = ms + SPECIAL_MENU_TIMEOUT;
      menuState = MS_VOLUME;
      setVolume(lastAnalog);
      updateDisplay();
      break;
  }
}
