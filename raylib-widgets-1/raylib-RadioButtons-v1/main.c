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
    int selectedButton = -1;  // Індекс вибраної кнопки (по замовчуванню жодна не вибрана)

    // Розміри кнопок
    Rectangle button1 = {100, 100, 200, 50};
    Rectangle button2 = {100, 200, 200, 50};
    Rectangle button3 = {100, 300, 200, 50};
    Rectangle button4 = {100, 400, 200, 50};

    // Головний цикл програми
    while (!WindowShouldClose())
    {
        // Обробка натискання кнопок
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            HandleButtonClick(0, button1, buttonStates, &selectedButton);
            HandleButtonClick(1, button2, buttonStates, &selectedButton);
            HandleButtonClick(2, button3, buttonStates, &selectedButton);
            HandleButtonClick(3, button4, buttonStates, &selectedButton);
        }

        // Початок малювання
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Малювання кнопок
        DrawButton(button1, buttonStates[0], "Button 1");
        DrawButton(button2, buttonStates[1], "Button 2");
        DrawButton(button3, buttonStates[2], "Button 3");
        DrawButton(button4, buttonStates[3], "Button 4");

        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
