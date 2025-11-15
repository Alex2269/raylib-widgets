// main.c

#include "raylib.h"
#include "knob_gui.h"
#include <stdio.h>
#include <math.h>

#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "glyphs.h"

int fontSize = 24;   // Розмір шрифту для тексту
int LineSpacing = 0; // Міжрядковий інтервал (0 - стандартний)
int spacing = 2; // Відступ між символами, той самий, що передається у DrawPSFText

int main(void)
{
    const int screenWidth = 800;  // Ширина вікна
    const int screenHeight = 450; // Висота вікна

    // Встановлюємо прапорець для мультисемплінгу (покращення якості графіки)
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Ініціалізуємо вікно з назвою
    InitWindow(screenWidth, screenHeight, "Raylib Knob Example");

    // Встановлюємо цільову частоту кадрів
    SetTargetFPS(60);

    // Масив значень регуляторів
    float values[5] = { 1.0f, 2.25f, 5.5f, 3.0f, 55.0f };

    // Радіус регуляторів
    float knob_radius = 44.0f;

    // У головному циклі
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Регулятор 0
        Gui_Knob_Channel(0, Terminus12x6_font, TerminusBold18x10_font, 600, 200,
                         "Масштабування 1", NULL,
                         knob_radius * 2,
                         &values[0], 2.0f, -2.0f, true, YELLOW);
        printf("scale: %0.2f\n", values[0]);

        // Регулятор 1
        Gui_Knob_Channel(1, Terminus12x6_font, TerminusBold18x10_font, 100 + (int)knob_radius, 200,
                         "Масштабування 2", NULL,
                         knob_radius,
                         &values[1], 0.0f, 5.0f, true, WHITE);

        // Регулятор 2
        Gui_Knob_Channel(2, Terminus12x6_font, TerminusBold18x10_font, 300 + (int)knob_radius, 200,
                         "Масштабування 3", NULL,
                         knob_radius,
                         &values[2], 20.0f, -20.0f, true, GREEN);

        // Регулятор 3
        Gui_Knob_Channel(3, Terminus12x6_font, TerminusBold18x10_font, 100 + (int)knob_radius, 350,
                         "Масштабування 4", NULL,
                         knob_radius,
                         &values[3], -2.0f, 5.0f, true, DARKGREEN);

        // Регулятор 4
        Gui_Knob_Channel(4, Terminus12x6_font, TerminusBold18x10_font, 300 + (int)knob_radius, 350,
                         "Масштабування 5", NULL,
                         knob_radius,
                         &values[4], -100.0f, 100.0f, true, SKYBLUE);

        EndDrawing();
    }

    // Закриваємо вікно і звільняємо ресурси
    CloseWindow();

    return 0;
}

