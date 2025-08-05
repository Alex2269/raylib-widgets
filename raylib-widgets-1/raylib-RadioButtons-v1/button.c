#include "button.h"
#include "raylib.h"

// Функція для малювання кнопки
void DrawButton(Rectangle buttonRect, bool buttonState, const char* text)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText(text, (int)(buttonRect.x + 25), (int)(buttonRect.y + 10), 20, BLACK);
}

// Функція для обробки натискання кнопки
void HandleButtonClick(int buttonIndex, Rectangle buttonRect, bool buttonStates[], int *selectedButton)
{
    if (CheckCollisionPointRec(GetMousePosition(), buttonRect))
    {
        // Якщо кнопка натиснута, встановлюємо її як вибрану, а всі інші - не вибрані
        *selectedButton = buttonIndex;
        for (int i = 0; i < 4; i++)
        {
            buttonStates[i] = (i == *selectedButton);
        }
    }
}
