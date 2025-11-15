// main.c

#include "raylib.h"
#include "cam_switch.h"
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

    // Масив значень регуляторів (float від 0.1f до 100.0f)
    float values[5] = { 0.4f, 2.5f, 5.5f, 4.0f, 0.0f };

    float knob_radius = 44.0f; // Радіус регуляторів

    // Основний цикл програми, виконується поки вікно не закриють
    while (!WindowShouldClose())
    {
        BeginDrawing();          // Починаємо малювання кадру
        ClearBackground(RAYWHITE); // Очищаємо фон білим кольором

        Gui_CamSwitch_Channel(0, Terminus12x6_font, TerminusBold18x10_font, 600, 200,
                        TextFormat("Масштабування 1", (int)values[0] + 1),
                        /*TextFormat("%0.1f", values[0])*/NULL,
                        knob_radius*2,
                        &values[0], 2.0f, -2.0f, true, YELLOW);

        // values[0] = round(values[0] / 0.1) * 0.1;
        printf("scale: %0.2f\n", values[0]);

        Gui_CamSwitch_Channel(1, Terminus12x6_font, TerminusBold18x10_font, 100 + knob_radius, 200,
                        TextFormat("Масштабування 2", (int)values[1] + 1),
                        /*TextFormat("%0.1f", values[0])*/NULL,
                        knob_radius,
                        &values[1], 0.0f, 5.0f, true, WHITE);

        Gui_CamSwitch_Channel(2, Terminus12x6_font, TerminusBold18x10_font, 300 + knob_radius, 200,
                        TextFormat("Масштабування 3", (int)values[2] + 1),
                        /*TextFormat("%0.1f", values[1])*/NULL,
                        knob_radius,
                        &values[2], -20.0f, 20.0f, true, GREEN);

        Gui_CamSwitch_Channel(3, Terminus12x6_font, TerminusBold18x10_font, 100 + knob_radius, 350,
                        TextFormat("Масштабування 4", (int)values[3] + 1),
                        /*TextFormat("%0.1f", values[0])*/NULL,
                        knob_radius,
                        &values[3], 20.0f, -20.0f, true, DARKGREEN);

        Gui_CamSwitch_Channel(4, Terminus12x6_font, TerminusBold18x10_font, 300 + knob_radius, 350,
                        TextFormat("Масштабування 5", (int)values[4] + 1),
                        /*TextFormat("%0.1f", values[1])*/NULL,
                        knob_radius,
                        &values[4], -100.0f, 100.0f, true, SKYBLUE);

        EndDrawing(); // Завершуємо малювання кадру
    }

    // Закриваємо вікно і звільняємо ресурси
    CloseWindow();

    return 0;
}

