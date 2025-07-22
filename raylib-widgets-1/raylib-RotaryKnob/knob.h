#ifndef _KNOB_H
#define _KNOB_H

#ifdef __cplusplus
 extern "C" {
#endif

// knob.h
#include <stdint.h>
#include "raylib.h"
#include <math.h>


// Функція для малювання регулятора (knob)
void draw_knob(uint16_t x_pos, uint16_t y_pos, float radius, float knobAngle, int knobValue);
// Функція для обробки регулятора (knob) та повернення значення
int knob_handler(uint16_t x_pos, uint16_t y_pos, float radius, int knobValue, bool *isDragging, float *knobAngle, bool isActive);


#ifdef __cplusplus
}
#endif

#endif /* _KNOB_H */
