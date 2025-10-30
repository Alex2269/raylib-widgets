// cursor.c

#include "cursor.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define STICKY_DISTANCE 10.0f // Відстань прилипання курсора до миші в пікселях

// Зовнішні змінні для відступів у PSF-шрифті (визначені у вашому коді)
extern int spacing;    // Відступ між символами
extern int LineSpacing; // Відступ між рядками

// Ініціалізація масиву курсорів з початковою ємністю
void InitCursorArray(CursorArray *arr, int initialCapacity) {
    arr->items = (Cursor *)malloc(sizeof(Cursor) * initialCapacity);
    arr->count = 0;
    arr->capacity = initialCapacity;
}

// Звільнення пам'яті масиву курсорів
void FreeCursorArray(CursorArray *arr) {
    if (arr->items) {
        free(arr->items);
        arr->items = NULL;
    }
    arr->count = 0;
    arr->capacity = 0;
}

// Додавання курсора в масив (з розширенням пам'яті при потребі)
void AddCursor(CursorArray *arr, Cursor cursor) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->items = (Cursor *)realloc(arr->items, sizeof(Cursor) * arr->capacity);
    }
    arr->items[arr->count++] = cursor;
}

// Видалення курсора за індексом (зсув елементів)
void RemoveCursor(CursorArray *arr, int index) {
    if (index < 0 || index >= arr->count) return;
    for (int i = index; i < arr->count - 1; i++) {
        arr->items[i] = arr->items[i + 1];
    }
    arr->count--;
}

// Ініціалізація курсора з параметрами
Cursor InitCursor(float fixedX, float startY, float width, float height, Color color, int minValue, int maxValue) {
    Cursor cursor;
    cursor.x = fixedX;
    cursor.y = startY;
    cursor.width = width;
    cursor.height = height;
    cursor.color = color;
    cursor.isDragging = false;
    cursor.minValue = minValue;
    cursor.maxValue = maxValue;
    cursor.value = minValue + (maxValue - minValue) * (startY / (float)GetScreenHeight());
    return cursor;
}

// Перевірка, чи миша знаходиться в зоні прилипання до курсора
static bool IsMouseNearCursor(Vector2 mousePos, Cursor cursor, float stickyDistance) {
    return (mousePos.x > cursor.x - cursor.width - stickyDistance &&
            mousePos.x < cursor.x + stickyDistance &&
            mousePos.y > cursor.y - cursor.height / 2 - stickyDistance &&
            mousePos.y < cursor.y + cursor.height / 2 + stickyDistance);
}

// Оновлення позиції курсора з урахуванням колізій з іншими курсорами
static void UpdateCursorDrag(Cursor *cursor, Vector2 mousePos, CursorArray *arr, int currentIndex) {
    cursor->y = mousePos.y;

    float minY = cursor->height / 2;
    float maxY = GetScreenHeight() - cursor->height / 2;

    // Обмеження руху в межах екрану
    if (cursor->y < minY) cursor->y = minY;
    if (cursor->y > maxY) cursor->y = maxY;

    // Обробка колізій з іншими курсорами (щоб не злипалися)
    for (int i = 0; i < arr->count; i++) {
        if (i == currentIndex) continue;

        Cursor *other = &arr->items[i];
        float minDist = (cursor->height + other->height) / 2;

        if (fabs(cursor->y - other->y) < minDist) {
            if (cursor->y > other->y) {
                cursor->y = other->y + minDist;
                if (cursor->y > maxY) cursor->y = maxY;
                } else {
                cursor->y = other->y - minDist;
                if (cursor->y < minY) cursor->y = minY;
            }
        }
    }

    // Оновлення значення курсора пропорційно позиції
    cursor->value = cursor->minValue + (cursor->maxValue - cursor->minValue) * ((cursor->y - minY) / (maxY - minY));
}

// Оновлення та обробка всіх курсорів (перетягування, прилипання, колізії)
void UpdateAndHandleCursors(CursorArray *arr, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased) {
    for (int i = 0; i < arr->count; i++) {
        Cursor *cursor = &arr->items[i];

        // Початок перетягування, якщо миша в зоні прилипання і кнопка натиснута
        if (mouseButtonPressed && IsMouseNearCursor(mousePos, *cursor, STICKY_DISTANCE)) {
            cursor->isDragging = true;
    }

        // Оновлення позиції під час перетягування
        if (cursor->isDragging && mouseButtonDown) {
            UpdateCursorDrag(cursor, mousePos, arr, i);
    }

        // Завершення перетягування
    if (mouseButtonReleased) {
            cursor->isDragging = false;
        }
    }
}

// Малювання курсорів та коротких ліній вліво від них
void DrawCursorsAndLines(CursorArray *arr, PSF_Font font) {
    for (int i = 0; i < arr->count; i++) {
        Cursor cursor = arr->items[i];

        // Малюємо ручку (прямокутник)
        DrawRectangle(cursor.x - cursor.width, cursor.y - cursor.height / 2, cursor.width, cursor.height, cursor.color);

        // Малюємо коротку горизонтальну лінію вліво від центру ручки
        float lineLength = 20.0f;
        DrawLine(cursor.x - cursor.width, cursor.y, cursor.x - cursor.width - lineLength, cursor.y, LIGHTGRAY);

        // Малюємо значення поруч праворуч від ручки
        char valText[16];
        sprintf(valText, "%i", cursor.value);
        //DrawTextEx(font, valText, (Vector2){cursor.x + 5, cursor.y - fontSize / 2}, fontSize, 1, cursor.color);
        DrawPSFText(font, cursor.x + 5, cursor.y - font.width, valText, spacing, cursor.color);
    }
}

