#include "raylib.h"
#include "guicheckbox.h"

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Button Example");

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
        checkboxState1 = GuiCheckbox(button1, checkboxState1, "Check me");
        checkboxState2 = GuiCheckbox(button2, checkboxState2, "Check me");
        checkboxState3 = GuiCheckbox(button3, checkboxState3, "Check me");


        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
