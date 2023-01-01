#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

// #include <Arduino.h>

extern uint8_t pwmPercent;

void intitializeRegisters();
void activateNote(uint8_t note, uint8_t velocity);
void conformVelocity(uint8_t* velocity);
void testNotes();
void resetNotes();
void checkSchedule();

#endif
