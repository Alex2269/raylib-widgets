// main.c

#include "raylib.h"
#include "cursor.h"
#include "all_font.h"
#include "glyphs.h"

int spacing = 2;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Cursor Example");
    SetTargetFPS(60);

    // Ініціалізуємо два курсори з початковими позиціями і значеннями
    Cursor cursors[2];
    cursors[0] = InitCursor(200, 100, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, RED, 0, 100);
    cursors[1] = InitCursor(600, 100, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, BLUE, 0, 100);

    // Задаємо обмеження позицій по X (щоб курсори не виходили за межі)
    cursors[0].min_X = 0; cursors[0].max_X = screenWidth;
    cursors[1].min_X = 0; cursors[1].max_X = screenWidth;

    // Ініціалізуємо центральний прямокутник (ручку)
    DragRect centerRect = { (cursors[0].x + cursors[1].x)/2, 150, 50, 20, GRAY, false };

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        bool mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Оновлюємо курсори і центральний прямокутник з обробкою вводу користувача
        UpdateAndHandleCursors(cursors, 2, mousePos, mousePressed, mouseDown, mouseReleased);
        UpdateAndHandleCenterRect(&centerRect, mousePos, mousePressed, mouseDown, mouseReleased);

        // Малюємо курсори, лінії, ручку і відстань
        DrawCursorsAndDistance(cursors, 2, TerminusBold24x12_font, &centerRect);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

