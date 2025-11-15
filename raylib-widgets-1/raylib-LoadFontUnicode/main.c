#include "raylib.h"
#include "LoadFontUnicode.h"
#include "cursor.h"
#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int fontSize = 24;
int LineSpacing = 0;
Font font;

int main(void)
{
    // Ініціалізація вікна
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Universal Cursor");

    font = LoadFontUnicode("bold.ttf", fontSize, LineSpacing);

    SetTargetFPS(60);

    // Ініціалізація курсорів як універсальних віджетів
    Cursor cursorA = InitCursor((float)SCREEN_WIDTH / 3, 50, (float)DEFAULT_CURSOR_WIDTH, (float)DEFAULT_CURSOR_HEIGHT, RED, 0, 100);
    Cursor cursorB = InitCursor((float)SCREEN_WIDTH * 2 / 3, 50, (float)DEFAULT_CURSOR_WIDTH, (float)DEFAULT_CURSOR_HEIGHT, BLUE, 0, 100);

    // Отримуємо поточний стан кнопок миші поза циклом для передачі в обробник
    bool mouseButtonPressed = false;
    bool mouseButtonDown = false;
    bool mouseButtonReleased = false;
    Vector2 mousePosition = { 0.0f, 0.0f };

    // Головний цикл гри
    while (!WindowShouldClose())
    {
        // Оновлення
        Vector2 currentMousePosition = GetMousePosition();
        mouseButtonPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        mouseButtonDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        mouseButtonReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        // Обробка вводу миші для курсорів
        HandleCursorMouseInput(&cursorA, currentMousePosition, mouseButtonPressed, mouseButtonDown, mouseButtonReleased, &cursorB);
        HandleCursorMouseInput(&cursorB, currentMousePosition, mouseButtonPressed, mouseButtonDown, mouseButtonReleased, &cursorA);

        // Малювання
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Малюємо курсори
            DrawCursor(cursorA);
            DrawCursor(cursorB);

            // Малюємо лінію та відстань
            DrawCursorsWithDistance(cursorA, cursorB);

            // Виводимо значення курсорів
            DrawTextEx(font, TextFormat("Значення курсора A: %i", GetCursorValue(cursorA)), (Vector2){10, SCREEN_HEIGHT - 60}, fontSize, 2, BLACK);
            DrawTextEx(font, TextFormat("Значення курсора B: %i", GetCursorValue(cursorB)), (Vector2){10, SCREEN_HEIGHT - 30}, fontSize, 2, BLACK);
            DrawTextEx(font,"Натисніть лівою кнопкою миші на курсор,\nщоб перемістити його", (Vector2) { 10, 10 }, fontSize, 2, BLACK);

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(font); // Unload font
    // Закриття вікна
    CloseWindow();

    return 0;
}

