#include "raylib.h"
#include "LoadFontUnicode.h"
#include "guicheckbox.h"

int fontSize = 24;
int LineSpacing = 0;
Font font;

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Button Example");
    font = LoadFontUnicode("bold.ttf", fontSize, LineSpacing);

    bool isChecked = false;
    bool isChecked2 = false;

    Rectangle checkboxRect = { 50, 50, 30, 30 };

    // Головний цикл програми
    while (!WindowShouldClose())
    {
        // Початок малювання
        BeginDrawing();

        ClearBackground(RAYWHITE);


        Gui_CheckBox(checkboxRect, &isChecked, "Увімкнути опцію", YELLOW);
        Gui_CheckBox((Rectangle){50, 100, 20, 20}, &isChecked2, "Movement", RED);


        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}

