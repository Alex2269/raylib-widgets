// cursorv.c

#include "cursorv.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define STICKY_DISTANCE 10.0f // Відстань прилипання курсора до миші в пікселях

// Зовнішні змінні для відступів у PSF-шрифті (визначені у вашому коді)
extern int spacing;    // Відступ між символами
extern int LineSpacing; // Відступ між рядками

// Ініціалізація масиву курсорів з початковою ємністю
void InitCursorVerticalArray(CursorVerticalArray *arr, int initialCapacity) {
    arr->items = (CursorVertical *)malloc(sizeof(CursorVertical) * initialCapacity);
    arr->count = 0;
    arr->capacity = initialCapacity;
}

// Звільнення пам'яті масиву курсорів
void FreeCursorVerticalArray(CursorVerticalArray *arr) {
    if (arr->items) {
        free(arr->items);
        arr->items = NULL;
    }
    arr->count = 0;
    arr->capacity = 0;
}

// Додавання курсора в масив (з розширенням пам'яті при потребі)
void AddCursorVertical(CursorVerticalArray *arr, CursorVertical cursor) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->items = (CursorVertical *)realloc(arr->items, sizeof(CursorVertical) * arr->capacity);
    }
    arr->items[arr->count++] = cursor;
}

// Видалення курсора за індексом (зсув елементів)
void RemoveCursorVertical(CursorVerticalArray *arr, int index) {
    if (index < 0 || index >= arr->count) return;
    for (int i = index; i < arr->count - 1; i++) {
        arr->items[i] = arr->items[i + 1];
    }
    arr->count--;
}

// Ініціалізація курсора з параметрами
CursorVertical InitCursorVertical(float fixedX, float startY, float width, float height, Color color, int minValue, int maxValue) {
    CursorVertical cursor;
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

// --- ActiveCursorVerticalStack функції ---

void InitActiveCursorVerticalStack(ActiveCursorVerticalStack *stack) {
    stack->top = -1;
}

void PushActiveCursorVertical(ActiveCursorVerticalStack *stack, int idx) {
    if (stack->top < MAX_ACTIVE_CURSORS - 1) {
        stack->indices[++stack->top] = idx;
    }
}

void PopActiveCursorVertical(ActiveCursorVerticalStack *stack) {
    if (stack->top >= 0) {
        stack->top--;
    }
}

bool IsCursorVerticalActive(ActiveCursorVerticalStack *stack, int idx) {
    for (int i = 0; i <= stack->top; i++) {
        if (stack->indices[i] == idx) return true;
    }
    return false;
}

int TopActiveCursorVertical(ActiveCursorVerticalStack *stack) {
    if (stack->top >= 0) return stack->indices[stack->top];
    return -1;
}

// --- Внутрішні допоміжні функції ---

// Перевірка, чи миша в зоні прилипання до курсора
static bool IsMouseNearCursorVertical(Vector2 mousePos, CursorVertical cursor, float stickyDistance) {
    return (mousePos.x > cursor.x - cursor.width - stickyDistance &&
            mousePos.x < cursor.x + stickyDistance &&
            mousePos.y > cursor.y - cursor.height / 2 - stickyDistance &&
            mousePos.y < cursor.y + cursor.height / 2 + stickyDistance);
}

// Оновлення позиції курсора з урахуванням колізій з іншими курсорами
static void UpdateCursorVerticalDrag(CursorVertical *cursor, Vector2 mousePos, CursorVerticalArray *arr, int currentIndex) {
    float minY = cursor->height / 2;
    float maxY = GetScreenHeight() - cursor->height / 2;

    float desiredY = mousePos.y;

    if (desiredY < minY) desiredY = minY;
    if (desiredY > maxY) desiredY = maxY;

    // Перевірка сусідніх курсорів, щоб не перестрибнути через них
    for (int i = 0; i < arr->count; i++) {
        if (i == currentIndex) continue;

        CursorVertical *other = &arr->items[i];

        // Якщо курсори мають однаковий Y, дозволяємо це
        // Але не дозволяємо рухатися "через" сусіда з більшим індексом
        if (desiredY == other->y) {
            // Якщо currentIndex менший, не даємо йому "переступити" сусіда
            if (currentIndex < i && desiredY > other->y) {
                desiredY = other->y;
            }
            // Якщо currentIndex більший, не даємо йому "переступити" сусіда
            else if (currentIndex > i && desiredY < other->y) {
                desiredY = other->y;
            }
        }
        else {
            // Якщо відстань менша мінімальної (наприклад 1 піксель), коригуємо
            float minDist = 1.0f;
            if (fabs(desiredY - other->y) < minDist) {
                if (desiredY > other->y) {
                    desiredY = other->y + minDist;
                } else {
                    desiredY = other->y - minDist;
                }
            }
        }
    }

    cursor->y = desiredY;

    cursor->value = cursor->minValue + (cursor->maxValue - cursor->minValue) * ((cursor->y - minY) / (maxY - minY));
}

// --- Головна функція оновлення і обробки курсорів ---

void UpdateAndHandleCursorVerticals(CursorVerticalArray *arr, Vector2 mousePos,
                            bool mouseButtonPressed, bool mouseButtonDown,
                            bool mouseButtonReleased, ActiveCursorVerticalStack *activeStack) {
    if (mouseButtonPressed) {
        // Перевіряємо курсори у зворотному порядку (зверху вниз)
        for (int i = arr->count - 1; i >= 0; i--) {
            if (IsMouseNearCursorVertical(mousePos, arr->items[i], STICKY_DISTANCE)) {
                if (!IsCursorVerticalActive(activeStack, i)) {
                    // Додаємо курсор у стек активних (LIFO)
                    PushActiveCursorVertical(activeStack, i);
                    arr->items[i].isDragging = true;

                    // Переміщаємо курсор у кінець масиву (щоб малювався зверху)
                    CursorVertical temp = arr->items[i];
                    for (int j = i; j < arr->count - 1; j++) {
                        arr->items[j] = arr->items[j + 1];
                    }
                    arr->items[arr->count - 1] = temp;

                    // Оновлюємо індекс у стеку активних курсорів (верхній елемент)
                    activeStack->indices[activeStack->top] = arr->count - 1;

                    break; // Захопили один курсор — виходимо
                }
            }
        }
    }

    if (mouseButtonDown) {
        // Перетягуємо лише верхній курсор у стеку (останній доданий)
        int activeIdx = TopActiveCursorVertical(activeStack);
        if (activeIdx >= 0 && activeIdx < arr->count) {
            UpdateCursorVerticalDrag(&arr->items[activeIdx], mousePos, arr, activeIdx);
        }
    }

    if (mouseButtonReleased) {
        // При відпусканні миші знімаємо перетягування з верхнього курсора і видаляємо його зі стеку
        int activeIdx = TopActiveCursorVertical(activeStack);
        if (activeIdx >= 0 && activeIdx < arr->count) {
            arr->items[activeIdx].isDragging = false;
            PopActiveCursorVertical(activeStack);
        }
    }
}

// Малювання курсорів та коротких ліній вліво від них
void DrawCursorVerticalsAndLines(CursorVerticalArray *arr, PSF_Font font) {
    for (int i = 0; i < arr->count; i++) {
        CursorVertical cursor = arr->items[i];

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

