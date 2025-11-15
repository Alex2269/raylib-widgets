// cursorv.h

#ifndef CURSORV_H
#define CURSORV_H

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
} CursorVertical;

// Динамічний масив курсорів
typedef struct {
    CursorVertical *items;
    int count;
    int capacity;
} CursorVerticalArray;

// Стек активних курсорів (індекси)
typedef struct {
    int indices[MAX_ACTIVE_CURSORS];
    int top; // -1, якщо порожній
} ActiveCursorVerticalStack;

// Ініціалізація масиву курсорів
void InitCursorVerticalArray(CursorVerticalArray *arr, int initialCapacity);

// Звільнення пам'яті масиву курсорів
void FreeCursorVerticalArray(CursorVerticalArray *arr);

// Додавання курсора в масив
void AddCursorVertical(CursorVerticalArray *arr, CursorVertical cursor);

// Видалення курсора за індексом
void RemoveCursorVertical(CursorVerticalArray *arr, int index);

// Ініціалізація курсора
CursorVertical InitCursorVertical(float fixedX, float startY, float width, float height, Color color, int minValue, int maxValue);

// Ініціалізація стеку активних курсорів
void InitActiveCursorVerticalStack(ActiveCursorVerticalStack *stack);

// Додавання індексу у стек
void PushActiveCursorVertical(ActiveCursorVerticalStack *stack, int idx);

// Видалення індексу зі стеку
void PopActiveCursorVertical(ActiveCursorVerticalStack *stack);

// Перевірка, чи індекс є у стеку
bool IsCursorVerticalActive(ActiveCursorVerticalStack *stack, int idx);

// Отримання верхнього індексу (активного курсора)
int TopActiveCursorVertical(ActiveCursorVerticalStack *stack);

// Оновлення та обробка курсорів (перетягування, прилипання, колізії)
void UpdateAndHandleCursorVerticals(CursorVerticalArray *arr, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased, ActiveCursorVerticalStack *activeStack);

// Малювання курсорів та ліній
void DrawCursorVerticalsAndLines(CursorVerticalArray *arr, PSF_Font font);

#endif // CURSORV_H

