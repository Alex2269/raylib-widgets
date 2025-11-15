#include <stdbool.h>
#include <string.h>
#include "raylib.h"
#include "gui_dropdown_roller.h"

// Зовнішні змінні шрифту (підключити свої)
extern Font font;
extern int fontSize;
extern int LineSpacing;

// Допоміжна функція для підрахунку довжини UTF-8 рядка (з твоїх кодів)
int utf8_strlen(const char* s);

// Обчислення яскравості кольору
static float GetLuminance(Color color)
{
    return 0.2126f * (color.r / 255.0f) + 0.7152f * (color.g / 255.0f) + 0.0722f * (color.b / 255.0f);
}

// Вибір контрастного кольору тексту
static Color GetContrastColor(Color bgColor)
{
    return (GetLuminance(bgColor) > 0.5f) ? BLACK : WHITE;
}

/**
 * Roller Dropdown з плавною анімацією і підтримкою миші/клавіатури.
 *
 * @param bounds - прямокутник кнопки dropdown
 * @param items - масив рядків пунктів
 * @param itemCount - кількість пунктів
 * @param currentIndex - поточний вибір (індекс)
 * @param hintText - текст підказки над кнопкою (може бути NULL)
 * @param isOpen - вказівник на стан відкриття списку
 * @param color - колір активного елемента
 * @param scrollOffset - вказівник на змінну для плавної анімації прокрутки (float)
 * @return оновлений індекс вибраного пункту
 */
int Gui_Dropdown_Roller(Rectangle bounds, const char **items, int itemCount,
                        int currentIndex, const char *hintText, bool *isOpen, Color color,
                        float *scrollOffset)
{
    int selectedIndex = currentIndex;
    int visibleCount = 5; // Кількість видимих пунктів (непарне краще)
    int halfVisible = visibleCount / 2;

    Vector2 mousePoint = GetMousePosition();
    Color textColor = GetContrastColor(color);
    int localSpacing = 2;
    int padding = 6;

    // Висота одного пункту (висота кнопки)
    float itemHeight = bounds.height;

    // Відстань для анімації (плавний перехід)
    // scrollOffset - змінна, що зберігає поточний відступ прокрутки у пікселях
    // При зміні selectedIndex ми оновлюємо scrollOffset плавно

    // Обробка відкриття/закриття
    if (*isOpen) {
        // Малюємо hintText зверху (як у твоєму коді)
        if (hintText && hintText[0] != '\0') {
            int charCount = utf8_strlen(hintText);
            float lineWidth = charCount * (fontSize * 0.6f) + padding * 2; // приблизно
            float lineHeight = fontSize + 4;

            Rectangle hintRect = {
                bounds.x + bounds.width / 2.0f - lineWidth / 2.0f,
                bounds.y - lineHeight - 8,
                lineWidth,
                lineHeight
            };

            DrawRectangleRec(hintRect, Fade(BLACK, 0.8f));
            DrawRectangleLinesEx(hintRect, 1, WHITE);

            Vector2 hintPos = { hintRect.x + padding, hintRect.y + 2 };
            DrawTextEx(font, hintText, hintPos, fontSize, LineSpacing, WHITE);
        }

        // Обробка прокрутки колесом миші
        if (CheckCollisionPointRec(mousePoint, bounds)) {
            int wheelMove = GetMouseWheelMove();
            if (wheelMove != 0) {
                selectedIndex -= wheelMove;
                if (selectedIndex < 0) selectedIndex = itemCount - 1;
                else if (selectedIndex >= itemCount) selectedIndex = 0;
            }
        }

        // Обробка клавіш Вверх/Вниз
        if (IsKeyPressed(KEY_UP)) {
            selectedIndex--;
            if (selectedIndex < 0) selectedIndex = itemCount - 1;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            selectedIndex++;
            if (selectedIndex >= itemCount) selectedIndex = 0;
        }

        // Плавне оновлення scrollOffset
        float targetOffset = (float)selectedIndex * itemHeight;
        const float smoothSpeed = 10.0f; // швидкість анімації
        *scrollOffset += (targetOffset - *scrollOffset) * (GetFrameTime() * smoothSpeed);

        // Центр списку (по вертикалі)
        float centerY = bounds.y + bounds.height / 2.0f;

        // Малюємо видимі пункти
        for (int i = -halfVisible; i <= halfVisible; i++) {
            int itemIndex = selectedIndex + i;
            // Циклічний індекс
            if (itemIndex < 0) itemIndex += itemCount;
            else if (itemIndex >= itemCount) itemIndex -= itemCount;

            // Позиція пункту з урахуванням scrollOffset
            float y = centerY + (i * itemHeight) - (*scrollOffset - selectedIndex * itemHeight);

            Rectangle itemRect = { bounds.x, y - itemHeight / 2, bounds.width, itemHeight };

            // Колір кнопки - активний центр яскравіший
            Color btnColor = (i == 0) ? color : Fade(color, 0.4f);

            if (Gui_Button(itemRect, items[itemIndex], btnColor, GRAY, DARKGRAY, (Color){0,0,0,0})) {
                selectedIndex = itemIndex;
                *isOpen = false;
            }
        }
    }

    // Малюємо кнопку dropdown з поточним вибором
    if (Gui_Button(bounds, items[selectedIndex], color, GRAY, DARKGRAY, (Color){0,0,0,0})) {
        *isOpen = !(*isOpen);
    }

    return selectedIndex;
}

