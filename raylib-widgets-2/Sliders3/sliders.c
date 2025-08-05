// sliders.c
#include "sliders.h"

#define SLIDER_KNOB_SIZE 10

float GuiSlider(Rectangle bounds, float value, float minValue, float maxValue, bool isVertical) {
    static bool isActive = false;
    Vector2 mousePos = GetMousePosition();

    // Нормалізоване значення від 0 до 1
    float normValue = (value - minValue) / (maxValue - minValue);

    bool mouseOver = CheckCollisionPointRec(mousePos, bounds);

    // Обробка натискання миші
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOver) {
        isActive = true;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isActive = false;
    }

    // Якщо слайдер активний — оновлюємо значення по позиції миші
    if (isActive) {
        if (isVertical) {
            normValue = (mousePos.y - bounds.y) / bounds.height;
        } else {
            normValue = (mousePos.x - bounds.x) / bounds.width;
        }
        normValue = (normValue < 0) ? 0 : (normValue > 1 ? 1 : normValue);
        value = minValue + normValue * (maxValue - minValue);
    }

    // Малюємо слайдер
    DrawRectangleRoundedLines(bounds, 0.3f, 8, GRAY);

    Color knobColor = isActive ? RED : SKYBLUE;

    if (isVertical) {
        float knobY = bounds.y + normValue * bounds.height;
        DrawRectangle(bounds.x, knobY - SLIDER_KNOB_SIZE / 2, bounds.width, SLIDER_KNOB_SIZE, knobColor);
    } else {
        float knobX = bounds.x + normValue * bounds.width;
        DrawRectangle(knobX - SLIDER_KNOB_SIZE / 2, bounds.y, SLIDER_KNOB_SIZE, bounds.height, knobColor);
    }

    return value;
}
