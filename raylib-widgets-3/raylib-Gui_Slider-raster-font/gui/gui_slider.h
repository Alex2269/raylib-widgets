#ifndef GUI_SLIDER_H
#define GUI_SLIDER_H

#include "raylib.h"
#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "glyphs.h"

// Максимальна кількість слайдерів, яких підтримує бібліотека
#define MAX_SLIDERS 16

// Тип значення, яке підтримує слайдер (ціле або дійсне)
typedef enum {
    GUI_SLIDER_INT,
    GUI_SLIDER_FLOAT
} GuiSliderValueType;

typedef enum {
    GUI_SLIDER_HORIZONTAL,
    GUI_SLIDER_VERTICAL
} GuiSliderOrientation;

/**
 * Основна функція віджета слайдера (підтримка int чи float).
 * Малює слайдер і обробляє зміну значення через перетягування та колесо миші.
 *
 * @param id - Ідентифікатор віджета (для внутрішнього стану)
 * @param centerX, centerY - Координати центру слайдера
 * @param width, height - Розміри слайдера
 * @param textLeft, textRight - Тексти над кнопками (тут не використовуються, можна NULL)
 * @param value - Вказівник на змінну int або float для редагування
 * @param minValue, maxValue - Вказівники на мінімальне та максимальне значення
 * @param step - Крок зміни значення при взаємодії
 * @param valueType - Тип значення (int або float)
 * @param baseColor - Основний колір елемента UI
 * @param font - Шрифт PSF для відображення тексту
 * @param spacing - Відстань між символами тексту
 *
 * @return true, якщо значення було змінено
 */
bool Gui_Slider(int id, int centerX, int centerY, int width, int height,
                const char* textLeft, const char* textRight,
                void* value, void* minValue, void* maxValue,
                float step, GuiSliderValueType valueType,
                GuiSliderOrientation orientation,
                Color baseColor, RasterFont font, int spacing);


#endif // GUI_SLIDER_H

