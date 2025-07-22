// dv_knob.c
#include "dv_knob.h"

#include <stdint.h>
#include "raylib.h"
#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846f

// Статичні змінні для накопичення дробової частини зміни кута
static float outerAccumulatedDelta = 0.0f;
static float innerAccumulatedDelta = 0.0f;

// Функція для ініціалізації подвійного регулятора
DualVernierKnob InitDualVernierKnob(uint16_t x, uint16_t y, float outerRadius) {
    DualVernierKnob knob;
    knob.x = x;
    knob.y = y;
    knob.outerRadius = outerRadius;
    knob.innerRadius = outerRadius * 0.6f;
    knob.outerAngle = -90.0f;
    knob.innerAngle = -90.0f;
    knob.prevOuterAngle = 0.0f;
    knob.prevInnerAngle = 0.0f;
    knob.value = 0;
    knob.isOuterDragging = false;
    knob.isInnerDragging = false;
    return knob;
}

// Функція для малювання подвійного регулятора
void DrawDualVernierKnob(DualVernierKnob knob) {
    Vector2 center = { (float)knob.x, (float)knob.y };

    // Малюємо зовнішню ручку (десятки)
    DrawCircleV(center, knob.outerRadius, LIGHTGRAY);
    DrawCircleV(center, knob.outerRadius - 5, DARKGRAY);

    // Малюємо індикатор для зовнішньої ручки
    float outerRad = (knob.outerAngle - 360.0f) * (PI / 180.0f);
    Vector2 outerIndicator = { center.x + cosf(outerRad) * (knob.outerRadius - 10), center.y + sinf(outerRad) * (knob.outerRadius - 10) };
    DrawLineEx(center, outerIndicator, 4, BLUE);

    // Малюємо внутрішню ручку (одиниці)
    DrawCircleV(center, knob.innerRadius, GRAY);
    DrawCircleV(center, knob.innerRadius - 3, DARKGRAY);

    // Малюємо індикатор для внутрішньої ручки
    float innerRad = (knob.innerAngle - 360.0f) * (PI / 180.0f);
    Vector2 innerIndicator = { center.x + cosf(innerRad) * (knob.innerRadius - 7), center.y + sinf(innerRad) * (knob.innerRadius - 7) };
    DrawLineEx(center, innerIndicator, 3, RED);

    // Відображення значення
    char valueText[10];
    sprintf(valueText, "%d", knob.value);
    DrawText(valueText, center.x + knob.outerRadius + 10, center.y - 10, 20, BLACK);
}

// Функція для обробки зовнішньої ручки
static void UpdateOuterKnob(DualVernierKnob *knob, Vector2 mousePos) {
    Vector2 center = { (float)knob->x, (float)knob->y };
    if (knob->isOuterDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float currentOuterAngle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);
        float deltaAngle = currentOuterAngle - knob->prevOuterAngle;

        if (deltaAngle > 180.0f) deltaAngle -= 360.0f;
        if (deltaAngle < -180.0f) deltaAngle += 360.0f;

        knob->outerAngle = currentOuterAngle;
        knob->prevOuterAngle = currentOuterAngle;

        outerAccumulatedDelta += deltaAngle / 3.60f;
        int change = (int)outerAccumulatedDelta;
        knob->value += change * 10; // Змінюємо десятки
        outerAccumulatedDelta -= change;
    }
}

// Функція для обробки внутрішньої ручки
static void UpdateInnerKnob(DualVernierKnob *knob, Vector2 mousePos) {
    Vector2 center = { (float)knob->x, (float)knob->y };
    if (knob->isInnerDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        float currentInnerAngle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);
        float deltaAngle = currentInnerAngle - knob->prevInnerAngle;

        if (deltaAngle > 180.0f) deltaAngle -= 360.0f;
        if (deltaAngle < -180.0f) deltaAngle += 360.0f;

        knob->innerAngle = currentInnerAngle;
        knob->prevInnerAngle = currentInnerAngle;

        innerAccumulatedDelta += deltaAngle / 7.20f;
        int change = (int)innerAccumulatedDelta;
        knob->value += change; // Змінюємо одиниці
        // knob->value = knob->value % 100; // Обмежуємо значення до 0-99 (за потреби)
        innerAccumulatedDelta -= change;
    }
}

// Функція для обробки подвійного регулятора та повернення значення
int UpdateDualVernierKnob(DualVernierKnob *knob, bool isActive) {
    Vector2 mousePos = GetMousePosition();
    Vector2 center = { (float)knob->x, (float)knob->y };
    float distanceToCenter = sqrtf(powf(mousePos.x - center.x, 2) + powf(mousePos.y - center.y, 2));
    bool isMouseInInnerCircle = distanceToCenter < knob->innerRadius;
    bool isMouseInOuterCircle = distanceToCenter < knob->outerRadius;

    if (!isActive) {
        knob->isOuterDragging = false;
        knob->isInnerDragging = false;
        return knob->value;
    }

    // Обробка натискання для початку перетягування
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (isMouseInInnerCircle) {
            knob->isInnerDragging = true;
            knob->prevInnerAngle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);
            knob->isOuterDragging = false; // Переконаємося, що інший стан вимкнено
        } else if (isMouseInOuterCircle) {
            knob->isOuterDragging = true;
            knob->prevOuterAngle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);
            knob->isInnerDragging = false; // Переконаємося, що інший стан вимкнено
        }
    }

    // Обробка відпускання кнопки миші для завершення перетягування
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        knob->isOuterDragging = false;
        knob->isInnerDragging = false;
    }

    // Оновлення ручок, якщо їх стан перетягування активний
    if (knob->isOuterDragging) {
        UpdateOuterKnob(knob, mousePos);
    }
    if (knob->isInnerDragging) {
        UpdateInnerKnob(knob, mousePos);
    }

    return knob->value;
}

