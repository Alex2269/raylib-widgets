#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "LoadFontUnicode.h"
#include "DrawSnow.h"
#include <math.h>

#define SCREEN_WIDTH 440
#define SCREEN_HEIGHT 440
#define CLOCK_RADIUS 210

int fontSize = 48;
int LineSpacing = 0;
Font font;
Font fontCalligraphy;

// Додайте в глобальні змінні
RenderTexture2D glassTexture;

// Десь у вашому файлі або в окремому util.h/c
float GetRandomFloat(float min, float max) {
    return (float)GetRandomValue(min * 1000, max * 1000) / 1000.0f;
}

// Це створить порожню текстуру розміром з ваш екран, на яку ми будемо малювати скло.
// Нова функція для малювання ефектів скла
void DrawGlassEffect(float elapsedTime) {
    // Починаємо малювати на нашу текстуру скла
    BeginTextureMode(glassTexture);
        // ClearBackground(BLANK); // Очищаємо текстуру (прозорий фон)

        Vector2 center = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};

        // --- Ефект відблиску (Glare/Highlight) ---
        // Можна зробити один або кілька відблисків.
        // Змінюємо позицію або розмір відблиску з часом для динамічності.
        float glareRadius = CLOCK_RADIUS * 0.5f;
        float glareAlpha = 0.6f; // Прозорість відблиску

        // Простий круговий відблиск
        DrawCircleGradient(center.x + sin(elapsedTime * 0.50f) * CLOCK_RADIUS * 0.2f,
                           center.y + cos(elapsedTime * 0.75f) * CLOCK_RADIUS * 0.2f,
                           glareRadius, WHITE, BLANK);

        // Більш реалістичний відблиск: великий, напівпрозорий, з градієнтом
        // Позиція відблиску може плавно змінюватись, імітуючи рух джерела світла або голови
        float glareX = center.x + sin(elapsedTime * 0.3f) * (CLOCK_RADIUS * 0.3f);
        float glareY = center.y + cos(elapsedTime * 0.4f + PI / 4) * (CLOCK_RADIUS * 0.3f);
        DrawCircleGradient(glareX, glareY,
                           CLOCK_RADIUS * 0.6f,
                           Fade(RAYWHITE, 0.2f), // Дуже світлий, напівпрозорий колір
                           Fade(BLANK, 0.0f)); // Повністю прозорий

        // Додаємо менший, яскравіший відблиск всередині
        DrawCircleGradient(glareX + CLOCK_RADIUS * 0.1f, glareY - CLOCK_RADIUS * 0.05f,
                           CLOCK_RADIUS * 0.3f,
                           Fade(SKYBLUE, 0.15f), // Можна додати легкий відтінок синього/блакитного
                           Fade(BLANK, 0.0f)); // Повністю прозорий


        // --- Легке затемнення по краях (Vignette effect) ---
        // Темне кільце по зовнішньому краю для ефекту лінзи
        DrawRing(center, CLOCK_RADIUS * 0.9f, CLOCK_RADIUS, 0, 360, 360, Fade(BLACK, 0.1f)); // Внутрішня частина затемнення
        DrawRing(center, CLOCK_RADIUS, CLOCK_RADIUS * 1.05f, 0, 360, 360, Fade(BLACK, 0.2f)); // Зовнішня частина затемнення (виходить за межі)

        // Також можна використати градієнтне коло для віньєтки:
        Color ColorGradient = (Color){ 120, 120, 120, 60 }; // Колір фону (дуже темний)
        DrawCircleGradient(center.x, center.y, CLOCK_RADIUS * 1.1f, ColorGradient, Fade(BLACK, 0.3f));

        // --- Ефект спотворення (Lens distortion) - більш складно без шейдерів ---
        // Без шейдерів це важко зробити. Можна симулювати, малюючи основний циферблат
        // на іншу текстуру, а потім малювати її спотворено.
        // Для простоти, в рамках цієї відповіді, ми цього не будемо робити,
        // оскільки це сильно ускладнить DrawClockFace.
        // Замість цього, сконцентруємось на відблисках та віньєтці.


        // --- Дрібні пилинки / подряпини (опціонально) ---
        // Можна намалювати декілька випадкових точок або тонких ліній
        // Для цього потрібен генератор випадкових чисел.
        // Наприклад, кілька "пилин"
        for (int i = 0; i < 5; i++) {
            float px = center.x + GetRandomFloat(-CLOCK_RADIUS * 0.45f, CLOCK_RADIUS * 0.45f);
            float py = center.y + GetRandomFloat(-CLOCK_RADIUS * 0.45f, CLOCK_RADIUS * 0.45f);
            DrawCircle(px, py, 3, Fade(WHITE, 0.3f));
        }

    EndTextureMode();
}

Texture2D hourHand, minuteHand, secondHand;
// Texture2D backgroundTexture;

const char* romanNumerals[] = {
  "XII", // для i = 0 (12 годин)
  "I",   // для i = 1
  "II",  // для i = 2
  "III", // для i = 3
  "IV",  // для i = 4
  "V",   // для i = 5
  "VI",  // для i = 6
  "VII", // для i = 7
  "VIII",// для i = 8
  "IX",  // для i = 9
  "X",   // для i = 10
  "XI"   // для i = 11
};

// Додайте цю функцію десь у ваш код (або замініть існуючу DrawGeometricBackground)
void DrawGeometricBackground() {
    Vector2 center = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    Color bgColor = (Color){ 20, 20, 20, 255 }; // Колір фону (дуже темний)

    // Замальовуємо весь фон
    // ClearBackground(bgColor);

    // Колір для елементів полярної сітки (темніший або приглушений)
    Color gridColor = (Color){ 60, 60, 60, 255 };

    // --- Малюємо концентричні кола (ізолінії радіуса) ---
    int numCircles = 10; // Кількість кіл
    float maxRadius = CLOCK_RADIUS * 0.9f; // Максимальний радіус сітки, трохи менше за циферблат

    for (int i = 1; i <= numCircles; i++) {
        float radius = (maxRadius / numCircles) * i; // Радіус поточного кола
        // Малюємо тонкий контур кола
        DrawCircleLines(center.x, center.y, radius, gridColor);
    }

    // --- Малюємо радіальні лінії (ізолінії кута) ---
    int numRadialLines = 24; // Кількість ліній, що виходять з центру (можна 12, 36, 60 тощо)
    float angleStep = 360.0f / numRadialLines; // Кут між лініями в градусах

    for (int i = 0; i < numRadialLines; i++) {
        float currentAngle = angleStep * i; // Поточний кут лінії

        // Кінцева точка лінії на максимальному радіусі
        Vector2 lineEnd = {
            center.x + cos(currentAngle * DEG2RAD) * maxRadius,
            center.y + sin(currentAngle * DEG2RAD) * maxRadius
        };

        // Малюємо лінію від центру до кінцевої точки
        DrawLineEx(center, lineEnd, 1.0f, gridColor); // Використовуємо DrawLineEx для контролю товщини
    }

    // Малюємо невелике коло в центрі, щоб приховати сходження ліній (опціонально)
     DrawCircle(center.x, center.y, 3, gridColor);
}

void DrawClockFace() {
  /*
  // Малюємо циферблат поверх фону за допомогою DrawRing для більшої кількості сегментів
  // Outer circle (filled)
  DrawRing((Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, // Центр
         0,                                // Внутрішній радіус (0 для заповненого кола)
         CLOCK_RADIUS,                    // Зовнішній радіус
         0,                                // Початковий кут (0 градусів)
         360,                              // Кінцевий кут (360 градусів для повного кола)
         360,                              // Кількість сегментів (чим більше, тим гладше)
         DARKGRAY);                       // Колір
  */

  // Inner circle (filled)
  /* DrawRing((Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
         0,
         CLOCK_RADIUS,
         0,
         360,
         360, // Використовуємо ту саму кількість сегментів
         (Color){ 40, 40, 40, 255 }); */

  // --- Малюємо малі риски для секунд/хвилин (60 штук) ---
  for (int i = 0; i < 60; i++) {
    // Кут для кожної риски (2*PI / 60)
    float angle = i * (PI / 30.0f) - PI / 2.0f; // - PI/2 для початку з 12 годин

    // Координати для малих рисок
    // Починаємо трохи далі від краю, ніж внутрішнє золоте коло
    int x1_min = SCREEN_WIDTH / 2 + cos(angle) * (CLOCK_RADIUS - 4);
    int y1_min = SCREEN_HEIGHT / 2 + sin(angle) * (CLOCK_RADIUS - 4);
    // Закінчуємо трохи ближче до центру
    int x2_min = SCREEN_WIDTH / 2 + cos(angle) * (CLOCK_RADIUS - 20); // Довжина риски ~16
    int y2_min = SCREEN_HEIGHT / 2 + sin(angle) * (CLOCK_RADIUS - 20);

    // DrawLine(x1_min, y1_min, x2_min, y2_min, SKYBLUE); // Колір для малих рисок
    // Використовуємо DrawLineEx для малювання товстої лінії
    DrawLineEx((Vector2){(float)x1_min, (float)y1_min}, // Початкова точка (потрібен Vector2)
               (Vector2){(float)x2_min, (float)y2_min}, // Кінцева точка (потрібен Vector2)
               2.0f,    // Товщина лінії (наприклад, 2.0)
               SKYBLUE); // Колір
  }

  // --- Малюємо товсті риски для годин (12 штук) та кільця ---
  for (int i = 0; i < 12; i++) {
    // Кут для позиціонування міток і цифр (радіальний вектор).
    // Починаємо з 12 годин (верх, -PI/2) і рухаємося за годинниковою стрілкою.
    float angle = i * (PI / 6.0f) - PI / 2.0f; // PI/6 = 30 градусів між годинами

    // Координати для товстих рисок годин
    // Починаємо трохи ближче до центру, ніж малі риски
    int x1_hr = SCREEN_WIDTH / 2 + cos(angle) * (CLOCK_RADIUS - 4);
    int y1_hr = SCREEN_HEIGHT / 2 + sin(angle) * (CLOCK_RADIUS - 4);
    // Закінчуємо далі від центру
    int x2_hr = SCREEN_WIDTH / 2 + cos(angle) * (CLOCK_RADIUS - 20); // Довжина риски ~16
    int y2_hr = SCREEN_HEIGHT / 2 + sin(angle) * (CLOCK_RADIUS - 20);

    // Використовуємо DrawLineEx для малювання товстої лінії
    DrawLineEx((Vector2){(float)x1_hr, (float)y1_hr}, // Початкова точка (потрібен Vector2)
               (Vector2){(float)x2_hr, (float)y2_hr}, // Кінцева точка (потрібен Vector2)
               4.0f,    // Товщина лінії (наприклад, 4.0)
               SKYBLUE); // Колір

    // --- Додаємо кільця на товстих рисках (на точках x1_h, y1_h) ---
    int x1_h = SCREEN_WIDTH / 2 + cos(angle) * (CLOCK_RADIUS - 12);
    int y1_h = SCREEN_HEIGHT / 2 + sin(angle) * (CLOCK_RADIUS - 12);
    DrawCircle(x1_h, y1_h, 5, GREEN); // Малюємо маленьке коло радіусом 5 GREEN

    // малювання тонкого кола над рисками
    DrawRing((Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, // Центр
           CLOCK_RADIUS - 4,      // Внутрішній радіус (0 для заповненого кола)
           CLOCK_RADIUS - 2,      // Зовнішній радіус
           0,                     // Початковий кут (0 градусів)
           360,                   // Кінцевий кут (360 градусів для повного кола)
           360,                   // Кількість сегментів (чим більше, тим гладше)
           SKYBLUE);              // Колір

    // малювання тонкого кола під рисками
    DrawRing((Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, // Центр
           CLOCK_RADIUS - 20,     // Внутрішній радіус (0 для заповненого кола)
           CLOCK_RADIUS - 18,     // Зовнішній радіус
           0,                     // Початковий кут (0 градусів)
           360,                   // Кінцевий кут (360 градусів для повного кола)
           360,                   // Кількість сегментів (чим більше, тим гладше)
           SKYBLUE);              // Колір

    Vector2 Offset = MeasureTextEx(fontCalligraphy, "Gothic\nClock", 48, 2);
    DrawTextEx(fontCalligraphy, "Gothic\nClock" ,
              (Vector2){
              (float)SCREEN_WIDTH / 2 - (Offset.x / 2.0f),
              (float) SCREEN_HEIGHT / 2 - (Offset.y / 2.0f)},
               48, 2, GOLD);

    // Додавання цифр (римських)
    const char* number_str = romanNumerals[i];

    Vector2 textOffset = MeasureTextEx(font, number_str, fontSize, 2);

    // Визначаємо бажану позицію центру тексту (трохи ближче до центру, ніж кільця)
    int numX_center = SCREEN_WIDTH / 2 + cos(angle) * (CLOCK_RADIUS - 45); // Далі від центру, ніж кінець риски
    int numY_center = SCREEN_HEIGHT / 2 + sin(angle) * (CLOCK_RADIUS - 45);

    // Розраховуємо базовий кут обертання для тексту:
    // радіальний кут (angle) плюс додаткові 90 градусів за годинниковою стрілкою (PI/2 радіан).
    float text_rotation_angle_rad = angle + PI / 2.0f;

    /* Якщо цифра знаходиться в нижній половині кола (приблизно від 4 до 8 годин),
     * додаємо ще 180 градусів, щоб цифра була "правильно" орієнтована.
     * Індекси 4 (IV), 5 (V), 6 (VI), 7 (VII), 8 (VIII) відповідають нижній частині. */
    if (i >= 4 && i <= 8) text_rotation_angle_rad += PI; // Додаємо 180 градусів (PI радіан)

    // Для 9 (IX) і 3 (III) також можна трохи підправити кут, але це вимагає тоншого налаштування.
    // Пропустимо це для простоти зараз.

    // Малюємо текст з обчисленим кутом обертання
    DrawTextPro(font,   // Шрифт
                number_str, // Текст цифри
                (Vector2){(float)numX_center, (float)numY_center}, // Позиція центру тексту на екрані
                (Vector2){textOffset.x / 2.0f, textOffset.y / 2.0f}, // Орігін обертання (центр самого тексту)
                text_rotation_angle_rad * RAD2DEG, // Кут обертання в градусах
                (float)fontSize, // Розмір шрифту
                -4.0f, // Інтервал між символами
                GOLD); // Колір тексту
  }
    // Малюємо маленьке коло по центру
    DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5, WHITE); // Це маленьке коло можна залишити DrawCircle
}

// Решта функцій (DrawRotatedTexture, DrawBackgroundTexture, main) без змін
void DrawRotatedTexture(Texture2D texture, float angle, float scale, Color color) {
  Vector2 origin = { texture.width / 2.0f, texture.height / 2.0f };
  Vector2 position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };

  DrawTexturePro(texture, (Rectangle){ 0, 0, (float)texture.width, (float)texture.height },
    (Rectangle){ position.x, position.y, (float)texture.width * scale, (float)texture.height * scale }, origin,
    angle * RAD2DEG, color);
}

void DrawBackgroundTexture(Texture2D texture, float angle, float scale) {
  Vector2 origin = { texture.width / 2.0f, texture.height / 2.0f };
  Vector2 position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };

  DrawTexturePro(texture, (Rectangle){ 0, 0, (float)texture.width, (float)texture.height },
                 (Rectangle){ position.x, position.y, (float)texture.width * scale, (float)texture.height * scale },
                 origin, angle, WHITE);
}

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT); // Залишаємо спробу ввімкнути MSAA, вона може працювати для інших елементів
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ретро Годинник");

  // В main(), після InitWindow() і завантаження шрифтів/стрілок:
  glassTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

  font = LoadFontUnicode("Gothicha.ttf", fontSize, LineSpacing);
  fontCalligraphy = LoadFontUnicode("Peristiwa.otf", 64, 60);
  SetTargetFPS(60);

  hourHand = LoadTexture("arrow_hours.png");
  SetTextureFilter(hourHand, TEXTURE_FILTER_BILINEAR);

  minuteHand = LoadTexture("arrow_minutes.png");
  SetTextureFilter(minuteHand, TEXTURE_FILTER_BILINEAR);

  secondHand = LoadTexture("arrow_seconds.png");
  SetTextureFilter(secondHand, TEXTURE_FILTER_BILINEAR);

  // backgroundTexture = LoadTexture("background.png");
  // SetTextureFilter(backgroundTexture, TEXTURE_FILTER_BILINEAR);


  while (!WindowShouldClose()) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    float secondAngle = (PI / 30.0f) * t->tm_sec - PI / 2.0f;
    float minuteAngle = (PI / 30.0f) * t->tm_min + (PI / 1800.0f) * t->tm_sec - PI / 2.0f;
    float hourAngle = (PI / 6.0f) * t->tm_hour + (PI / 360.0f) * t->tm_min - PI / 2.0f;

    BeginDrawing();

    // ClearBackground(BLACK); // Або інший фоновий колір
    DrawGeometricBackground(); // <-- Тут малюємо фон

    // !!! Викликайте функцію малювання складної сніжинки тут !!!
    Vector2 center = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    float snowflakeRadius = CLOCK_RADIUS * 0.35f; // Зменшимо розмір, щоб гілочки вмістилися
    int numArms = 12; // Кількість основних променів
    int depth = 4; // Глибина рекурсії (кількість розгалужень)
    float branchAngle = 15.0f; // Кут відхилення гілочок (в градусах)
    float branchScale = 0.55f; // Коефіцієнт зменшення довжини гілочок

    // Виклик функції для малювання сніжинки з розгалуженими гілочками
    DrawComplexSnowflake(center, snowflakeRadius, numArms, depth, branchAngle, branchScale, SKYBLUE);

    DrawClockFace(); // Малюємо циферблат поверх сніжинки

    // Накладаємо текстуру скла поверх усього з прозорістю
    // Важливо: для малювання текстури RenderTexture, її потрібно перевернути по Y,
    // оскільки OpenGL текстури можуть бути з початком координат у нижньому лівому куті.
    DrawTexturePro(glassTexture.texture,
                   (Rectangle){0, 0, (float)glassTexture.texture.width, (float)-glassTexture.texture.height}, // '-height' для перевороту
                   (Rectangle){0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
                   (Vector2){0, 0}, 0.0f, WHITE); // Малюємо на весь екран з нормальною прозорістю WHITE

    // Отримуємо поточний час для анімації відблиску
    float elapsedTime = GetTime();

    DrawGlassEffect(elapsedTime); // Малюємо ефекти скла на glassTexture

    // Колір для стрілок
    Color arowColor = (Color){ 100, 250, 250, 255 };
    // Малюємо стрілки
    DrawRotatedTexture(hourHand, hourAngle, 1.0f, arowColor);
    DrawRotatedTexture(minuteHand, minuteAngle, 1.0f, arowColor);
    DrawRotatedTexture(secondHand, secondAngle, 1.0f, arowColor);

    EndDrawing();

  }

  UnloadTexture(hourHand);
  UnloadTexture(minuteHand);
  UnloadTexture(secondHand);
  // UnloadTexture(backgroundTexture);
  UnloadFont(font);
  CloseWindow();
  return 0;
}




