// sliders.h
#ifndef SLIDERS_H
#define SLIDERS_H

#include "raylib.h"

// Функція, що малює слайдер, обробляє введення і повертає оновлене значення
// minValue, maxValue — діапазон значень слайдера
// value — поточне значення (передається і повертається)
// isVertical — орієнтація слайдера
float GuiSlider(Rectangle bounds, float *value, float minValue, float maxValue, bool isVertical);

#endif // SLIDERS_H
