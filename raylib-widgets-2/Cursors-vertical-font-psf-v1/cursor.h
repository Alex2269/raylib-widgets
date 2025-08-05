// cursor.h
#ifndef CURSOR_H
#define CURSOR_H

#include "raylib.h"
#include "psf_font.h"
#include <stdbool.h>

#define MAX_ACTIVE_CURSORS 16

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

// Стек активних курсорів (індекси)
typedef struct {
    int indices[MAX_ACTIVE_CURSORS];
    int top; // -1, якщо порожній
} ActiveCursorStack;

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

// Ініціалізація стеку активних курсорів
void InitActiveCursorStack(ActiveCursorStack *stack);

// Додавання індексу у стек
void PushActiveCursor(ActiveCursorStack *stack, int idx);

// Видалення індексу зі стеку
void PopActiveCursor(ActiveCursorStack *stack);

// Перевірка, чи індекс є у стеку
bool IsCursorActive(ActiveCursorStack *stack, int idx);

// Отримання верхнього індексу (активного курсора)
int TopActiveCursor(ActiveCursorStack *stack);

// Оновлення та обробка курсорів (перетягування, прилипання, колізії)
void UpdateAndHandleCursors(CursorArray *arr, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased, ActiveCursorStack *activeStack);

// Малювання курсорів та ліній
void DrawCursorsAndLines(CursorArray *arr, PSF_Font font);

#endif // CURSOR_H

