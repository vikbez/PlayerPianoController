#include <EEPROM.h>
#include "setting.h"
#include "lcd.h"
#include "input.h"

//settings within the program
unsigned long nextReset   = 0;
unsigned char pressedKeys = 0;

// settings changed by the control box
namespace Setting {
  bool          handleNotes      = true;
  bool          scheduleNotes    = true;
  unsigned char minPWM           = 110;
  unsigned char maxPWM           = 255;
  unsigned char pressedPWM       = 85;
  unsigned char pressedDelay     = 140;
  unsigned char minNoteTime      = 80;
  unsigned char autoReset        = 20;
  unsigned char maxNotes         = 20;
  unsigned char volume           = 100;
}

void setVolume(int newVolume) {
  Setting::volume = newVolume;
}

void initializeSettings() {
  using namespace Setting;
  const unsigned char defaults[] = {
     1,   1, 110, 255,  85, 140,  80,  20,  20,   0,
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,  42
  };

  // init settings if needed
  if (EEPROM.read(31) != 42) {
    for (int i = 0; i < 32; i++) {
      EEPROM.write(i, defaults[i]);
    }
  }

  handleNotes      = EEPROM.read(0);
  scheduleNotes    = EEPROM.read(1);
  minPWM           = EEPROM.read(2);
  maxPWM           = EEPROM.read(3);
  pressedPWM       = EEPROM.read(4);
  pressedDelay     = EEPROM.read(5);
  minNoteTime      = EEPROM.read(6);
  autoReset        = EEPROM.read(7);
  maxNotes         = EEPROM.read(8);
  // testNotes        = EEPROM.read(9); no data needed
  volume           = 100;
}


void updateSetting(int setting, int value) {
  using namespace SettingID;
  using namespace Setting;

  switch (setting) {
    case HANDLE_NOTES:
      handleNotes = value;
      break;
    case SCHEDULE_NOTES:
      scheduleNotes = value;
      break;
    case MIN_PWM:
      minPWM = value;
      break;
    case MAX_PWM:
      maxPWM = value;
      break;
    case PRESSED_PWM:
      pressedPWM = value;
      break;
    case PRESSED_DELAY:
      pressedDelay = value;
      break;
    case MIN_NOTE_TIME:
      minNoteTime = value;
      break;
    case AUTO_RESET:
      autoReset = value;
      break;
    case MAX_NOTES:
      maxNotes = value;
      break;
    default:
      return;
      break;
  }
  EEPROM.write(setting, value);
}

bool menuIsBool(int menu) {
  if (menu == S_SCHEDULE_NOTES ||
      menu == S_HANDLE_NOTES) {
    return true;
  }
  return false;
}

bool menuIsAction(int menu) {
  if (menu == S_TEST_NOTES) {
    return true;
  }
  return false;
}

void changeSetting(int changeBy) {
  if (menuIsBool(currentMenu)) {
    EEPROM.write(currentMenu, !(static_cast<bool>(EEPROM.read(currentMenu))));
  } else {
    EEPROM.write(currentMenu, EEPROM.read(currentMenu) + changeBy);
  }
  updateSetting(currentMenu, EEPROM.read(currentMenu));
}

