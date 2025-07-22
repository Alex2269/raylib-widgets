// sliders.c
#include "sliders.h"

#define SLIDER_KNOB_SIZE 10

float Gui_Slider(Rectangle bounds, float *value, float minValue, float maxValue, bool isVertical) {
    static bool isActive = false;
    Vector2 mousePos = GetMousePosition();

    // Нормалізоване значення від 0 до 1
    float normValue = (*value - minValue) / (maxValue - minValue);

    bool mouseOver = CheckCollisionPointRec(mousePos, bounds);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOver) {
        isActive = true;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isActive = false;
    }

    if (isActive) {
        if (isVertical) {
            normValue = (mousePos.y - bounds.y) / bounds.height;
        } else {
            normValue = (mousePos.x - bounds.x) / bounds.width;
        }
        if (normValue < 0) normValue = 0;
        if (normValue > 1) normValue = 1;

        *value = minValue + normValue * (maxValue - minValue);
    }

    DrawRectangleRoundedLines(bounds, 0.3f, 8, GRAY);

    Color knobColor = isActive ? RED : SKYBLUE;

    if (isVertical) {
        float knobY = bounds.y + normValue * bounds.height;
        DrawRectangle(bounds.x, knobY - SLIDER_KNOB_SIZE / 2, bounds.width, SLIDER_KNOB_SIZE, knobColor);
    } else {
        float knobX = bounds.x + normValue * bounds.width;
        DrawRectangle(knobX - SLIDER_KNOB_SIZE / 2, bounds.y, SLIDER_KNOB_SIZE, bounds.height, knobColor);
    }

    // Тепер функція нічого не повертає, бо оновлює значення через вказівник
    return 0; // Можна змінити на void, якщо не потрібен return
}
