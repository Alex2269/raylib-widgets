#include "raylib.h"
#include "button.h"

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib RadioButton Example");

    // Стан кнопок (якщо вибрано, то 1, якщо не вибрано - 0)
    int selectedButton = -1;

    // Головний цикл програми
    while (!WindowShouldClose())
    {
        // Обробка введення
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Перевірка, яка кнопка була натиснута
            if (CheckButtonPress(GetMousePosition(), (Rectangle){100, 100, 200, 50}))
            {
                selectedButton = 0;
            }
            else if (CheckButtonPress(GetMousePosition(), (Rectangle){100, 200, 200, 50}))
            {
                selectedButton = 1;
            }
            else if (CheckButtonPress(GetMousePosition(), (Rectangle){100, 300, 200, 50}))
            {
                selectedButton = 2;
            }
            else if (CheckButtonPress(GetMousePosition(), (Rectangle){100, 400, 200, 50}))
            {
                selectedButton = 3;
            }
        }

        // Початок малювання
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Малювання кнопок
        DrawButton((Rectangle){100, 100, 200, 50}, selectedButton == 0, "Button 1");
        DrawButton((Rectangle){100, 200, 200, 50}, selectedButton == 1, "Button 2");
        DrawButton((Rectangle){100, 300, 200, 50}, selectedButton == 2, "Button 3");
        DrawButton((Rectangle){100, 400, 200, 50}, selectedButton == 3, "Button 4");

        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
