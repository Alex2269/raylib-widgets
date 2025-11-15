// dv_knob.c

#include "dv_knob.h"
#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846f                // Константа π для обчислень кута в радіанах
#define INNER_TO_OUTER_RATIO 10.0f                 // Кратність обертів: внутрішня ручка обертається у 10 разів швидше за зовнішню
#define OUTER_INITIAL_ANGLE_DEG -90.0f             // Початковий (базовий) кут зовнішньої ручки (щоб 0° знаходився вгорі кола)
#define INNER_INITIAL_ANGLE_DEG -90.0f             // Початковий кут внутрішньої ручки (також 0° вгорі кола)

// Локальна функція для нормалізації кута в межах [0,360) градусів.
// Потрібна, щоб обробка кутів була коректною при переході через 0/360 градусів
static float AngleForRender(float angle) {
    float a = fmodf(angle, 360.0f);
    if (a < 0) a += 360.0f;
    return a;
}

/**
 * Ініціалізує подвійний верньєрний регулятор з двома ручками.
 * Встановлює позицію, радіуси, початкові кути та початковий стан.
 *
 * @param x Координата X центра регулятора
 * @param y Координата Y центра регулятора
 * @param outerRadius Радіус зовнішньої ручки (внутрішня автоматично = 0.45 * outerRadius)
 * @return DualVernierKnob Відініціалізований регулятор
 */
DualVernierKnob InitDualVernierKnob(uint16_t x, uint16_t y, float outerRadius) {
    DualVernierKnob knob;
    knob.x = x;
    knob.y = y;
    knob.outerRadius = outerRadius;
    knob.innerRadius = outerRadius * 0.45f;    // Внутрішня ручка приблизно вдвічі менша за зовнішню
    knob.outerAngle = 0.0f;                    // Початковий кут зовнішньої ручки, 0° спрямовано вгору
    knob.innerAngle = 0.0f;                    // Початковий кут внутрішньої ручки, також 0°
    knob.prevOuterAngle = knob.outerAngle;    // Попередній кут для відстеження зміни кута (4 плавність руху)
    knob.prevInnerAngle = knob.innerAngle;
    knob.value = 0;                            // Початкове значення регулятора
    knob.isOuterDragging = false;              // Прапорець перетягування зовнішньої ручки мишею
    knob.isInnerDragging = false;              // Прапорець перетягування внутрішньої ручки мишею
    return knob;
}

/**
 * Малює подвійний верньєрний регулятор на екрані.
 * Відображає зовнішню та внутрішню ручки, їх індикатори кута,
 * а також числове значення регулятора.
 *
 * @param knob Структура регулятора для відображення
 */
void DrawDualVernierKnob(DualVernierKnob knob) {
    Vector2 center = {(float)knob.x, (float)knob.y};

    // Малюємо зовнішню ручку (великий круг із двома шарами для об'єму)
    DrawCircleV(center, knob.outerRadius, LIGHTGRAY);
    DrawCircleV(center, knob.outerRadius - 5, DARKGRAY);

    // Обчислюємо кут зовнішньої ручки для візуалізації:
    // Кут нормалізовано, потім зміщено на OUTER_INITIAL_ANGLE_DEG (-90°),
    // щоб кут 0° відповідав положенню вгору на колі
    float outerAngleRender = AngleForRender(knob.outerAngle);
    float outerRad = (outerAngleRender + OUTER_INITIAL_ANGLE_DEG) * (PI / 180.0f);

    // Позиція указателя (стрілки) зовнішньої ручки
    Vector2 outerIndicator = {
        center.x + cosf(outerRad) * (knob.outerRadius - 10),
        center.y + sinf(outerRad) * (knob.outerRadius - 10)
    };
    DrawLineEx(center, outerIndicator, 4, BLUE);

    // Малюємо внутрішню ручку (менший круг із двома шарами)
    DrawCircleV(center, knob.innerRadius, GRAY);
    DrawCircleV(center, knob.innerRadius - 3, DARKGRAY);

    // Аналогічно обчислення кута внутрішньої ручки з початковим зміщенням
    float innerAngleRender = AngleForRender(knob.innerAngle);
    float innerRad = (innerAngleRender + INNER_INITIAL_ANGLE_DEG) * (PI / 180.0f);

    // Позиція указателя внутрішньої ручки (стрілки)
    Vector2 innerIndicator = {
        center.x + cosf(innerRad) * (knob.innerRadius - 7),
        center.y + sinf(innerRad) * (knob.innerRadius - 7)
    };
    DrawLineEx(center, innerIndicator, 3, RED);

    // Вивід поточного числового значення поруч із регулятором
    char valueText[40];
    sprintf(valueText, "%lld", (long long)knob.value);
    DrawText(valueText, center.x + knob.outerRadius + 15, center.y - 10, 20, BLACK);
}

/**
 * Оновлює кут зовнішньої ручки при перетягуванні мишею.
 * Обчислює зміни кута та коригує внутрішній кут із урахуванням кратності обертів.
 *
 * @param knob Вказівник на структуру регулятора
 * @param mousePos Поточна позиція миші
 */
static void UpdateOuterKnob(DualVernierKnob *knob, Vector2 mousePos) {
    Vector2 center = {(float)knob->x, (float)knob->y};

    if (knob->isOuterDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Обчислення поточного кута миші відносно центра
        float currentOuterAngle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);

        // Обчислення різниці кута від попереднього кадру
        float deltaAngle = currentOuterAngle - knob->prevOuterAngle;

        // Корекція для переходу через 0/360 градусів
        if (deltaAngle > 180.0f) deltaAngle -= 360.0f;
        if (deltaAngle < -180.0f) deltaAngle += 360.0f;

        knob->outerAngle += deltaAngle;                            // Оновлення зовнішнього кута
        knob->innerAngle += deltaAngle * INNER_TO_OUTER_RATIO;     // Оновлення внутрішнього кута пропорційно кратності

        knob->prevOuterAngle = currentOuterAngle;                  // Запам'ятати поточний кут
    }
}

/**
 * Оновлює кут внутрішньої ручки при перетягуванні мишею.
 * Коригує зовнішній кут з урахуванням кратності у зворотньому напрямку.
 *
 * @param knob Вказівник на структуру регулятора
 * @param mousePos Поточна позиція миші
 */
static void UpdateInnerKnob(DualVernierKnob *knob, Vector2 mousePos) {
    Vector2 center = {(float)knob->x, (float)knob->y};

    if (knob->isInnerDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Обчислення поточного кута для внутрішньої ручки
        float currentInnerAngle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);

        // Різниця кута від попереднього кадру
        float deltaAngle = currentInnerAngle - knob->prevInnerAngle;

        // Корекція переходу через 0/360 градусів
        if (deltaAngle > 180.0f) deltaAngle -= 360.0f;
        if (deltaAngle < -180.0f) deltaAngle += 360.0f;

        knob->innerAngle += deltaAngle;
        knob->outerAngle += deltaAngle / INNER_TO_OUTER_RATIO;    // Зовнішня ручка рухається повільніше, на 1/10 від швидкості внутрішньої

        knob->prevInnerAngle = currentInnerAngle;                 // Запам'ятати поточний кут
    }
}

/**
 * Основна функція оновлення стану подвійного верньєрного регулятора.
 * Обробляє введення користувача (перетягування мишею),
 * оновлює кути та обчислює накопичене значення.
 *
 * @param knob Вказівник на структуру регулятора
 * @param isActive Чи активний регулятор (true - активний, false - деактивований)
 * @return int64_t Поточне числове значення регулятора (на основі кута внутрішньої ручки)
 */
int64_t UpdateDualVernierKnob(DualVernierKnob *knob, bool isActive) {
    Vector2 mousePos = GetMousePosition();
    Vector2 center = {(float)knob->x, (float)knob->y};

    // Обчислюємо відстань від курсору до центра, щоб визначити, яку ручку можна тягнути
    float distanceToCenter = sqrtf(powf(mousePos.x - center.x, 2) + powf(mousePos.y - center.y, 2));
    bool isMouseInInnerCircle = distanceToCenter < knob->innerRadius;          // Миша всередині внутрішньої ручки
    bool isMouseInOuterCircle = (distanceToCenter > knob->innerRadius) &&
                                (distanceToCenter < knob->outerRadius);           // Миша всередині зовнішньої ручки

    if (!isActive) {
        // Якщо регулятор неактивний - припинити перетягування
        knob->isOuterDragging = false;
        knob->isInnerDragging = false;
        return knob->value;
    }

    // Починаємо перетягування тієї ручки, по якій клацнули мишкою
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (isMouseInInnerCircle) {
            knob->isInnerDragging = true;
            knob->prevInnerAngle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);
            knob->isOuterDragging = false;    // Забороняємо тягнути дві ручки одночасно
        }
        else if (isMouseInOuterCircle) {
            knob->isOuterDragging = true;
            knob->prevOuterAngle = atan2f(mousePos.y - center.y, mousePos.x - center.x) * (180.0f / PI);
            knob->isInnerDragging = false;
        }
    }

    // При відпусканні миші припиняємо будь-яке перетягування
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        knob->isOuterDragging = false;
        knob->isInnerDragging = false;
    }

    // Оновлюємо кути при активному перетягуванні
    if (knob->isOuterDragging) UpdateOuterKnob(knob, mousePos);
    if (knob->isInnerDragging) UpdateInnerKnob(knob, mousePos);

    // Обчислення числового значення регулятора:
    // Вважаємо, що повний оберт (360°) внутрішньої ручки дорівнює 100 одиницям значення
    knob->value = (int64_t)(knob->innerAngle * (100.0f / 360.0f));

    return knob->value;
}

