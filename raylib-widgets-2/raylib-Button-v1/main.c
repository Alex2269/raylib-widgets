#include "raylib.h"
#include "LoadFontUnicode.h"
#include <stdio.h>

// Змінні шрифту (припускаємо, що ти їх ініціалізуєш через LoadFontUnicode)
Font font;
int fontSize = 20;
int LineSpacing = 2;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Gui_Button Example");

    // Завантаження кастомного шрифту з підтримкою Unicode (приклад)
    // Ти можеш замінити цей код на свій LoadFontUnicode
    font = LoadFontUnicode("bold.ttf", fontSize, LineSpacing);

    fontSize = 20;
    LineSpacing = 2;

    bool buttonPressed = false;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (Gui_Button((Rectangle){screenWidth/2 - 100,
            screenHeight/2 - 25, 200, 50}, "Товчи"))
        {
            buttonPressed = true;
        }

        if (buttonPressed)
        {
            // DrawText("Кнопка натиснута!", 10, 10, 20, RED);
            DrawTextEx(font, "Кнопка натиснута!", (Vector2){10, 10}, fontSize, LineSpacing, RED);
        }

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}

