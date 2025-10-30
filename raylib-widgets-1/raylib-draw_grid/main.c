#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Для usleep
#include "raylib.h"
#include "draw_grid.h"

int fontSize = 24;
int LineSpacing = 0;
Font font;

// --- Основна функція програми ---
int main(void) {
    // --- Ініціалізація Raylib ---
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib draw_grid");

    SetTargetFPS(60);

    // --- Головний цикл Raylib ---
    while (!WindowShouldClose()) {

        // --- Відображення ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // --- Малювання області осцилографа ---
        int osc_width = screenWidth - 250;
        int osc_height = screenHeight;

        DrawRectangle(0, 0, osc_width, osc_height, BLACK); // Фон осцилографа

        draw_grid(osc_width, osc_height, 50, 49); // Малювання сітки

        EndDrawing();
    }

    // --- Завершення Raylib ---
    CloseWindow();

    return 0;
}
