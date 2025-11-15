// main.c

#include "raylib.h"
#include "button.h"

#include "all_font.h"
#include "glyphs.h"

int LineSpacing = 0;
int spacing = 2;

#define MAX_BUTTONS 4

// Масив кольорів для кнопок
static Color button_colors[MAX_BUTTONS] = { YELLOW, GREEN, RED, BLUE };

int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 200;

    InitWindow(screenWidth, screenHeight, "Independent Buttons Example");
    SetTargetFPS(60);

    bool button_states[MAX_BUTTONS] = { false, false, false, false };

    int posX = 50;
    int posY = 30;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GRAY);

        // Малюємо і обробляємо кожну кнопку окремо
        for (int i = 0; i < MAX_BUTTONS; i++) {
            Rectangle btnRect = { posX + i * 90, posY, 80, 40 };
            Color bgColor = button_states[i] ? button_colors[i] : Fade(button_colors[i], 0.4f);

            if (Gui_Button(btnRect, TerminusBold18x10_font, TextFormat("Btn %d", i + 1), bgColor, GRAY, DARKGRAY, (Color){0,0,0,0})) {
                // Кожен клік змінює стан цієї кнопки незалежно
                button_states[i] = !button_states[i];
            }

            // Підпис під кнопкою про її стан
            DrawTextScaled(TerminusBold18x10_font,
                           btnRect.x + 10,
                           btnRect.y + btnRect.height + 5,
                           button_states[i] ? "ON" : "OFF", spacing, 1, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

