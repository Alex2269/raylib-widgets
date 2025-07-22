#include "raylib.h"
#include "button.h"

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Button Example");

    // Масив станів кнопок
    bool buttonStates[4] = { false, false, false, false };

    // Розміри кнопок
    Rectangle button1 = {100, 100, 20, 20};
    Rectangle button2 = {100, 200, 20, 20};
    Rectangle button3 = {100, 300, 20, 20};
    Rectangle button4 = {100, 400, 20, 20};

    // Головний цикл програми
    while (!WindowShouldClose())
    {
        // Обробка введення для кожної кнопки
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            HandleButton1Click(button1, &buttonStates[0]);
            HandleButton2Click(button2, &buttonStates[1]);
            HandleButton3Click(button3, &buttonStates[2]);
            HandleButton4Click(button4, &buttonStates[3]);
        }

        // Початок малювання
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Малювання кнопок
        DrawButton1(button1, buttonStates[0]);
        DrawButton2(button2, buttonStates[1]);
        DrawButton3(button3, buttonStates[2]);
        DrawButton4(button4, buttonStates[3]);

        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
