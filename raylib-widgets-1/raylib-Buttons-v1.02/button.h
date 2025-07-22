#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

// Функція для перевірки натискання кнопки
int CheckButtonPress(Vector2 mousePosition, Rectangle buttonRect);

// Функція для малювання кнопки
void DrawButton(Rectangle buttonRect, int isSelected, const char* text);

#endif
