#include "raylib.h"
#include "DrawVerticalScale.h"
#include "all_font.h"
#include "glyphs.h"

int spacing = 2;

int main(void)
{
    const int screenWidth = 200;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Vertical Scale Example");
    SetTargetFPS(60);

    // Параметри шкали
    float scale = 2.0f;        // 2 пікселі на одиницю значення
    float offset_y = 50.0f;    // Центральне значення шкали

    // Область для шкали (вся висота вікна, 110 пікселів завширшки праворуч)
    Rectangle scaleArea = { screenWidth - 110, 10, 50, screenHeight - 20 };

    while (!WindowShouldClose())
    {
        // Для демонстрації плавного зміщення змінимо offset_y
        offset_y += 0.1f;
        if (offset_y > 150.0f) offset_y = 0.0f;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малюємо вертикальну шкалу у правій частині вікна
        DrawVerticalScale(0, scale, offset_y, scaleArea, Terminus12x6_font, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
