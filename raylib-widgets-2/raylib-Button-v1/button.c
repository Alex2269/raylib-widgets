#include "button.h"
#include "raylib.h"

// Зовнішні змінні, які ти вже маєш
extern Font font;
extern int fontSize;
extern int LineSpacing;

// Функція кнопки
bool Gui_Button(Rectangle bounds, const char *text)
{
    Vector2 mousePoint = GetMousePosition();
    bool pressed = false;

    // Колір кнопки залежно від наведення та натискання
    bool mouseOver = CheckCollisionPointRec(mousePoint, bounds);
    Color btnColor = LIGHTGRAY;

    if (mouseOver) btnColor = GRAY;
    if (mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnColor = DARKGRAY;

    // Малюємо кнопку
    DrawRectangleRec(bounds, btnColor);

    // Малюємо текст кнопки, вирівнюючи по центру кнопки
    Vector2 textSize = MeasureTextEx(font, text, fontSize, LineSpacing);
    Vector2 textPos = {
        bounds.x + (bounds.width - textSize.x) / 2,
        bounds.y + (bounds.height - textSize.y) / 2
    };
    DrawTextEx(font, text, textPos, fontSize, LineSpacing, WHITE);

    // Обробляємо натискання
    if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        pressed = true;
    }

    return pressed;
}

