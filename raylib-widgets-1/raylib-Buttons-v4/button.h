#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

// Оголошення функцій для малювання та обробки натискання кожної кнопки
void DrawButton1(Rectangle buttonRect, bool buttonState);
void DrawButton2(Rectangle buttonRect, bool buttonState);
void DrawButton3(Rectangle buttonRect, bool buttonState);
void DrawButton4(Rectangle buttonRect, bool buttonState);

void HandleButton1Click(Rectangle buttonRect, bool *buttonState);
void HandleButton2Click(Rectangle buttonRect, bool *buttonState);
void HandleButton3Click(Rectangle buttonRect, bool *buttonState);
void HandleButton4Click(Rectangle buttonRect, bool *buttonState);

#endif // BUTTON_H
