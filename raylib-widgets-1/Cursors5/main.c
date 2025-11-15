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
    cursors[0] = InitCursor(SCREEN_WIDTH / 6.0f, DEFAULT_CURSOR_TOP_Y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, RED, 0, 100);
    cursors[1] = InitCursor(SCREEN_WIDTH / 5.0f, DEFAULT_CURSOR_TOP_Y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, BLUE, 0, 100);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малювання курсорів та лінії з відстанню
        DrawCursorsAndDistance(cursors, 2, font, fontSize);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}
