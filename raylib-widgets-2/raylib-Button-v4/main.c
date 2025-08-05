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

    bool buttonPressed = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (Gui_Button((Rectangle){screenWidth/2 - 100, screenHeight/2 - 25, 200, 50},
                       "Натисни мене",
                       LIGHTGRAY, GRAY, DARKGRAY,
                       (Color){0,0,0,0})) // альфа 0 — автоматичний колір тексту
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

