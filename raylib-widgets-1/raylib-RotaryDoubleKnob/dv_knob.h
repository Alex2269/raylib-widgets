// dv_knob.h
#ifndef _DV_KNOB_H
#define _DV_KNOB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "raylib.h"
#include <math.h>

extern int fontSize;
extern Font font;

// Структура для зберігання стану подвійного регулятора
typedef struct DualVernierKnob {
    uint16_t x;
    uint16_t y;
    float outerRadius;
    float innerRadius;
    float outerAngle;
    float innerAngle;
    float prevOuterAngle; // Додано для відстеження попереднього кута зовнішньої ручки
    float prevInnerAngle; // Додано для відстеження попереднього кута внутрішньої ручки
    int value;
    bool isOuterDragging;
    bool isInnerDragging;
} DualVernierKnob;

// Функція для ініціалізації подвійного регулятора
DualVernierKnob InitDualVernierKnob(uint16_t x, uint16_t y, float outerRadius);

// Функція для малювання подвійного регулятора
void DrawDualVernierKnob(DualVernierKnob knob);

// Функція для обробки подвійного регулятора та повернення значення
int UpdateDualVernierKnob(DualVernierKnob *knob, bool isActive);

#ifdef __cplusplus
}
#endif

#endif /* _DV_KNOB_H */

