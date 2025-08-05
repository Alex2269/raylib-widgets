// slider_widget_ellipse.h

#ifndef SLIDER_WIDGET_TRIANGLE_H
#define SLIDER_WIDGET_TRIANGLE_H

#include "raylib.h"
#include "psf_font.h"

/**
 * @brief Напрямок трикутної ручки слайдера.
 * Трикутник може бути спрямований вліво або вправо.
 */
typedef enum {
    TRIANGLE_LEFT = 0,  ///< Напрямок ручки вліво
    TRIANGLE_RIGHT = 1, ///< Напрямок ручки вправо
} TriangleKnobDirection;

/**
 * @brief Реєструє слайдер з трикутною ручкою або оновлює параметри існуючого.
 *
 * Використовується для додавання нового слайдера в масив слайдерів або оновлення існуючого
 * по індексу. Цей слайдер має ручку у вигляді трикутника, спрямованого вліво або вправо,
 * а також підтримує вертикальну або горизонтальну орієнтацію.
 *
 * @param sliderIndex Індекс слайдера в масиві (має бути в діапазоні [0..MAX_SLIDERS-1])
 * @param bounds Прямокутна область (позиція та розмір) слайдера на екрані
 * @param value Вказівник на float-значення, яке зберігає позицію слайдера
 * @param minValue Мінімальне значення слайдера
 * @param maxValue Максимальне значення слайдера
 * @param isVertical Визначає орієнтацію слайдера: true — вертикальний, false — горизонтальний
 * @param baseColor Базовий колір елементів слайдера (фону, ручки)
 * @param knobDirection Напрямок трикутної ручки (вліво або вправо)
 * @param textTop Текст, який відображається над слайдером (може бути NULL)
 * @param textRight Текст, який відображається праворуч від слайдера (може бути NULL)
 */
void RegisterTriangleKnobSlider(int sliderIndex, Rectangle bounds, float *value, float minValue, float maxValue,
                                bool isVertical, Color baseColor, TriangleKnobDirection knobDirection,
                                const char* textTop, const char* textRight);

/**
 * @brief Оновлює стан усіх слайдерів з трикутними ручками і малює їх на екрані.
 *
 * Ця функція повинна викликатися один раз на кадр у циклі малювання програми.
 * Вона обробляє введення миші — натискання, переміщення та відпускання кнопок,
 * оновлює значення активного слайдера і малює всі слайдери (фони, ручки, підказки).
 *
 * @param font Шрифт PSF, використовуваний для відображення тексту підказок і значень
 * @param spacing Відстань між символами шрифту, для правильного вирівнювання тексту
 */
void UpdateTriangleKnobSlidersAndDraw(PSF_Font font, int spacing);

#endif // SLIDER_WIDGET_TRIANGLE_H

