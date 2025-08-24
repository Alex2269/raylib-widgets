// button.h
#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

// Масив станів кнопок
extern bool buttonStates[10];
// Массив прямокутників для кнопок
extern Rectangle buttons[10];
// массив надписів для кнопок
extern const char* buttonLabels[10];
extern Color buttonColors[10];

// Функція для малювання кнопок
void DrawButton(Rectangle buttonRect, bool buttonState, const char* label, Color color);
// Ініціалізація кнопок вже здійснюється через масиви
void buttons_init(void);
// Виклик малювання кнопок
void DrawButtons(void);
// Обробка натискання на кнопки
void HandleButtonClicks(void);
// Функція для обробки натискання кнопок
void HandleButtonClick(Rectangle buttonRect, bool* buttonState);


#endif // BUTTON_H

