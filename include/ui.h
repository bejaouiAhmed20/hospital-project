#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Color codes
#define COLOR_RESET 7
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_CYAN 11
#define COLOR_RED 12
#define COLOR_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

// Function prototypes
void setColor(int color);
void clearScreen();
void setConsoleEncoding();
void clearInputBuffer();
void waitForKeypress();
void readString(char* buffer, int maxLength);
int displayMainMenu();
void displayWelcomeMessage();

// New function prototypes for colorful menus
void displayMenuHeader(const char* title);
void displayMenuOption(int number, const char* text);
void displayMenuFooter();
int getMenuChoice();
void displayError(const char* message);
void displaySuccess(const char* message);

#endif // UI_H