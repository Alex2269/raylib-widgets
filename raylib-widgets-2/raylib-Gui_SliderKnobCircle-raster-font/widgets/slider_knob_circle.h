// circle_knob_slider.h

#ifndef GUI_CIRCLEKNOBSLIDER_H
#define GUI_CIRCLEKNOBSLIDER_H

#include "raylib.h"

#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "color_utils.h"
#include "glyphs.h"

/**
 * @brief Реєструє слайдер з круглою ручкою або оновлює параметри існуючого.
 *
 * Використовується для додавання або оновлення слайдера з круглою ручкою.
 * Підтримується вертикальна або горизонтальна орієнтація.
 *
 * @param sliderIndex Індекс слайдера в масиві [0..MAX_SLIDERS-1]
 * @param bounds Прямокутна область розміщення слайдера (позиція і розмір)
 * @param value Вказівник на float, що зберігає значення слайдера
 * @param minValue Мінімальне значення слайдера
 * @param maxValue Максимальне значення слайдера
 * @param isVertical Орієнтація слайдера: true - вертикальний, false - горизонтальний
 * @param baseColor Основний колір елементів слайдера
 * @param textTop Текст над слайдером (може бути NULL)
 * @param textRight Текст праворуч від слайдера (може бути NULL)
 */
void Gui_SliderKnobCircle(int sliderIndex, Rectangle bounds,
                              RasterFont font, int spacing,
                              const char* textTop, const char* textRight,
                              float *value, float minValue, float maxValue,
                              bool isVertical, Color baseColor);

/**
 * @brief Оновлює стан усіх слайдерів з круглою ручкою і малює їх.
 *
 * Обробляє введення миші (натискання, переміщення, відпускання),
 * оновлює активний слайдер і малює всі слайдери, їх ручки,
 * текстові підказки і значення.
 *
 * @param font Шрифт PSF для відображення тексту підказок та значень
 * @param spacing Відстань між символами для вирівнювання тексту
 */
void UpdateCircleKnobSlidersAndDraw(RasterFont font, int spacing);

#endif // GUI_CIRCLEKNOBSLIDER_H

