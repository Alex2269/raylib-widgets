// cursor.h
#ifndef CURSOR_H
#define CURSOR_H

#include "raylib.h"
#include <stdbool.h>

// Конфігурація за замовчуванням для курсора
#define DEFAULT_CURSOR_WIDTH 10.0f
#define DEFAULT_CURSOR_HEIGHT 30.0f
#define DEFAULT_CURSOR_TOP_Y 50
#define ARROW_SIZE 10

extern int fontSize;
extern Font font;

// Структура для представлення курсора
typedef struct {
    float x;
    float y;
    float topY; // Y-координата верхньої частини курсора
    float width;
    float height;
    Color color;
    bool isDragging;
    int value; // Додаткове значення, пов'язане з позицією курсора
    int minValue;
    int maxValue;
} Cursor;

// Функція для ініціалізації курсора
Cursor InitCursor(float startX, float topY, float width, float height, Color color, int minValue, int maxValue);

void DrawCursorsWithDistance(Cursor cursorA, Cursor cursorB);

// Функція для малювання курсора зверху з лінією вниз
void DrawCursor(Cursor cursor);

// Функція для перевірки, чи знаходиться миша над курсором (акривною зоною)
bool IsMouseOverCursor(Vector2 mousePos, Cursor cursor);

// Функція для оновлення позиції курсора при перетягуванні з урахуванням меж та іншого курсора
void UpdateCursorDrag(Cursor *cursor, Vector2 mousePos, Cursor *otherCursor);

// Функція для отримання значення курсора на основі його позиції
int GetCursorValue(Cursor cursor);

// Функція обробника подій миші для курсора з урахуванням іншого курсора
void HandleCursorMouseInput(Cursor *cursor, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased, Cursor *otherCursor);

void HandleAllCursorsMouseInput(Cursor *cursors, int count, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased);

#endif // CURSOR_H

