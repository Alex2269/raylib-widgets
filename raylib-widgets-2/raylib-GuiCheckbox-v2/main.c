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

    // Масив станів кнопок
    bool checkboxState1 =  false;
    bool checkboxState2 =  false;
    bool checkboxState3 =  false;

    // Розміри кнопок
    Rectangle button1 = {100, 100, 20, 20};
    Rectangle button2 = {100, 140, 20, 20};
    Rectangle button3 = {100, 180, 20, 20};


    // Головний цикл програми
    while (!WindowShouldClose())
    {
        // Початок малювання
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Малювання кнопок
        Gui_CheckBox(button1, &checkboxState1, "Check me");
        Gui_CheckBox(button2, &checkboxState2, "Check me");
        Gui_CheckBox(button3, &checkboxState3, "Check me");

        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
