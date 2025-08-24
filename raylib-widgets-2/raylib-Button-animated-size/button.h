// button.h
#ifndef BUTTON_H
#define BUTTON_H

#include "raylib.h"

// Зовнішні змінні шрифту
extern Font font;
extern int fontSize;
extern int LineSpacing;

typedef struct {
    Rectangle bounds;
    float scale;         // масштаб кнопки (1.0 - нормальний)
    bool pressed;
    float animProgress;  // прогрес анімації від 0 до 1
} AnimatedButton;

void UpdateButton(AnimatedButton *btn);
void DrawButton(AnimatedButton *btn, const char *text, Font font, int fontSize, int lineSpacing);

#endif // BUTTON_H
