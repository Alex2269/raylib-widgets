#include "raylib.h"
#include "psf_font.h"
#include "cursor.h"
#include <stdio.h>

PSF_Font font18;

int fontSize = 24;
int LineSpacing = 0;
int spacing = 2; // Відступ між символами, той самий, що передається у DrawPSFText

int main(void) {
    int window_w = 800;
    int window_h = 600;
    InitWindow(window_w, window_h, "Slider Widget with Overlapping Cursors");
    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");

    SetTargetFPS(60);

    Cursor cursors[2];
    cursors[0] = InitCursor(225.0f, DEFAULT_CURSOR_TOP_Y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, RED, 0, 100);
    cursors[1] = InitCursor(425.0f, DEFAULT_CURSOR_TOP_Y, DEFAULT_CURSOR_WIDTH, DEFAULT_CURSOR_HEIGHT, BLUE, 0, 100);

    // Ініціалізація центрального прямокутника (ручки) між курсорами
    DragRect centerRect = {
        .x = (cursors[0].x + cursors[1].x) / 2,  // центр по горизонталі
        .y = cursors[0].y,                        // початкова вертикальна позиція лінії
        .width = 20,                             // ширина прямокутника
        .height = 10,                            // висота прямокутника
        .color = LIGHTGRAY,                       // колір прямокутника
        .isDragging = false                      // спочатку не перетягується
    };

    // обмеження виходу курсорів за визначені межі
    cursors[0].min_X = cursors[1].min_X = 20;
    cursors[0].max_X = cursors[1].max_X = window_w - 18;


    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GRAY);
        DrawRectangle(10, 10, window_w-20, window_h-20, DARKGRAY);

        // Малювання курсорів, ліній, ручки та тексту
        DrawCursorsAndDistance(cursors, 2, font18, font18.height, &centerRect);

        EndDrawing();
    }

    UnloadPSFFont(font18);
    CloseWindow();
    return 0;
}

