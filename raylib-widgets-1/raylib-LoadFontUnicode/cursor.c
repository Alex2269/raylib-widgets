// cursor.c
#include "cursor.h"
#include "raylib.h"
#include <math.h> // Для fabs
#include <stddef.h>

#define THIN_LINE 100

// Функція для ініціалізації курсора
Cursor InitCursor(float startX, float topY, float width, float height, Color color, int minValue, int maxValue) {
    Cursor cursor;
    cursor.x = startX;
    cursor.y = topY + height + THIN_LINE; // Початкова y-координата
    cursor.topY = topY;
    cursor.width = width;
    cursor.height = height;
    cursor.color = color;
    cursor.isDragging = false;
    cursor.minValue = minValue;
    cursor.maxValue = maxValue;
    cursor.value = minValue + (maxValue - minValue) * ((startX) / GetScreenWidth()); // Початкове значення
    return cursor;
}


void DrawCursorsWithDistance(Cursor cursorA, Cursor cursorB) {
    float distance = fabs(cursorA.x - cursorB.x);
    float lineY = cursorA.y; // Лінія тепер починається нижче курсорів
    Vector2 middlePoint = {(cursorA.x + cursorB.x) / 2, lineY - 15}; // Трохи вище лінії для тексту

    // Малюємо лінію між курсорами
    DrawLine(cursorA.x, lineY, cursorB.x, lineY, BLACK);

    // --- Стрілка біля червоного курсора ---
    // Визначаємо напрямок: вправо, якщо синій праворуч, і вліво, якщо синій ліворуч
    float arrowAOffset = (cursorB.x > cursorA.x) ? ARROW_SIZE : -ARROW_SIZE;
    // Малюємо стрілку
    DrawTriangle(
        (Vector2){cursorA.x, lineY},
        (Vector2){cursorA.x + arrowAOffset, lineY + arrowAOffset / 2},
        (Vector2){cursorA.x + arrowAOffset, lineY - arrowAOffset / 2},
        BLACK
    );

    // --- Стрілка біля синього курсора ---
    // Визначаємо напрямок: вліво, якщо червоний ліворуч, і вправо, якщо червоний праворуч
    float arrowBOffset = (cursorA.x > cursorB.x) ? ARROW_SIZE : -ARROW_SIZE;
    // Малюємо стрілку
    DrawTriangle(
        (Vector2){cursorB.x, lineY},
        (Vector2){cursorB.x + arrowBOffset, lineY + arrowBOffset / 2},
        (Vector2){cursorB.x + arrowBOffset, lineY - arrowBOffset / 2},
        BLACK
    );

    // Малюємо значення відстані над лінією
    char distanceText[32];
    sprintf(distanceText, "%i px", (int)distance);
    int textWidth = MeasureText(distanceText, fontSize);
    DrawTextEx(font, distanceText, (Vector2) {middlePoint.x - textWidth / 2, middlePoint.y - fontSize - 5}, fontSize, 2, DARKGRAY);
}

// Функція для малювання курсора зверху з лінією вниз
void DrawCursor(Cursor cursor) {
    DrawRectangle(cursor.x - cursor.width / 2, cursor.topY, cursor.width, cursor.height, cursor.color);
    DrawLine(cursor.x, cursor.topY + cursor.height, cursor.x, cursor.topY + cursor.height + THIN_LINE, BLACK); // Тонка лінія вниз
    // Не оновлюємо тут y-координату, оскільки вона залежить від контексту використання
}

// Функція для перевірки, чи знаходиться миша над курсором (акривною зоною)
bool IsMouseOverCursor(Vector2 mousePos, Cursor cursor) {
    return (mousePos.x > cursor.x - cursor.width * 2 && mousePos.x < cursor.x + cursor.width * 2 &&
            mousePos.y > cursor.topY && mousePos.y < cursor.topY + cursor.height);
}

// Функція для оновлення позиції курсора при перетягуванні з урахуванням меж та іншого курсора
void UpdateCursorDrag(Cursor *cursor, Vector2 mousePos, Cursor *otherCursor) {
    cursor->x = mousePos.x;

    // Обмеження горизонтальної позиції курсора межами екрану
    float minX = cursor->width / 2;
    float maxX = GetScreenWidth() - cursor->width / 2;

    // Обмеження позиції відносно іншого курсора
    float collisionThreshold = cursor->width * 1.5f; // Відстань, на якій починається "відштовхування"

    if (otherCursor != NULL && cursor->isDragging) {
        if (cursor == otherCursor) return; // Не порівнюємо курсор сам з собою

        if (cursor->x < otherCursor->x && cursor->x + collisionThreshold > otherCursor->x) {
            cursor->x = otherCursor->x - collisionThreshold;
        } else if (cursor->x > otherCursor->x && cursor->x - collisionThreshold < otherCursor->x) {
            cursor->x = otherCursor->x + collisionThreshold;
        }
    }

    // Застосовуємо загальні межі екрану після перевірки колізій
    if (cursor->x < minX) {
        cursor->x = minX;
    }
    if (cursor->x > maxX) {
        cursor->x = maxX;
    }

    // Оновлюємо значення курсора на основі його горизонтальної позиції
    cursor->value = cursor->minValue + (cursor->maxValue - cursor->minValue) * ((cursor->x - minX) / (maxX - minX));
    // Обмежуємо значення в межах [minValue, maxValue] (за потреби)
    if (cursor->value < cursor->minValue) cursor->value = cursor->minValue;
    if (cursor->value > cursor->maxValue) cursor->value = cursor->maxValue;
    cursor->y = cursor->topY + cursor->height + THIN_LINE; // Оновлюємо y для малювання лінії
}

// Функція для отримання значення курсора на основі його позиції
int GetCursorValue(Cursor cursor) {
    return cursor.value;
}

// Функція обробника подій миші для курсора з урахуванням іншого курсора
void HandleCursorMouseInput(Cursor *cursor, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased, Cursor *otherCursor) {
    if (mouseButtonPressed)
    {
        if (IsMouseOverCursor(mousePos, *cursor))
        {
            cursor->isDragging = true;
        }
    }

    if (cursor->isDragging && mouseButtonDown)
    {
        UpdateCursorDrag(cursor, mousePos, otherCursor);
        if (otherCursor != NULL && otherCursor->isDragging) {
            // Якщо інший курсор також перетягується, оновлюємо і його, щоб уникнути перекриття
            // (Це може бути не потрібним, залежно від бажаної поведінки)
            // UpdateCursorDrag(otherCursor, GetMousePosition(), cursor);
        }
    }

    if (mouseButtonReleased)
    {
        cursor->isDragging = false;
    }
}

