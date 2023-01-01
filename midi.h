#ifndef MIDI_H
#define MIDI_H

#include <Arduino.h>

#define NOTE_OFF_HEADER       8
#define NOTE_ON_HEADER        9
#define CONTROL_CHANGE_HEADER 11
#define SUSTAIN_STATUS_BYTE   64
#define MIN_NOTE_PITCH        21

struct Note {
  byte planned;
  byte triggered;
  byte velocity_on;
  byte velocity_stay;
  unsigned long time_on;
  unsigned long time_stay;
  unsigned long time_off;
};

void onNoteOn(byte channel, byte note, byte velocity);
void onNoteOff(byte channel, byte note, byte velocity);
void onControlChange(byte channel, byte control, byte value);
void checkForMidiUSB();

#endif