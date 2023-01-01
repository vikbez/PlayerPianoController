#include <Arduino.h>
#include "shiftRegister.h"

char command[32] = {0};
char *commandName;
int args[5] = {0};
bool newData = false;

void repeatNote(int vel, int time_on, int time_off) {
  for (int i = 0; i < 5; i++) {
    activateNote(40, vel);
    delay(time_on);
    activateNote(40, 0);
    delay(time_off);
  }
}

void runCommand() {
  if (newData == true) {
    commandName = strtok(command, " ");
    for (int i = 0; i < 5; i++) {
      args[i] = atoi(strtok(NULL, " "));
    }
    Serial.printf("Parsed command: %s arg0: %i arg1:%i arg2:%i arg3:%i\n", commandName, args[0], args[1], args[2], args[3]);
    newData = false;
    if (strcmp(commandName, "re") == 0) {
      // velocity time_on time_off
      repeatNote(args[0], args[1], args[2]);

    } else if (strcmp(commandName, "no") == 0) {
      // note vel time_on
      activateNote(args[0], args[1]);
      delay(args[2]);
      activateNote(args[0], 0);
    }
  }
}

void readSerial() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
 
  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    
    if (rc != endMarker) {
      command[ndx] = rc;
      ndx++;
      if (ndx >= 32) {
        ndx = 32 - 1;
      }
    } else {
      command[ndx] = '\0';
      ndx = 0;
      newData = true;
    }
  }
}

void checkSerial() {
  readSerial();
  runCommand();
}
