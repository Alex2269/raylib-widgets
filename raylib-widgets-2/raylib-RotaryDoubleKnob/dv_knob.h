// dv_knob.h

#ifndef _DV_KNOB_H
#define _DV_KNOB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>  // Для типу int64_t з великою розрядністю
#include <stdbool.h>
#include "raylib.h"

// Структура для подвійного регулятора з двома ручками
typedef struct DualVernierKnob {
    uint16_t x;             // Координата центру регулятора по X
    uint16_t y;             // Координата центру регулятора по Y
    float outerRadius;      // Радіус зовнішньої ручки
    float innerRadius;      // Радіус внутрішньої ручки
    float outerAngle;       // Поточний кут зовнішньої ручки (накопичуваний, у градусах)
    float innerAngle;       // Поточний кут внутрішньої ручки (накопичуваний, у градусах)
    float prevOuterAngle;   // Попередній кут зовнішньої ручки (для обчислення зміни кута)
    float prevInnerAngle;   // Попередній кут внутрішньої ручки (для обчислення зміни кута)
    int64_t value;          // Значення регулятора з великим діапазоном (int64_t дозволяє дуже великі значення)
    bool isOuterDragging;   // Чи тягнеться зовнішня ручка в даний момент (перетягування мишею)
    bool isInnerDragging;   // Чи тягнеться внутрішня ручка в даний момент
} DualVernierKnob;

// Ініціалізація структури регулятора із заданою позицією та великим радіусом
DualVernierKnob InitDualVernierKnob(uint16_t x, uint16_t y, float outerRadius);

// Малювання регулятора на екрані
void DrawDualVernierKnob(DualVernierKnob knob);

// Оновлення стану регулятора, обробка введення, обчислення і повернення нового значення
int64_t UpdateDualVernierKnob(DualVernierKnob *knob, bool isActive);

#ifdef __cplusplus
}
#endif

#endif /* _DV_KNOB_H */

