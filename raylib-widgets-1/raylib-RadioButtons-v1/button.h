#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

// Функції для малювання кнопок
void DrawButton(Rectangle buttonRect, bool buttonState, const char* text);

// Функції для обробки натискання кнопок
void HandleButtonClick(int buttonIndex, Rectangle buttonRect, bool buttonStates[], int *selectedButton);

#endif
