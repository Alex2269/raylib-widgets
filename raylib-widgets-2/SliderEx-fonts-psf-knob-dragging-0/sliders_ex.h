
// sliders_ex.h

#ifndef SLIDERS_EX_H
#define SLIDERS_EX_H

#include "raylib.h"
#include "psf_font.h"

// Максимальна кількість слайдерів
#define MAX_SLIDERS 10

// Розмір ручки слайдера (ширина або висота залежно від орієнтації)
#define SLIDER_KNOB_SIZE 12

// Структура слайдера з усіма параметрами
typedef struct {
    Rectangle bounds;    // Позиція і розмір слайдера
    float value;         // Поточне значення слайдера
    float minValue;      // Мінімальне значення
    float maxValue;      // Максимальне значення
    bool isVertical;     // Орієнтація: true - вертикальний, false - горизонтальний
    Color baseColor;     // Базовий колір слайдера
    bool isActive;       // Чи активний (перетягується) слайдер
} SliderEx;

// Ініціалізує масив слайдерів (приклад)
void InitSliders(void);

// Оновлює всі слайдери: обробляє введення миші, перетягування
void UpdateSliders(Vector2 mousePos, bool mousePressed, bool mouseDown, bool mouseReleased);

// Малює всі слайдери у порядку масиву (останній зверху)
void DrawSliders(PSF_Font font);

// Повертає поточне значення слайдера за індексом
float Gui_SliderEx(int sliderId, PSF_Font font, const char *textTop, const char *textRight);

#endif // SLIDERS_EX_H

