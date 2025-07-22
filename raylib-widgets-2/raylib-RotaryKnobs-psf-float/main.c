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
    float values[4] = { 0.0f, 25.0f, 50.0f, 75.0f };

    float radius = 30.0f; // Радіус регуляторів

    // Основний цикл програми, виконується поки вікно не закриють
    while (!WindowShouldClose())
    {
        BeginDrawing();          // Починаємо малювання кадру
        ClearBackground(RAYWHITE); // Очищаємо фон білим кольором


        int knob_radius = 40;
        Gui_Knob_Channel(0, 100 + knob_radius, 100,
                        TextFormat("Масштабування%d\nпо вертикалі", (int)values[0] + 1),
                        TextFormat("%0.1f", values[0]),
                        knob_radius,
                        &values[0], 0.1f, 100.0f, true, WHITE);

        Gui_Knob_Channel(1, 300 + knob_radius, 100,
                        TextFormat("Масштабування%d\nпо горизонталі", (int)values[1] + 1),
                        TextFormat("%0.1f", values[1]),
                        knob_radius,
                        &values[1], 0.1f, 100.0f, true, WHITE);


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

