// main.c

#include "raylib.h"
#include "button.h"
#include "LoadFontUnicode.h"
#include <stdio.h>

// Глобальні змінні шрифту
Font font;
int fontSize = 20;
int LineSpacing = 2;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Gui_Button with Contrast Text");

    // Завантаження шрифту (заміни шлях на свій шрифт з підтримкою кирилиці)
    // Завантаження кастомного шрифту з підтримкою Unicode (приклад)
    // Ти можеш замінити цей код на свій LoadFontUnicode
    font = LoadFontUnicode("bold.ttf", fontSize, LineSpacing);

    SetTargetFPS(60);

    AnimatedButton myButton = { {100, 100, 100, 50}, 1.0f, false, 0.0f };

    while (!WindowShouldClose())
    {
        // UpdateButton(&myButton);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawButton(&myButton, "Товчи", font, fontSize, LineSpacing);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}

