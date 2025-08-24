#include "raylib.h"

typedef struct Slider {
    Rectangle bar;       // область слайдера
    Rectangle knob;      // область повзунка
    float minValue;
    float maxValue;
    float value;
    bool vertical;
    bool dragging;
} Slider;

void SliderInit(Slider *slider, float x, float y, float length, float thickness, float minValue, float maxValue, bool vertical) {
    slider->vertical = vertical;
    slider->minValue = minValue;
    slider->maxValue = maxValue;
    slider->value = minValue;
    slider->dragging = false;

    if (vertical) {
        slider->bar = (Rectangle){ x, y, thickness, length };
        slider->knob = (Rectangle){ x - thickness / 2, y, thickness * 2, thickness };
    } else {
        slider->bar = (Rectangle){ x, y, length, thickness };
        slider->knob = (Rectangle){ x, y - thickness / 2, thickness, thickness * 2 };
    }
}

void SliderUpdate(Slider *slider) {
    Vector2 mousePoint = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePoint, slider->knob)) {
            slider->dragging = true;
        }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        slider->dragging = false;
    }

    if (slider->dragging) {
        if (slider->vertical) {
            float pos = mousePoint.y;
            if (pos < slider->bar.y) pos = slider->bar.y;
            if (pos > slider->bar.y + slider->bar.height) pos = slider->bar.y + slider->bar.height;
            slider->knob.y = pos - slider->knob.height / 2;

            float t = (slider->knob.y + slider->knob.height / 2 - slider->bar.y) / slider->bar.height;
            slider->value = slider->maxValue - t * (slider->maxValue - slider->minValue);
        } else {
            float pos = mousePoint.x;
            if (pos < slider->bar.x) pos = slider->bar.x;
            if (pos > slider->bar.x + slider->bar.width) pos = slider->bar.x + slider->bar.width;
            slider->knob.x = pos - slider->knob.width / 2;

            float t = (slider->knob.x + slider->knob.width / 2 - slider->bar.x) / slider->bar.width;
            slider->value = slider->minValue + t * (slider->maxValue - slider->minValue);
        }
    }
}

void SliderDraw(Slider *slider) {
    DrawRectangleRec(slider->bar, LIGHTGRAY);
    DrawRectangleRec(slider->knob, DARKGRAY);
}

int main(void) {
    InitWindow(800, 450, "raylib slider example");

    Slider hSlider, vSlider;
    SliderInit(&hSlider, 100, 200, 300, 10, 0, 100, false); // горизонтальний
    SliderInit(&vSlider, 500, 100, 200, 10, 0, 100, true);  // вертикальний

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        SliderUpdate(&hSlider);
        SliderUpdate(&vSlider);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Horizontal Slider Value: %.2f", hSlider.value), 100, 170, 20, BLACK);
        DrawText(TextFormat("Vertical Slider Value: %.2f", vSlider.value), 500, 70, 20, BLACK);

        SliderDraw(&hSlider);
        SliderDraw(&vSlider);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

