// main.c (або ваш основний файл)
#include "raylib.h"
#include "LoadFontUnicode.h"
#include "cursor.h"
#include <stdio.h>
#include <math.h>

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

    Cursor cursors[2];
    cursors[0] = InitCursor((float)SCREEN_WIDTH / 3, 50, (float)DEFAULT_CURSOR_WIDTH, (float)DEFAULT_CURSOR_HEIGHT, RED, 0, 100);
    cursors[1] = InitCursor((float)SCREEN_WIDTH * 2 / 3, 50, (float)DEFAULT_CURSOR_WIDTH, (float)DEFAULT_CURSOR_HEIGHT, BLUE, 0, 100);

    // Отримуємо поточний стан кнопок миші поза циклом для передачі в обробник
    bool mouseButtonPressed = false;
    bool mouseButtonDown = false;
    bool mouseButtonReleased = false;

    while (!WindowShouldClose())
    {
        // Оновлення
        Vector2 currentMousePosition = GetMousePosition();
        mouseButtonPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        mouseButtonDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        mouseButtonReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        // Обробка вводу для всіх курсорів одним викликом
        HandleAllCursorsMouseInput(cursors, 2, currentMousePosition, mouseButtonPressed, mouseButtonDown, mouseButtonReleased);

        // Малювання
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Малюємо курсори в циклі
            for (int i = 0; i < 2; ++i) DrawCursor(cursors[i]);

            // Малюємо лінію та відстань між двома курсорами
            DrawCursorsWithDistance(cursors[0], cursors[1]);

            // Виводимо значення курсорів
            DrawTextEx(font, TextFormat("Значення курсора A: %i", GetCursorValue(cursors[0])), (Vector2){10, SCREEN_HEIGHT - 60}, fontSize, 2, BLACK);
            DrawTextEx(font, TextFormat("Значення курсора B: %i", GetCursorValue(cursors[1])), (Vector2){10, SCREEN_HEIGHT - 30}, fontSize, 2, BLACK);
            DrawTextEx(font,"Натисніть лівою кнопкою миші на курсор,\nщоб перемістити його", (Vector2) { 10, 10 }, fontSize, 2, BLACK);

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(font); // Unload font
    // Закриття вікна
    CloseWindow();

    return 0;
}

