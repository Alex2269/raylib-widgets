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
    // Ініціалізація вікна
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Universal Cursor");

    // Завантаження шрифту з підтримкою Unicode
    font = LoadFontUnicode("bold.ttf", fontSize, LineSpacing);

    SetTargetFPS(60);

    // Ініціалізація двох курсорів з початковими позиціями, розмірами та кольорами
    Cursor cursors[2];
    cursors[0] = InitCursor(SCREEN_WIDTH / 6.0f, DEFAULT_CURSOR_TOP_Y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, RED, 0, 100);
    cursors[1] = InitCursor(SCREEN_WIDTH / 1.5f, DEFAULT_CURSOR_TOP_Y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, BLUE, 0, 100);

    // Ініціалізація центрального прямокутника (ручки) між курсорами
    DragRect centerRect = {
        .x = (cursors[0].x + cursors[1].x) / 2,  // центр по горизонталі
        .y = cursors[0].y,                        // початкова вертикальна позиція лінії
        .width = 40,                             // ширина прямокутника
        .height = 20,                            // висота прямокутника
        .color = DARKGRAY,                       // колір прямокутника
        .isDragging = false                      // спочатку не перетягується
    };

    // Основний цикл програми
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малювання курсорів, ліній, ручки та тексту
        DrawCursorsAndDistance(cursors, 2, font, fontSize, &centerRect);

        EndDrawing();
    }

    // Звільнення ресурсів
    UnloadFont(font);
    CloseWindow();

    return 0;
}

