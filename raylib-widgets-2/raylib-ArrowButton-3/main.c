// main.c

#include "raylib.h"
#include "LoadFontUnicode.h"
#include "arrow_button.h"

#include "psf_font.h"
// #include "GlyphCache.h"

PSF_Font font18;
PSF_Font font32;

int fontSize = 24;
int LineSpacing = 0;
int spacing = 2; // Відступ між символами, той самий, що передається у DrawPSFText


int main(void) {
    InitWindow(400, 200, "Arrow Button with PSF Font and Hold Logic");
    SetTargetFPS(60);

    int value = 0;
    HoldState holdUp = {0};
    HoldState holdDown = {0};

    // Завантаження PSF шрифту (шлях до вашого файлу)
    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");
    font32 = LoadPSFFont("fonts/Uni3-TerminusBold32x16.psf");

    Rectangle btnUp = {50, 70, 20, 20};
    Rectangle btnDown = {150, 70, 20, 20};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (Gui_ArrowButton(btnUp, font18, ARROW_UP, true, &value, 1, 0, 1000, NULL, NULL, BLUE, &holdUp)) {}
        if (Gui_ArrowButton(btnDown, font18, ARROW_DOWN, true, &value, 1, 0, 1000, NULL, NULL, RED, &holdDown)) {}

        // Визначаємо текст значення
        char valueText[32];
        sprintf(valueText, "%d", value);
        int charCount = utf8_strlen(valueText);
        int textWidth = charCount * (font18.width + spacing) - spacing;

        // Прямокутник фону між кнопками
        Rectangle valueRect = {
            btnUp.x + btnUp.width + 5,
            btnUp.y,
            btnDown.x - (btnUp.x + btnUp.width) - 10,
            btnUp.height
        };

        // Малюємо фон і рамку
        DrawRectangleRec(valueRect, LIGHTGRAY);
        DrawRectangleLines(valueRect.x, valueRect.y, valueRect.width, valueRect.height, DARKGRAY);

        // Малюємо значення по центру
        float textX = valueRect.x + (valueRect.width - textWidth) / 2.0f;
        float textY = valueRect.y + (valueRect.height - font18.height) / 2.0f;
        DrawPSFText(font18, (int)textX, (int)textY, valueText, spacing, BLACK);

        // --- початок блоку регулювання розміру буфера колесом миші ---

        Vector2 mousePos = GetMousePosition(); // Позиція миші
        bool mouseOver = CheckCollisionPointRec(mousePos, valueRect); // Чи курсор над прямокутником значення

        /* Якщо миша знаходиться над прямокутником значення,
        тоді можемо регулювати ширину буфера зберігання
        колесом миші */
        if (mouseOver) {
            // Проста зміна масштабу колесом миші
            int wheelMove = GetMouseWheelMove();
            value += wheelMove * 50; // Змінюємо крок масштабу за бажанням
            if (value < 50) value = 50;
            if (value > 1000) value = 1000;
        }

        DrawPSFText(font32, 50, 20, TextFormat("Значення: %d", value), spacing, BLACK);

        // --- кінець блоку регулювання розміру буфера колесом миші ---

        EndDrawing();
    }

    // Після виходу з циклу звільняємо пам'ять шрифту
    // GlyphCache_ClearAllCaches();

    UnloadPSFFont(font32);
    UnloadPSFFont(font18);

    CloseWindow();

    return 0;
}

