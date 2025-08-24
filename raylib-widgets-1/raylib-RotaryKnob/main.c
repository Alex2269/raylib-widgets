// main.c
#include <stdint.h>
#include "raylib.h"
#include "knob.h"  // Підключення файлу з логікою регулятора

int main() {
    const int screenWidth = 350;
    const int screenHeight = 400;
    InitWindow(screenWidth, screenHeight, "Rotary Knob Example");

    SetTargetFPS(60);

    uint16_t x_pos[6] = { 100, 250, 100, 250, 100, 250 };
    uint16_t y_pos[6] = { 100, 100, 200, 200, 300, 300 };
    float knobRadius = 40;
    int knobValue[6] = { 0, 0, 0, 0, 0, 0 };

    bool isDragging[6] = { false, false, false, false, false, false };
    float knobAngle[6] = { -135.0f, -135.0f, -135.0f, -135.0f, -135.0f, -135.0f };

    while (!WindowShouldClose()) {
        // Оновлюємо кожен регулятор через knob_handler
        for (int i = 0; i < 6; i++) {
            knobValue[i] = knob_handler(x_pos[i], y_pos[i], knobRadius, knobValue[i], &isDragging[i], &knobAngle[i], true);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малюємо всі регулятори через draw_knob
        for (int i = 0; i < 6; i++) {
            draw_knob(x_pos[i], y_pos[i], knobRadius, knobAngle[i], knobValue[i]);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
