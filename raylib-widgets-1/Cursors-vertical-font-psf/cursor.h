// cursor.h
#ifndef CURSOR_H
#define CURSOR_H

#include "raylib.h"
#include "psf_font.h"
#include <stdbool.h>

// Структура курсора (ручки слайдера)
typedef struct {
    float x;          // Фіксована позиція по X (справа)
    float y;          // Поточна позиція по Y (рухається вертикально)
    float width;      // Ширина ручки (горизонтальна)
    float height;     // Висота ручки (вертикальна)
    Color color;      // Колір ручки
    bool isDragging;  // Чи перетягується зараз
    int value;        // Значення (пропорційне позиції по Y)
    int minValue;     // Мінімальне значення
    int maxValue;     // Максимальне значення
} Cursor;

// Динамічний масив курсорів
typedef struct {
    Cursor *items;
    int count;
    int capacity;
} CursorArray;

// Ініціалізація масиву курсорів
void InitCursorArray(CursorArray *arr, int initialCapacity);

// Звільнення пам'яті масиву курсорів
void FreeCursorArray(CursorArray *arr);

// Додавання курсора в масив
void AddCursor(CursorArray *arr, Cursor cursor);

// Видалення курсора за індексом
void RemoveCursor(CursorArray *arr, int index);

// Ініціалізація курсора
Cursor InitCursor(float fixedX, float startY, float width, float height, Color color, int minValue, int maxValue);

// Оновлення та обробка курсорів (перетягування, прилипання, колізії)
void UpdateAndHandleCursors(CursorArray *arr, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased);

// Малювання курсорів та ліній
void DrawCursorsAndLines(CursorArray *arr, PSF_Font font);

#endif // CURSOR_H

