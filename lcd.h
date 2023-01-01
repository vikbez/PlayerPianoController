#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

#define MS_WELCOME 0
#define MS_SETTINGS 1
#define MS_VOLUME 2
#define MS_RESET 3

extern int           menuState;
extern const String  MENU_NAMES[];
extern const int     SPECIAL_MENU_TIMEOUT;
extern const int     NUM_OF_MENUS;
extern int           currentMenu;
extern unsigned long exitScreen;

void initializeLCD();
void printHomeScreen();
void updateDisplay();
void checkDisplay();

#endif