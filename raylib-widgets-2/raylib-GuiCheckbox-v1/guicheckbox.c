#include "guicheckbox.h"
#include "raylib.h"

bool GuiCheckbox(Rectangle bounds, bool checked, const char *text)
{
    bool clicked = false;
    Vector2 mousePoint = GetMousePosition();

    // Малюємо чекбокс
    Color boxColor = checked ? GREEN : LIGHTGRAY;
    DrawRectangleRec(bounds, boxColor);
    DrawText(text, (int)(bounds.x + bounds.width + 10), (int)(bounds.y), 20, BLACK);

    // Обробляємо натискання миші
    if (CheckCollisionPointRec(mousePoint, bounds))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            clicked = true;
            checked = !checked;
        }
    }

    return checked;
}

