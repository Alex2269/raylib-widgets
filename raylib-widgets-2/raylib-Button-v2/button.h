#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

// Оголошення функцій для малювання та обробки натискання кожної кнопки
bool Gui_Button(Rectangle bounds, const char *text,
                Color colorNormal, Color colorHover, Color colorPressed, Color colorText);

#endif // BUTTON_H
