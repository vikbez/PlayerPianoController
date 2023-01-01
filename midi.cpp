#include "midi.h"
#include "shiftRegister.h"
#include "setting.h"

extern const bool DEBUG_MODE;
Note scheduled[88] = {{0,0,0,0,0,0,0}};

/* time schema of a note : 
 *
 * solenoid
 * position
 *  |    ________
 *  |   /        \
 *  |  /          \
 *  | /            \
 *  |__________________ time
 * 
 * pulse
 * pwm
 *  |____ . . . . . . . velocity, varies, max is MAX_PWM
 *  |   |
 *  |   |________ . . . pressedPwm, setting
 *  |           |
 *  |___________|______ time
 *  :   :       :
 *  :   :     note off
 *  :  keepTime
 * note
 *  on
 * 
 */

void onNoteOn(byte channel, byte note, byte velocity) {
  if (DEBUG_MODE) Serial.printf("[%u] Note ON chan:%i note:%i vel:%i\n", millis(), channel, note, velocity);
  note -= 21; // lower octaves a bit

  if (note >= 0 && note < 87) {
    if (Setting::scheduleNotes) {
      unsigned long on_time = millis();
      // planned, triggered, vel-on, vel-stay, time-on, time-stay, time-off
      scheduled[note] = {1, 0, velocity, Setting::pressedPWM, on_time, on_time + Setting::pressedDelay, 0};
    } else {
      activateNote(note, velocity);
    }
    pressedKeys++;
  }
}

void onNoteOff(byte channel, byte note, byte velocity) {
  if (DEBUG_MODE) Serial.printf("[%u] Note OFF chan:%i note:%i vel:%i\n", millis(), channel, note, velocity);
  note -= 21; // lower octaves a bit

  if (note >= 0 && note < 87) {
    if (Setting::scheduleNotes) {
      scheduled[note].time_off = (millis() - scheduled[note].time_on < Setting::minNoteTime) ? scheduled[note].time_on + Setting::minNoteTime : millis();
    } else {
      activateNote(note, 0);
    }
    pressedKeys--;
  }
}

void onControlChange(byte channel, byte control, byte value) {
  // if (DEBUG_MODE) Serial.printf("[%u] Control: chan:%i ctrl:%i val:%i\n", millis(), channel, control, value);

  // handle panic
  if (control == 120 || control == 121 || control == 123) { resetNotes(); }

  // handle sustain pedal
  if (control == 64 && value > 63) { activateNote(87, 127); }
  if (control == 64 && value < 64) { activateNote(87, 0);   }
}

void checkForMidiUSB() {
  usbMIDI.read();
}

