// slider.h
#ifndef SLIDER_H
#define SLIDER_H

#include "raylib.h"
#include <stdbool.h>

// Конфігурація за замовчуванням для слайдера
#define DEFAULT_SLIDER_WIDTH 10
#define DEFAULT_SLIDER_HEIGHT 30

// Структура для представлення (ручки слайдера)
typedef struct SliderHandle {
    float x;
    float y;
    float topY;
    float width;
    float height;
    Color color;
    bool isDragging;
    float valueNormalized; // Нормалізоване значення ручки слайдера (від 0.0 до 1.0)
    float minValue;       // Мінімальне значення слайдера
    float maxValue;       // Максимальне значення слайдера
} SliderHandle;

// Структура для представлення слайдера з двома ручками
typedef struct SliderWidget {
    Rectangle bounds;     // Прямокутник, що визначає межі слайдера
    SliderHandle leftHandle;
    SliderHandle rightHandle;
    int leftValue;        // Значення лівої ручки
    int rightValue;       // Значення правої ручки
    int minValue;         // Мінімальне значення слайдера
    int maxValue;         // Максимальне значення слайдера
} SliderWidget;

// Функція для ініціалізації ручки слайдера
SliderHandle InitSliderHandle(float startX, float startY, float width, float height, Color color, float minValue, float maxValue);

// Функція для ініціалізації віджета слайдера
SliderWidget InitSliderWidget(Rectangle bounds, int minValue, int maxValue);

// Функція для малювання ручки слайдера
void DrawSliderHandle(SliderHandle handle);

// Функція для малювання віджета слайдера
void DrawSliderWidget(SliderWidget slider);

// Функція для перевірки, чи знаходиться миша над ручкою слайдера
bool IsMouseOverSliderHandle(Vector2 mousePos, SliderHandle handle);

// Функція для оновлення позиції ручки слайдера при перетягуванні (обмежена межами слайдера)
void UpdateSliderHandleDrag(SliderHandle *handle, Vector2 mousePos, Rectangle sliderBounds);

// Функція для отримання нормалізованого значення ручки слайдера
float GetSliderHandleValueNormalized(SliderHandle handle, Rectangle sliderBounds);

// Функція обробника подій миші для віджета слайдера
void HandleSliderWidgetMouseInput(SliderWidget *slider, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased);

#endif
