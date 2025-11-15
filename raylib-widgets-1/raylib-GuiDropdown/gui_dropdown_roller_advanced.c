#include <stdbool.h>
#include <string.h>
#include "raylib.h"
#include "gui_dropdown_roller_advanced.h"

extern Font font;
extern int fontSize;
extern int LineSpacing;
int utf8_strlen(const char* s);

static float GetLuminance(Color color)
{
    return 0.2126f * (color.r / 255.0f) + 0.7152f * (color.g / 255.0f) + 0.0722f * (color.b / 255.0f);
}

static Color GetContrastColor(Color bgColor)
{
    return (GetLuminance(bgColor) > 0.5f) ? BLACK : WHITE;
}

int Gui_Dropdown_Roller_Advanced(Rectangle bounds, const char **items, int itemCount,
                                 int currentIndex, const char *hintText, bool *isOpen, Color color,
                                 float *scrollOffset, float *scrollVelocity)
{
    int selectedIndex = currentIndex;
    int visibleCount = 7; // Кількість видимих пунктів (непарне)
    int halfVisible = visibleCount / 2;

    Vector2 mousePoint = GetMousePosition();
    Color textColor = GetContrastColor(color);
    int localSpacing = 2;
    int padding = 6;

    float itemHeight = bounds.height;

    if (*isOpen) {
        // Малюємо hintText зверху (як у попередньому коді)...

        // Обробка коліщатка миші
        if (CheckCollisionPointRec(mousePoint, bounds)) {
            int wheelMove = GetMouseWheelMove();
            if (wheelMove != 0) {
                *scrollVelocity -= wheelMove * itemHeight * 0.5f;
            }
        }

        // Обробка перетягування миші
        static bool dragging = false;
        static float dragStartY = 0;
        static float scrollStartOffset = 0;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, bounds)) {
            dragging = true;
            dragStartY = mousePoint.y;
            scrollStartOffset = *scrollOffset;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            dragging = false;
        }
        if (dragging) {
            float deltaY = mousePoint.y - dragStartY;
            *scrollOffset = scrollStartOffset - deltaY;
            *scrollVelocity = 0; // При перетягуванні інерція скидається
        }

        // Плавна інерція прокрутки
        const float friction = 8.0f;
        if (!dragging) {
            float dt = GetFrameTime();
            if (*scrollVelocity > 0.01f || *scrollVelocity < -0.01f) {
                *scrollOffset += *scrollVelocity * dt;
                // Зменшуємо швидкість
                if (*scrollVelocity > 0)
                    *scrollVelocity -= friction * dt * itemHeight;
                else
                    *scrollVelocity += friction * dt * itemHeight;
            } else {
                *scrollVelocity = 0;
            }
        }

        // Обмеження scrollOffset в межах (циклічний список)
        float maxOffset = (float)(itemCount - 1) * itemHeight;
        while (*scrollOffset < 0) *scrollOffset += maxOffset;
        while (*scrollOffset > maxOffset) *scrollOffset -= maxOffset;

        // Обчислюємо індекс, найближчий до центру
        int centerIndex = (int)((*scrollOffset + itemHeight / 2) / itemHeight);
        centerIndex = centerIndex % itemCount;
        if (centerIndex < 0) centerIndex += itemCount;
        selectedIndex = centerIndex;

        float centerY = bounds.y + bounds.height / 2.0f;

        // Малюємо видимі пункти з анімацією масштабу і прозорості
        for (int i = -halfVisible; i <= halfVisible; i++) {
            int itemIndex = selectedIndex + i;
            if (itemIndex < 0) itemIndex += itemCount;
            else if (itemIndex >= itemCount) itemIndex -= itemCount;

            float itemCenterY = centerY + (i * itemHeight) - fmodf(*scrollOffset, itemHeight);

            Rectangle itemRect = { bounds.x, itemCenterY - itemHeight / 2, bounds.width, itemHeight };

            // Відстань від центру в пунктах
            float dist = (float)abs(i);

            // Масштаб і прозорість (зменшуємо з віддаленням)
            float scale = 1.0f - 0.15f * dist;
            if (scale < 0.5f) scale = 0.5f;

            float alpha = 1.0f - 0.3f * dist;
            if (alpha < 0.3f) alpha = 0.3f;

            Color btnColor = (i == 0) ? color : Fade(color, alpha);

            // Малюємо кнопку з масштабом (потрібно змінити DrawTextEx або DrawPSFText для підтримки масштабу)
            // Тут приклад з DrawTextEx (звичайний шрифт raylib)
            Vector2 textSize = MeasureTextEx(font, items[itemIndex], fontSize, LineSpacing);
            Vector2 textPos = {
                bounds.x + bounds.width / 2.0f - (textSize.x * scale) / 2.0f,
                itemCenterY - (textSize.y * scale) / 2.0f
            };

            DrawRectangleRec(itemRect, Fade(btnColor, alpha));
            DrawTextEx(font, items[itemIndex], textPos, fontSize * scale, LineSpacing, GetContrastColor(btnColor));

            // Обробка кліку по пункту
            if (CheckCollisionPointRec(mousePoint, itemRect) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                selectedIndex = itemIndex;
                *isOpen = false;
                *scrollOffset = selectedIndex * itemHeight;
                *scrollVelocity = 0;
            }
        }
    }

    // Малюємо кнопку dropdown з поточним вибором
    if (Gui_Button(bounds, items[selectedIndex], color, GRAY, DARKGRAY, (Color){0,0,0,0})) {
        *isOpen = !(*isOpen);
    }

    return selectedIndex;
}
