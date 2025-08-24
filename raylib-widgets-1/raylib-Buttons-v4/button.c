#include "button.h"
#include "raylib.h"

// Функції для малювання кнопок
void DrawButton1(Rectangle buttonRect, bool buttonState)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText("Button 1", (int)(buttonRect.x + 25), (int)(buttonRect.y), 20, BLACK);
}

void DrawButton2(Rectangle buttonRect, bool buttonState)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText("Button 2", (int)(buttonRect.x + 25), (int)(buttonRect.y), 20, BLACK);
}

void DrawButton3(Rectangle buttonRect, bool buttonState)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText("Button 3", (int)(buttonRect.x + 25), (int)(buttonRect.y), 20, BLACK);
}

void DrawButton4(Rectangle buttonRect, bool buttonState)
{
    Color buttonColor = buttonState ? GREEN : LIGHTGRAY;
    DrawRectangleRec(buttonRect, buttonColor);
    DrawText("Button 4", (int)(buttonRect.x + 25), (int)(buttonRect.y), 20, BLACK);
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
