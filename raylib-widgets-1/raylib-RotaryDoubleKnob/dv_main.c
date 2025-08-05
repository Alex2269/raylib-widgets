// dv_main.c
#include <stdint.h>
#include "raylib.h"
#include "dv_knob.h" // Підключаємо файл з подвійним регулятором
#include "LoadFontUnicode.h"

int fontSize = 24;
int LineSpacing = 0;
Font font;

int main() {
    const int screenWidth = 400;
    const int screenHeight = 300;
    InitWindow(screenWidth, screenHeight, "Dual Vernier Knob Example");

    font = LoadFontUnicode("bold.ttf", fontSize, LineSpacing);

    SetTargetFPS(60);

    // Створюємо екземпляр подвійного регулятора
    DualVernierKnob vernierKnob = InitDualVernierKnob(200, 150, 60);

    while (!WindowShouldClose()) {
        // Оновлюємо стан подвійного регулятора
        vernierKnob.value = UpdateDualVernierKnob(&vernierKnob, true);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малюємо подвійний регулятор
        DrawDualVernierKnob(vernierKnob);

        // DrawText("Клікніть та обертайте внутрішню/зовнішню ручку", 20, 30, 20, GRAY);
        DrawTextEx(font, "Клікніть та обертайте\nвнутрішню/зовнішню ручку", (Vector2) {20, 20}, fontSize, 2, DARKGRAY);

        // DrawText(TextFormat("Значення: %d", vernierKnob.value), 20, 60, 20, BLACK);
        DrawTextEx(font, TextFormat("Значення: %d", vernierKnob.value), (Vector2) {20, 240}, fontSize, 2, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

