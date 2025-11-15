// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // usleep
#include <stdbool.h>
#include <stdint.h>

#include "main.h"
#include "raylib.h"
#include "draw_grid.h"
#include "init_osc_data.h"
#include "DrawVerticalScale.h"
#include "DrawHorizontalScale.h"
#include "gui_control_panel.h"
#include "cursor.h"

#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "glyphs.h"

int LineSpacing = 0; // Відступ між рядками тексту

int spacing = 2; // Відступ між символами, той самий, що передається у DrawPSFText
int scale = 1;
int padding = 3;
int borderThickness = 1;

int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 600;

    // Встановлюємо прапорець для мультисемплінгу (покращення якості графіки)
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "Raylib Oscilloscope with Trigger and Scaling");

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

    OscData oscData = {0};
    init_osc_data(&oscData);

    while (!WindowShouldClose()) {

        static bool control_panel_visible = true;
        // Обробка введення для керування панеллю та масштабом
        if (IsKeyPressed(KEY_TAB)) control_panel_visible = !control_panel_visible;

        int panel_width = control_panel_visible ? 350 : 0;
        int osc_width = screenWidth - panel_width;
        int osc_height = screenHeight;

        // Обмеження виходу курсорів за визначені межі
        cursors[0].min_X = cursors[1].min_X = 20;
        cursors[0].max_X = cursors[1].max_X = osc_width - 18;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, osc_width, osc_height, BLACK);
        draw_grid(osc_width, osc_height, 50, 49);

        // Малювання курсорів, ліній, ручки та тексту
        DrawCursorsAndDistance(cursors, 2, Terminus12x6_font, &centerRect);
        DrawTextScaled(Terminus12x6_font, 180, 10, "простий осцилограф на бібліотеці raylib", spacing, scale, GREEN);

        ChannelSettings *Ch = &oscData.channels[oscData.active_channel];
        Rectangle scaleArea = { 1, 0, 5, 600};
        DrawVerticalScale(1, Ch->scale_y, Ch->offset_y / Ch->scale_y, scaleArea, Terminus12x6_font, WHITE);

        // Виклик горизонтальної шкали у тій самій області (або зміщеній для видимості)
        //  area - прямокутна область (x, y, width, height), де малюється шкала
        Rectangle horScaleArea = { 50, osc_height - 60, osc_width - 100 , 50 };
        DrawHorizontalScale(0, scale, 275 - oscData.trigger_offset_x,
                            horScaleArea, Terminus12x6_font, WHITE);

        // gui_control_panel(&oscData, screenWidth, screenHeight);
        if (control_panel_visible) {
            gui_control_panel(&oscData, screenWidth, screenHeight);
        }

        EndDrawing();
    }

    // Після виходу з циклу звільняємо пам'ять шрифту

    CloseWindow();

    return 0;
}


