#include "raylib.h"

// Оголошення функцій для малювання та обробки натискання кожної кнопки
void DrawButton1(Rectangle buttonRect, bool buttonState);
void DrawButton2(Rectangle buttonRect, bool buttonState);
void DrawButton3(Rectangle buttonRect, bool buttonState);
void DrawButton4(Rectangle buttonRect, bool buttonState);

void HandleButton1Click(Rectangle buttonRect, bool *buttonState);
void HandleButton2Click(Rectangle buttonRect, bool *buttonState);
void HandleButton3Click(Rectangle buttonRect, bool *buttonState);
void HandleButton4Click(Rectangle buttonRect, bool *buttonState);

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Button Example");

    // Масив станів кнопок
    bool buttonStates[4] = { false, false, false, false };

    // Розміри кнопок
    Rectangle button1 = {100, 100, 200, 50};
    Rectangle button2 = {100, 200, 200, 50};
    Rectangle button3 = {100, 300, 200, 50};
    Rectangle button4 = {100, 400, 200, 50};

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

// Функції для малювання кнопок
void DrawButton1(Rectangle buttonRect, bool buttonState)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText("Button 1", (int)(buttonRect.x + 50), (int)(buttonRect.y + 15), 20, BLACK);
}

void DrawButton2(Rectangle buttonRect, bool buttonState)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText("Button 2", (int)(buttonRect.x + 50), (int)(buttonRect.y + 15), 20, BLACK);
}

void DrawButton3(Rectangle buttonRect, bool buttonState)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText("Button 3", (int)(buttonRect.x + 50), (int)(buttonRect.y + 15), 20, BLACK);
}

void DrawButton4(Rectangle buttonRect, bool buttonState)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText("Button 4", (int)(buttonRect.x + 50), (int)(buttonRect.y + 15), 20, BLACK);
}

// Функції для обробки натискання кнопок
void HandleButton1Click(Rectangle buttonRect, bool *buttonState)
{
    if (CheckCollisionPointRec(GetMousePosition(), buttonRect))
    {
        *buttonState = !*buttonState;
    }
}

void HandleButton2Click(Rectangle buttonRect, bool *buttonState)
{
    if (CheckCollisionPointRec(GetMousePosition(), buttonRect))
    {
        *buttonState = !*buttonState;
    }
}

void HandleButton3Click(Rectangle buttonRect, bool *buttonState)
{
    if (CheckCollisionPointRec(GetMousePosition(), buttonRect))
    {
        *buttonState = !*buttonState;
    }
}

void HandleButton4Click(Rectangle buttonRect, bool *buttonState)
{
    if (CheckCollisionPointRec(GetMousePosition(), buttonRect))
    {
        *buttonState = !*buttonState;
    }
}
