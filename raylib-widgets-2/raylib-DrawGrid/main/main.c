// main.c

#include "raylib.h"
#include "draw_grid.h"


int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 600;

    // Встановлюємо прапорець для мультисемплінгу (покращення якості графіки)
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "Raylib Oscilloscope with Trigger and Scaling");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        static bool control_panel_visible = true;
        // Обробка введення для керування панеллю та масштабом
        if (IsKeyPressed(KEY_TAB)) control_panel_visible = !control_panel_visible;

        int panel_width = control_panel_visible ? 350 : 0;
        int osc_width = screenWidth - panel_width;
        int osc_height = screenHeight;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, osc_width, osc_height, BLACK);
        draw_grid(osc_width, osc_height, 50, 49);

        EndDrawing();
    }

    // Після виходу з циклу звільняємо пам'ять шрифту

    CloseWindow();

    return 0;
}


