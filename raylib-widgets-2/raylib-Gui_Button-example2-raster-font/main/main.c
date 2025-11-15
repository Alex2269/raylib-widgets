#include "raylib.h"
#include "button.h"

#include "all_font.h"   // ваш шрифт
#include "glyphs.h"

int LineSpacing = 0;
int spacing = 2;

#define MAX_CHANNELS 4

// Масив кольорів каналів
static Color channel_colors[MAX_CHANNELS] = { YELLOW, GREEN, RED, BLUE };

int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 200;

    InitWindow(screenWidth, screenHeight, "Gui_Button Example");
    SetTargetFPS(60);

    int posX = 50;
    int posY = 30;
    int active_button = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);

        // Кнопки вибору активного каналу з кольорами
        for (int i = 0; i < MAX_CHANNELS; i++) {
            Rectangle btnRect = { posX + i * 80, posY, 70, 40 };
            Color btnColor = (active_button == i) ? channel_colors[i] : Fade(channel_colors[i], 0.4f);

            if (Gui_Button(btnRect, TerminusBold18x10_font, TextFormat("BT%d", i + 1), btnColor, GRAY, DARKGRAY, (Color){0,0,0,0})) {
                active_button = i;
            }
        }

        // Вивід стану кнопок
        const char *statusText = TextFormat("Кнопка %d %s", active_button + 1, "ВКЛ");
        DrawTextScaled(TerminusBold24x12_font, 50, 100, statusText, spacing, 2, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

