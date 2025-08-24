// knob.c
#include "knob.h"

#include <stdint.h>
#include "raylib.h"
#include <math.h>

#define PI 3.14159265358979323846

// Функція для малювання регулятора (knob)
void draw_knob(uint16_t x_pos, uint16_t y_pos, float radius, float knobAngle, int knobValue) {
    Vector2 center = { (float)x_pos, (float)y_pos };

    // Малюємо сам регулятор
    DrawCircleV(center, radius, LIGHTGRAY);  // Зовнішнє коло
    DrawCircleV(center, radius - 5, DARKGRAY);  // Внутрішнє коло

    // Малюємо індикатор (стрілку)
    float rad = (knobAngle - 90.0f) * (PI / 180.0f);
    Vector2 indicator = { center.x + cos(rad) * (radius - 10), center.y + sin(rad) * (radius - 10) };
    DrawLineEx(center, indicator, 4, RED);

    // Малюємо риски на регуляторі
    for (int i = 0; i <= 100; i += 5) {
        float tickAngle = -135.0f + (i / 100.0f) * 270.0f;
        float rad = (tickAngle - 90.0f) * (PI / 180.0f);
        float innerRadius = (i % 10 == 0) ? radius - 8 : radius - 4;
        float outerRadius = radius;

        Vector2 start = { center.x + cos(rad) * innerRadius, center.y + sin(rad) * innerRadius };
        Vector2 end = { center.x + cos(rad) * outerRadius, center.y + sin(rad) * outerRadius };

        DrawLineEx(start, end, 2, BLACK);
    }

    // Відображення значення
    char valueText[10];
    sprintf(valueText, "%d", knobValue);
    DrawText(valueText, center.x + 45, center.y - 20, 20, BLACK);
}

// Функція для обробки регулятора (knob) та повернення значення
int knob_handler(uint16_t x_pos, uint16_t y_pos, float radius, int knobValue, bool *isDragging, float *knobAngle, bool isActive) {
    Vector2 center = { (float)x_pos, (float)y_pos };
    Vector2 mousePos = GetMousePosition();

    if (!isActive) return knobValue;

    // Обчислюємо кут миші щодо центру регулятора
    float angle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);
    angle -= 90.0f + 180.0f;

    if (angle < -180.0f) angle += 360.0f;
    if (angle < -135.0f) angle = -135.0f;
    if (angle > 135.0f) angle = 135.0f;

    // Обробка натискання миші
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointCircle(mousePos, center, radius)) {
        *isDragging = true;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        *isDragging = false;
    }

    if (*isDragging) {
        *knobAngle = angle;

        // Перетворюємо кут в значення від 0 до 100
        knobValue = (int)(((*knobAngle + 135.0f) / 270.0f) * 100.0f);
        if (knobValue < 0) knobValue = 0;
        if (knobValue > 100) knobValue = 100;

        // Дискретизація значення за кроком 5 одиниць
        knobValue = (knobValue / 5) * 5;
    }

    return knobValue;
}
