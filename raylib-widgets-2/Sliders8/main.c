// main.c
#include "raylib.h"
#include "sliders.h"

#include "sliders.h"

float sliderValue1 = 50.0f;
float sliderValue2 = 50.0f;

void UpdateDrawFrame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    Gui_Slider((Rectangle){100, 150, 300, 20}, &sliderValue1, 0.0f, 100.0f, false);
    DrawText(TextFormat("Slider Value: %.2f", sliderValue1), 100, 100, 20, DARKGRAY);

    Gui_Slider((Rectangle){100, 250, 300, 20}, &sliderValue2, 0.0f, 100.0f, false);
    DrawText(TextFormat("Slider Value: %.2f", sliderValue2), 100, 200, 20, DARKGRAY);



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
