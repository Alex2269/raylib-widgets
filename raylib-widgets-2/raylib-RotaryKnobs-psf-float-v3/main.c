// main.c

#include "raylib.h"
#include "knob_gui.h"
#include "LoadFontUnicode.h"
#include <stdio.h>

int fontSize = 24;   // Розмір шрифту для тексту
int LineSpacing = 0; // Міжрядковий інтервал (0 - стандартний)
Font font;           // Об'єкт шрифту

#include "psf_font.h"
// #include "GlyphCache.h"

PSF_Font font12;
PSF_Font font18;
PSF_Font font20;
PSF_Font font22;
PSF_Font font24;
PSF_Font font28;
PSF_Font font32;

int spacing = 2; // Відступ між символами, той самий, що передається у DrawPSFText

int main(void)
{
    const int screenWidth = 800;  // Ширина вікна
    const int screenHeight = 450; // Висота вікна

    // Встановлюємо прапорець для мультисемплінгу (покращення якості графіки)
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Ініціалізуємо вікно з назвою
    InitWindow(screenWidth, screenHeight, "Raylib Knob Example");
    font = LoadFontUnicode("fonts/bold.ttf", fontSize, LineSpacing);

    // Завантаження PSF шрифту (шлях до вашого файлу)
    font12 = LoadPSFFont("fonts/Uni3-Terminus12x6.psf");
    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");
    font20 = LoadPSFFont("fonts/Uni3-TerminusBold20x10.psf");
    font22 = LoadPSFFont("fonts/Uni3-TerminusBold22x11.psf");
    font24 = LoadPSFFont("fonts/Uni3-TerminusBold24x12.psf");
    font28 = LoadPSFFont("fonts/Uni3-TerminusBold28x14.psf");
    font32 = LoadPSFFont("fonts/Uni3-TerminusBold32x16.psf");

    // Встановлюємо цільову частоту кадрів
    SetTargetFPS(60);

    // Масив значень регуляторів (float від 0.1f до 100.0f)
    float values[5] = { 0.0f, 2.25f, 5.5f, 3.0f, 55.0f };

    float knob_radius = 44.0f; // Радіус регуляторів

    // Основний цикл програми, виконується поки вікно не закриють
    while (!WindowShouldClose())
    {
        BeginDrawing();          // Починаємо малювання кадру
        ClearBackground(RAYWHITE); // Очищаємо фон білим кольором

        Gui_Knob_Channel(0, 600, 200,
                        TextFormat("Масштабування%d\nпо вертикалі", (int)values[0] + 1),
                        /*TextFormat("%0.1f", values[0])*/NULL,
                        knob_radius*2,
                        &values[0], 0.0f, 5.0f, true, YELLOW);


        Gui_Knob_Channel(1, 100 + knob_radius, 200,
                        TextFormat("Масштабування%d\nпо вертикалі", (int)values[0] + 1),
                        /*TextFormat("%0.1f", values[0])*/NULL,
                        knob_radius,
                        &values[1], 0.0f, 5.0f, true, WHITE);

        Gui_Knob_Channel(2, 300 + knob_radius, 200,
                        TextFormat("Масштабування%d\nпо горизонталі", (int)values[1] + 1),
                        /*TextFormat("%0.1f", values[1])*/NULL,
                        knob_radius,
                        &values[2], -20.0f, 20.0f, true, GREEN);

        Gui_Knob_Channel(3, 100 + knob_radius, 350,
                        TextFormat("Масштабування%d\nпо вертикалі", (int)values[0] + 1),
                        /*TextFormat("%0.1f", values[0])*/NULL,
                        knob_radius,
                        &values[3], -2.0f, 5.0f, true, DARKGREEN);

        Gui_Knob_Channel(4, 300 + knob_radius, 350,
                        TextFormat("Масштабування%d\nпо горизонталі", (int)values[1] + 1),
                        /*TextFormat("%0.1f", values[1])*/NULL,
                        knob_radius,
                        &values[4], -100.0f, 100.0f, true, SKYBLUE);


        EndDrawing(); // Завершуємо малювання кадру
    }

    // Після виходу з циклу звільняємо пам'ять шрифту
    // GlyphCache_ClearAllCaches();

    UnloadPSFFont(font12);
    UnloadPSFFont(font18);
    UnloadPSFFont(font20);
    UnloadPSFFont(font22);
    UnloadPSFFont(font24);
    UnloadPSFFont(font28);
    UnloadPSFFont(font32);

    // Закриваємо вікно і звільняємо ресурси
    CloseWindow();

    return 0;
}

