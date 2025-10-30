// slider.c
#include <stddef.h>
#include "slider.h"
#include "raylib.h"
#include <math.h> // Для функції fabs

// Функція для ініціалізації ручки слайдера
SliderHandle InitSliderHandle(float startX, float startY, float width, float height, Color color, float minValue, float maxValue) {
    SliderHandle handle;
    handle.x = startX;
    handle.y = startY;
    handle.topY = startY;
    handle.width = width;
    handle.height = height;
    handle.color = color;
    handle.isDragging = false;
    handle.valueNormalized = (startX - minValue) / (maxValue - minValue); // Нормалізоване початкове значення
    handle.minValue = minValue;
    handle.maxValue = maxValue;
    return handle;
}

// Функція для ініціалізації віджета слайдера
SliderWidget InitSliderWidget(Rectangle bounds, int minValue, int maxValue) {
    SliderWidget slider;
    slider.bounds = bounds;
    float handleStartY = bounds.y + bounds.height / 2 - DEFAULT_SLIDER_HEIGHT / 2;
    float initialOffset = 5.0f; // Невелике початкове зміщення

    slider.leftHandle = InitSliderHandle(bounds.x + initialOffset, handleStartY, DEFAULT_SLIDER_WIDTH, DEFAULT_SLIDER_HEIGHT, RED, (float)bounds.x, (float)bounds.x + bounds.width);
    slider.rightHandle = InitSliderHandle(bounds.x + bounds.width - initialOffset, handleStartY, DEFAULT_SLIDER_WIDTH, DEFAULT_SLIDER_HEIGHT, BLUE, (float)bounds.x, (float)bounds.x + bounds.width);
    slider.leftValue = minValue;
    slider.rightValue = maxValue;
    slider.minValue = minValue;
    slider.maxValue = maxValue;
    return slider;
}

// Функція для малювання ручки слайдера
void DrawSliderHandle(SliderHandle handle) {
    DrawRectangle(handle.x - handle.width / 2, handle.y, handle.width, handle.height, handle.color);
}

// Функція для малювання віджета слайдера
void DrawSliderWidget(SliderWidget slider) {
    DrawRectangleRec(slider.bounds, DARKGRAY);
    // Малюємо фон повзунка
    // DrawRectangleRoundedLines(slider.bounds, 2.0f, 10, SKYBLUE);
    DrawRectangleRoundedLinesEx(slider.bounds, 0.25f, 10, 4, SKYBLUE);
    DrawLine(slider.leftHandle.x, slider.bounds.y + slider.bounds.height / 2, slider.rightHandle.x, slider.bounds.y + slider.bounds.height / 2, BLACK);
    DrawSliderHandle(slider.leftHandle);
    DrawSliderHandle(slider.rightHandle);
}

// Функція для перевірки, чи знаходиться миша над ручкою слайдера
bool IsMouseOverSliderHandle(Vector2 mousePos, SliderHandle handle) {
    return (mousePos.x > handle.x - handle.width / 2 && mousePos.x < handle.x + handle.width / 2 &&
            mousePos.y > handle.y && mousePos.y < handle.y + handle.height);
}

// Функція для оновлення позиції ручки слайдера при перетягуванні (обмежена межами слайдера)
void UpdateSliderHandleDrag(SliderHandle *handle, Vector2 mousePos, Rectangle sliderBounds) {
    handle->x = mousePos.x;

    // Обмеження горизонтальної позиції ручки межами слайдера
    if (handle->x < sliderBounds.x) {
        handle->x = sliderBounds.x;
    }
    if (handle->x > sliderBounds.x + sliderBounds.width) {
        handle->x = sliderBounds.x + sliderBounds.width;
    }

    // Оновлюємо нормалізоване значення ручки
    handle->valueNormalized = (handle->x - sliderBounds.x) / sliderBounds.width;
}

// Функція для отримання нормалізованого значення ручки слайдера
float GetSliderHandleValueNormalized(SliderHandle handle, Rectangle sliderBounds) {
    return (handle.x - sliderBounds.x) / sliderBounds.width;
}

// Функція обробника подій миші для віджета слайдера
void HandleSliderWidgetMouseInput(SliderWidget *slider, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased) {
    static SliderHandle *activeHandle = NULL;
    float minDistance = 5.0f; // Мінімальна відстань між курсорами

    if (mouseButtonPressed) {
        if (IsMouseOverSliderHandle(mousePos, slider->leftHandle)) {
            activeHandle = &slider->leftHandle;
            activeHandle->isDragging = true;
        } else if (IsMouseOverSliderHandle(mousePos, slider->rightHandle)) {
            activeHandle = &slider->rightHandle;
            activeHandle->isDragging = true;
        }
    }

    if (activeHandle != NULL && activeHandle->isDragging && mouseButtonDown) {
        UpdateSliderHandleDrag(activeHandle, mousePos, slider->bounds);

        // Оновлюємо значення слайдера на основі положення ручок
        float leftNormalized = GetSliderHandleValueNormalized(slider->leftHandle, slider->bounds);
        float rightNormalized = GetSliderHandleValueNormalized(slider->rightHandle, slider->bounds);

        slider->leftValue = slider->minValue + (int)(leftNormalized * (slider->maxValue - slider->minValue));
        slider->rightValue = slider->minValue + (int)(rightNormalized * (slider->maxValue - slider->minValue));

        // Забезпечуємо мінімальну відстань між ручками
        if (slider->rightHandle.x - slider->leftHandle.x < minDistance) {
            if (activeHandle == &slider->leftHandle) {
                slider->leftHandle.x = slider->rightHandle.x - minDistance;
                if (slider->leftHandle.x < slider->bounds.x) slider->leftHandle.x = slider->bounds.x;
            } else if (activeHandle == &slider->rightHandle) {
                slider->rightHandle.x = slider->leftHandle.x + minDistance;
                if (slider->rightHandle.x > slider->bounds.x + slider->bounds.width) slider->rightHandle.x = slider->bounds.x + slider->bounds.width;
            }
            // Оновлюємо значення після коригування позиції
            leftNormalized = GetSliderHandleValueNormalized(slider->leftHandle, slider->bounds);
            rightNormalized = GetSliderHandleValueNormalized(slider->rightHandle, slider->bounds);
            slider->leftValue = slider->minValue + (int)(leftNormalized * (slider->maxValue - slider->minValue));
            slider->rightValue = slider->minValue + (int)(rightNormalized * (slider->maxValue - slider->minValue));
        }
    }

    if (mouseButtonReleased) {
        if (activeHandle != NULL) {
            activeHandle->isDragging = false;
            activeHandle = NULL;
        }
    }
}

