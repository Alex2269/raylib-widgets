// main.c

#include "raylib.h"
#include "LoadFontUnicode.h"
#include "psf_font.h"
// #include "GlyphCache.h"
#include "cursorv.h"
#include <stdio.h>

PSF_Font font18;
PSF_Font font32;

int fontSize = 24;
int LineSpacing = 0;
int spacing = 2; // Відступ між символами, той самий, що передається у DrawPSFText

// Функція для додавання нової ручки з кольором по черзі
void AddNewCursorVertical(CursorVerticalArray *arr, float fixedX, float startY) {
    Color colors[] = { RED, BLUE, GREEN, ORANGE, PURPLE, MAROON };
    int colorIndex = arr->count % (sizeof(colors) / sizeof(colors[0]));
    CursorVertical newCursorVertical = InitCursorVertical(fixedX, startY, 20, 10, colors[colorIndex], 0, 100);
    AddCursorVertical(arr, newCursorVertical);
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Завантаження PSF шрифту (шлях до вашого файлу)
    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");
    font32 = LoadPSFFont("fonts/Uni3-TerminusBold32x16.psf");

    InitWindow(screenWidth, screenHeight, "Vertical Slider with Sticky CursorVerticals");
    SetTargetFPS(60);

    Font font = GetFontDefault();
    int fontSize = 20;

    CursorVerticalArray cursors;
    InitCursorVerticalArray(&cursors, 4);

    ActiveCursorVerticalStack activeStack;
    InitActiveCursorVerticalStack(&activeStack);

    float fixedX = screenWidth - 50; // Ручки справа, 50 пікселів від краю

    // Додаємо дві початкові ручки
    AddNewCursorVertical(&cursors, fixedX, screenHeight / 3.0f);
    AddNewCursorVertical(&cursors, fixedX, screenHeight / 1.5f);

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        bool mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        // Оновлюємо курсори (перетягування, прилипання, колізії)
        UpdateAndHandleCursorVerticals(&cursors, mousePos, mousePressed, mouseDown, mouseReleased, &activeStack);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малюємо курсори та лінії
        DrawCursorVerticalsAndLines(&cursors, font18);

        // Інструкції для користувача
        DrawPSFText(font18, 10, 10, "ЛКМ перетягує ручки", spacing, DARKGRAY);
        DrawPSFText(font18, 10, 40, "Натисніть '+' щоб додати ручку", spacing, DARKGRAY);
        DrawPSFText(font18, 10, 70, "Натисніть '-' щоб видалити останню ручку", spacing, DARKGRAY);

        EndDrawing();

        // Додавання/видалення ручок клавішами
        if (IsKeyPressed(KEY_KP_ADD) || IsKeyPressed(KEY_EQUAL)) {
            AddNewCursorVertical(&cursors, fixedX, screenHeight / 2.0f);
        }
        if ((IsKeyPressed(KEY_KP_SUBTRACT) || IsKeyPressed(KEY_MINUS)) && cursors.count > 0) {
            RemoveCursorVertical(&cursors, cursors.count - 1);
        }
    }

    FreeCursorVerticalArray(&cursors);

    // Після виходу з циклу звільняємо пам'ять шрифту
    // GlyphCache_ClearAllCaches();
    UnloadPSFFont(font32);
    UnloadPSFFont(font18);

    CloseWindow();

    return 0;
}

