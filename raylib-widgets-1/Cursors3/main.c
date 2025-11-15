// main.c
#include "raylib.h"
#include "LoadFontUnicode.h"
#include "cursor.h"
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int fontSize = 24;
int LineSpacing = 0;
Font font;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Universal Cursor");
    font = LoadFontUnicode("bold.ttf", fontSize, LineSpacing);
    SetTargetFPS(60);

    Cursor cursors[2];
    cursors[0] = InitCursor(SCREEN_WIDTH / 3.0f, DEFAULT_CURSOR_TOP_Y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, RED, 0, 100);
    cursors[1] = InitCursor(SCREEN_WIDTH * 2 / 3.0f, DEFAULT_CURSOR_TOP_Y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, BLUE, 0, 100);

    while (!WindowShouldClose())
    {
        Vector2 mousePos = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        bool mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        // Оновлення та обробка вводу для всіх курсорів
        UpdateAndHandleCursors(cursors, 2, mousePos, mousePressed, mouseDown, mouseReleased);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Малювання курсорів та лінії з відстанню
            DrawCursorsAndDistance(cursors, 2, font, fontSize);

            // Вивід значень курсорів
            DrawTextEx(font, TextFormat("Значення курсора A: %i", cursors[0].value), (Vector2){10, SCREEN_HEIGHT - 60}, fontSize, 2, BLACK);
            DrawTextEx(font, TextFormat("Значення курсора B: %i", cursors[1].value), (Vector2){10, SCREEN_HEIGHT - 30}, fontSize, 2, BLACK);

            DrawTextEx(font, "Натисніть лівою кнопкою миші на курсор,\nщоб перемістити його", (Vector2){10, 10}, fontSize, 2, BLACK);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}
