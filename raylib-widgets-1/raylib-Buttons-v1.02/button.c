#include "button.h"

// Функція для перевірки натискання кнопки
int CheckButtonPress(Vector2 mousePosition, Rectangle buttonRect)
{
    return CheckCollisionPointRec(mousePosition, buttonRect);
}

// Функція для малювання кнопки
void DrawButton(Rectangle buttonRect, int isSelected, const char* text)
{
    Color buttonColor = (isSelected) ? GREEN : LIGHTGRAY;

    DrawRectangle(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, buttonColor);
    DrawText(text, buttonRect.x + 50, buttonRect.y + 15, 20, BLACK);
}
