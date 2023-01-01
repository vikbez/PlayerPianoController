#ifndef SETTING_H
#define SETTING_H

#include <Arduino.h>

#define S_HANDLE_NOTES       0
#define S_SCHEDULE_NOTES     1
#define S_MIN_PWM            2
#define S_MAX_PWM            3
#define S_PRESSED_PWM        4
#define S_PRESSED_DELAY      5
#define S_MIN_NOTE_TIME      6
#define S_AUTO_RESET         7
#define S_MAX_NOTES          8
#define S_TEST_NOTES				 9

// so a using statement can be used in the switch statement
namespace SettingID {
	enum SettingID {
		HANDLE_NOTES,
		SCHEDULE_NOTES,
		MIN_PWM,
		MAX_PWM,
		PRESSED_PWM,
		PRESSED_DELAY,
		MIN_NOTE_TIME,
		AUTO_RESET,
		MAX_NOTES,
		TEST_NOTES
	};
}

extern unsigned long nextReset;
extern unsigned char pressedKeys;

namespace Setting {
	extern bool          handleNotes;
	extern bool          scheduleNotes;
	extern unsigned char minPWM;
	extern unsigned char maxPWM;
	extern unsigned char pressedPWM;
	extern unsigned char pressedDelay;
	extern unsigned char minNoteTime;
	extern unsigned char autoReset;
	extern unsigned char maxNotes;
	// extern bool          testNotes;
	extern unsigned char volume;
}

void setVolume(int newVolume);
void initializeSettings();
void updateSetting(int setting, int value);
bool menuIsBool(int menu);
bool menuIsAction(int menu);
void changeSetting(int changeBy);

#endif