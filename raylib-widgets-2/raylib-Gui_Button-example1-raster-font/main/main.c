// main.c
#include "raylib.h"
#include "button.h"

#include "all_font.h"   // ваш шрифт
#include "glyphs.h"

int LineSpacing = 0;
int spacing = 2;

int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 200;

    InitWindow(screenWidth, screenHeight, "Gui_Button Example");
    SetTargetFPS(60);

    // Стани кнопок
    bool buttonPressed[3] = { false, false, false };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);

        // Параметри кнопок
        Rectangle btn1 = { 50, 50, 120, 40 };
        Rectangle btn2 = { 200, 50, 120, 40 };
        Rectangle btn3 = { 350, 50, 120, 40 };

        if (Gui_Button(btn1, TerminusBold24x12_font, "Кнопка 1", SKYBLUE, BLUE, DARKBLUE, (Color){0,0,0,0}))
            buttonPressed[0] = !buttonPressed[0];

            if (Gui_Button(btn2, TerminusBold24x12_font, "Кнопка 2", LIGHTGRAY, GRAY, DARKGRAY, (Color){0,0,0,0}))
                buttonPressed[1] = !buttonPressed[1];

                if (Gui_Button(btn3, TerminusBold24x12_font, "Вийти", PINK, RED, MAROON, WHITE))
                    CloseWindow();

        // Вивід стану кнопок
        DrawTextScaled(TerminusBold24x12_font, 50, 110,
                       buttonPressed[0] ? "Кнопка\n1 ВКЛ" : "Кнопка\n1 ВИКЛ",
                       spacing, 1, BLACK);

        DrawTextScaled(TerminusBold24x12_font, 200, 110,
                       buttonPressed[1] ? "Кнопка\n2 ВКЛ" : "Кнопка\n2 ВИКЛ",
                       spacing, 1, BLACK);

        DrawTextScaled(TerminusBold24x12_font, 350, 110,
                       "Клікни Вийти\nщоб закрити",
                       spacing, 1, BLACK);

        EndDrawing();
    }

    return 0;
}

