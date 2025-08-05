#include "raylib.h"

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib RadioButton Example");

    // Масив станів кнопок (кожна кнопка може бути вибрана або не вибрана)
    bool buttonStates[4] = { false, false, false, false };

    // Налаштування кольорів
    Color defaultColor = LIGHTGRAY;
    Color selectedColor = GREEN;

    // Головний цикл програми
    while (!WindowShouldClose())
    {
        // Обробка введення
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Перевірка, яка кнопка була натиснута
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){100, 100, 200, 50}))
            {
                buttonStates[0] = !buttonStates[0]; // Змінюємо стан кнопки 1
            }
            else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){100, 200, 200, 50}))
            {
                buttonStates[1] = !buttonStates[1]; // Змінюємо стан кнопки 2
            }
            else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){100, 300, 200, 50}))
            {
                buttonStates[2] = !buttonStates[2]; // Змінюємо стан кнопки 3
            }
            else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){100, 400, 200, 50}))
            {
                buttonStates[3] = !buttonStates[3]; // Змінюємо стан кнопки 4
            }
        }

        // Оновлення кольору кнопок в залежності від їх стану
        Color button1Color = (buttonStates[0]) ? selectedColor : defaultColor;
        Color button2Color = (buttonStates[1]) ? selectedColor : defaultColor;
        Color button3Color = (buttonStates[2]) ? selectedColor : defaultColor;
        Color button4Color = (buttonStates[3]) ? selectedColor : defaultColor;

        // Початок малювання
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Малювання кнопок
        DrawRectangle(100, 100, 200, 50, button1Color);
        DrawText("Button 1", 150, 115, 20, BLACK);

        DrawRectangle(100, 200, 200, 50, button2Color);
        DrawText("Button 2", 150, 215, 20, BLACK);

        DrawRectangle(100, 300, 200, 50, button3Color);
        DrawText("Button 3", 150, 315, 20, BLACK);

        DrawRectangle(100, 400, 200, 50, button4Color);
        DrawText("Button 4", 150, 415, 20, BLACK);

        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
