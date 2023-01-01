const int ShiftPWM_latchPin=9;
#define SHIFTPWM_NOSPI
const int ShiftPWM_dataPin=7;
const int ShiftPWM_clockPin=8;
const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;

#include <Arduino.h>
#include <ShiftPWM.h>
#include "shiftRegister.h"
#include "setting.h"
#include "midi.h"

extern const bool DEBUG_MODE;
extern Note scheduled[88];

void intitializeRegisters() {
	const char    PWM_FREQUENCY = 75;
	const int     NUM_REGISTERS = 11;
	ShiftPWM.SetAmountOfRegisters(NUM_REGISTERS);
	ShiftPWM.Start(PWM_FREQUENCY, 255);
  // ShiftPWM.PrintInterruptLoad();
}

void activateNote(uint8_t note, uint8_t velocity) {
  // reverse order for shiftpwm output
  note = 87 - note;

  if (Setting::handleNotes == false) { return; }
	if (velocity > 0) {
    if (pressedKeys >= Setting::maxNotes) { return; }
		conformVelocity(&velocity);
  }
	ShiftPWM.SetOne(note, velocity);
  if (DEBUG_MODE) Serial.printf("[%u] ShiftPWM Set: note:%i vel:%i pressedKeys:%i\n", millis(), note, velocity, pressedKeys);
}

void conformVelocity(uint8_t* velocity) {
  char range = 0;
	
  // if (DEBUG_MODE) Serial.printf("[%u] Conformed volume input_midi_velocity:%i -> ", millis(), *velocity);
  if (Setting::volume < 100) {
    range = (Setting::maxPWM - Setting::minPWM) * Setting::volume / 100;
    *velocity = round((*velocity) * range / static_cast<double>(127) + Setting::minPWM);

  } else if (Setting::volume > 100) {
    range = (Setting::maxPWM - Setting::minPWM) * (200 - Setting::volume) / 100;
    *velocity = round((*velocity) * range / static_cast<double>(127) + (Setting::maxPWM - range));
    
  } else {
    *velocity = round((*velocity) * (Setting::maxPWM - Setting::minPWM) / static_cast<double>(127) + Setting::minPWM);
  }
  // if (DEBUG_MODE) Serial.printf("output_pwm_velocity:%i\n", *velocity);
}

void testNotes() {
	for(int note = 0; note < 88; note++) {
		ShiftPWM.SetOne(note, Setting::maxPWM);
		delay(150);
		ShiftPWM.SetOne(note, 0);
	}
}

void resetNotes() {
  int i;

  for (i = 0; i < 88; i++) {
    scheduled[i] = {0, 0, 0, 0, 0, 0, 0};
  }
  ShiftPWM.SetAll(0);
  pressedKeys = 0;
}

void checkSchedule() {
  int i;

  for (i = 0; i < 88; i++) {
    if (scheduled[i].planned && scheduled[i].time_on < millis()) {
      activateNote(i, scheduled[i].velocity_on);
      scheduled[i].planned = 0;
      scheduled[i].triggered = 1;
    }
    if (scheduled[i].triggered && scheduled[i].time_off && scheduled[i].time_off < millis()) {
      activateNote(i, 0);
      scheduled[i].time_stay = 0; 
      scheduled[i].triggered = 0;
    }
    if (scheduled[i].time_stay && scheduled[i].time_stay < millis()) {
      activateNote(i, scheduled[i].velocity_stay);
      scheduled[i].time_stay = 0;
    }
  }

  // if no notes triggered, push reset forward
  if (pressedKeys == 0) {
		nextReset = millis() + Setting::autoReset * 1000;
  }

}
