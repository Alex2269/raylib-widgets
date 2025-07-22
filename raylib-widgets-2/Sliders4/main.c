// main.c
#include "raylib.h"
#include "sliders.h"

#include "sliders.h"

float sliderValue = 50.0f;

void UpdateDrawFrame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    GuiSlider((Rectangle){100, 150, 300, 20}, &sliderValue, 0.0f, 100.0f, false);

    DrawText(TextFormat("Slider Value: %.2f", sliderValue), 100, 100, 20, DARKGRAY);

    EndDrawing();
}

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Button Example");

    // Головний цикл програми
    while (!WindowShouldClose())
    {

        UpdateDrawFrame();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
