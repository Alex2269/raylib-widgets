#include "raylib.h"
#include "raylib.h"
#include "sliders.h"

#define SLIDER_KNOB_SIZE 10

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Immediate Mode Slider Example");

    float sliderValue = 50.0f; // Початкове значення
    float minVal = 0.0f;
    float maxVal = 100.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Виклик слайдера (горизонтальний)
        sliderValue = GuiSlider((Rectangle){ 100, 150, 300, 20 }, sliderValue, minVal, maxVal, false);

        DrawText(TextFormat("Slider Value: %.2f", sliderValue), 100, 100, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
