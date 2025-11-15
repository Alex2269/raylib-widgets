// button_extended.c

#include "raylib.h"
#include "button_extended.h"
#include "color_utils.h"

static const double LONG_PRESS_DURATION = 0.5;     // Секунди для тривалого натискання
static const double DOUBLE_CLICK_INTERVAL = 0.3;   // Максимальний інтервал між кліками для подвійного

bool Gui_ButtonExtended(Rectangle bounds, RasterFont font, const char *text, int spacing,
                        Color colorNormal, Color colorHover, Color colorPressed,
                        Color colorText, ButtonState *state,
                        bool *outLongPress, bool *outDoubleClick)
{
    Vector2 mousePoint = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mousePoint, bounds);
    bool pressed = false;

    double currentTime = GetTime();

    *outLongPress = false;
    *outDoubleClick = false;

    Color btnColor = colorNormal;
    if (mouseOver) btnColor = colorHover;
    if (mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnColor = colorPressed;

    // Малюємо кнопку (як в оригіналі)
    Color borderColor = (GetLuminance(btnColor) > 0.5f) ? BLACK : WHITE;
    int borderThickness = 2;
    DrawRectangleLinesEx((Rectangle){bounds.x - borderThickness, bounds.y - borderThickness,
        bounds.width + 2*borderThickness, bounds.height + 2*borderThickness},
        borderThickness, borderColor);
    DrawRectangleRec(bounds, btnColor);

    Color textColor = (colorText.a == 0) ? ((GetLuminance(btnColor) > 0.5f) ? BLACK : WHITE) : colorText;

    int charCount = utf8_strlen(text);
    float lineWidth = charCount * (font.glyph_width + spacing) - spacing;

    Vector2 textPos = {
        bounds.x + (bounds.width - lineWidth) / 2,
        bounds.y + (bounds.height - font.glyph_height) / 2
    };

    DrawTextScaled(font, textPos.x, textPos.y, text, spacing, 1, textColor);

    // Обробка натискань, тривалого натискання і подвійного кліка
    if (mouseOver) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            if (!state->isPressed) {
                // Кнопка тільки-но натиснута
                state->isPressed = true;
                state->longPressStartTime = currentTime;
                state->longPressTriggered = false;
            } else {
                // Тримаємо кнопку, перевіряємо тривалість для long press
                if (!state->longPressTriggered && (currentTime - state->longPressStartTime > LONG_PRESS_DURATION)) {
                    *outLongPress = true;
                    state->longPressTriggered = true;
                }
            }
        } else {
            if (state->isPressed) {
                // Кнопка відпущена після натискання
                pressed = true;

                // Перевірка подвійного кліку
                if ((currentTime - state->lastClickTime) < DOUBLE_CLICK_INTERVAL) {
                    *outDoubleClick = true;
                }
                state->lastClickTime = currentTime;
                state->isPressed = false;
                state->longPressTriggered = false;
            }
        }
    } else {
        // Якщо миша відійшла, скидаємо стан
        state->isPressed = false;
        state->longPressTriggered = false;
    }

    return pressed;
}

