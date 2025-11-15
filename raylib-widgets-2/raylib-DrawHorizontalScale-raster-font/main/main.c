// main.c
#include "raylib.h"
#include "DrawVerticalScale.h"
#include "DrawHorizontalScale.h"
#include "all_font.h"
#include "glyphs.h"

int spacing = 2;

int main(void)
{
    const int screenWidth = 300;
    const int screenHeight = 300;

    InitWindow(screenWidth, screenHeight, "Vertical and Horizontal Scale Example");
    SetTargetFPS(60);

    float scale = 2.0f;
    float offset_y = 50.0f;
    float offset_x = 50.0f;

    Rectangle scaleArea = { 20, 20, 60, screenHeight - 40 };

    while (!WindowShouldClose())
    {
        offset_y += 0.05f; if (offset_y > 150.0f) offset_y = 0.0f;
        offset_x += 0.05f; if (offset_x > 150.0f) offset_x = 0.0f;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawVerticalScale(0, scale, offset_y, scaleArea, TerminusBold24x12_font, BLACK);

        // Виклик горизонтальної шкали у тій самій області (або зміщеній для видимості)
        Rectangle horScaleArea = { scaleArea.x + scaleArea.width + 20, screenHeight - 60, screenWidth - scaleArea.x - scaleArea.width - 40, 50 };
        DrawHorizontalScale(0, scale, offset_x, horScaleArea, TerminusBold24x12_font, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

