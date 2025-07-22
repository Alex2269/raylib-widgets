#include "raylib.h"
#include "stdint.h"

#define WIDTH 800
#define HEIGHT 480
#define CELL_SIZE 40
#define BEGIN_RAY (CELL_SIZE * 4)
#define WIDTH_RAY (WIDTH - (CELL_SIZE-1))

#define MIN_Y_BORDER CELL_SIZE
#define MAX_Y_BORDER (HEIGHT - CELL_SIZE)

// Константи для кращої читабельності
#define ADC_RESOLUTION 4096 // Роздільна здатність АЦП (Analog-to-Digital Converter)

// Інші константи для меж та розмірів
#define X_WORKSPACE (WIDTH_RAY) // Максимальна межа для буфера по осі X
#define Y_WORKSPACE (MAX_Y_BORDER - MIN_Y_BORDER) // Робоча область по осі Y

// Макрос для обмеження значення в межах
#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))



int16_t scroll_x_a = 0;
int16_t scroll_x_b = 0; // Зміщення буферів по осі X для каналів A і B
int16_t scroll_y_a = 0;
int16_t scroll_y_b = 0; // Зміщення буферів по осі Y для каналів A і B
uint16_t trigger_a = 0; // Змінна тригера
uint16_t buffering = 0; // Статус буферизації (1 - буферизація включена, 0 - вимкнена)



#define SLIDER_WIDTH 10 // Ширина повзунка
// Вертикальні повзунки для налаштування каналів
Rectangle vSlider_a = {BEGIN_RAY - 24, MIN_Y_BORDER, SLIDER_WIDTH, MAX_Y_BORDER - CELL_SIZE}; // Повзунок для каналу A
float vSliderValue_a = 0.5f; // Початкове значення повзунка для каналу A (центр)

Rectangle vSlider_b = {WIDTH_RAY + 12, MIN_Y_BORDER, SLIDER_WIDTH, MAX_Y_BORDER - CELL_SIZE}; // Повзунок для каналу B
float vSliderValue_b = 0.5f; // Початкове значення повзунка для каналу B (центр)

// Горизонтальні повзунки для налаштування каналів
Rectangle hSlider_a = {BEGIN_RAY, 12, WIDTH - BEGIN_RAY - CELL_SIZE, SLIDER_WIDTH}; // Горизонтальний повзунок для каналу A
float hSliderValue_a = 0.5f; // Початкове значення горизонтального повзунка для каналу A (центр)

Rectangle hSlider_b = {BEGIN_RAY, HEIGHT - 28, WIDTH - BEGIN_RAY - CELL_SIZE, SLIDER_WIDTH}; // Горизонтальний повзунок для каналу B
float hSliderValue_b = 0.5f; // Початкове значення горизонтального повзунка для каналу B (центр)

// Повзунок для налаштування тригера
Rectangle vSlider_trig_a = {10, MIN_Y_BORDER, SLIDER_WIDTH*3, MAX_Y_BORDER - CELL_SIZE};
float vSlider_trigValue_a = 0.5f; // Початкове значення повзунка тригера (центр)


/* Оновлення вертикальних повзунків для каналів A і B */
void update_vertical_sliders(void) 
{
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) 
  {
    if (CheckCollisionPointRec(GetMousePosition(), vSlider_a)) 
    {
      vSliderValue_a = (GetMouseY() - vSlider_a.y) / vSlider_a.height;
      vSliderValue_a = fmaxf(0.0f, fminf(1.0f, vSliderValue_a));
      scroll_y_a = (int16_t)(vSliderValue_a * Y_WORKSPACE - Y_WORKSPACE / 2);
    }

    if (CheckCollisionPointRec(GetMousePosition(), vSlider_b)) 
    {
      vSliderValue_b = (GetMouseY() - vSlider_b.y) / vSlider_b.height;
      vSliderValue_b = fmaxf(0.0f, fminf(1.0f, vSliderValue_b));
      scroll_y_b = (int16_t)(vSliderValue_b * Y_WORKSPACE - Y_WORKSPACE / 2);
    }
  }
}

/* Оновлення горизонтальних повзунків для каналів A і B */
void update_horizontal_sliders(void) 
{
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) 
  {
    if (CheckCollisionPointRec(GetMousePosition(), hSlider_a)) 
    {
      hSliderValue_a = (GetMouseX() - hSlider_a.x) / hSlider_a.width;
      hSliderValue_a = fmaxf(0.0f, fminf(1.0f, hSliderValue_a));
      scroll_x_a = (int16_t)(hSliderValue_a * WIDTH_RAY);
      scroll_x_a = CLAMP(scroll_x_a, MIN_Y_BORDER, X_WORKSPACE); // Обмежуємо scroll_x_a
    }

    if (CheckCollisionPointRec(GetMousePosition(), hSlider_b)) 
    {
      hSliderValue_b = (GetMouseX() - hSlider_b.x) / hSlider_b.width;
      hSliderValue_b = fmaxf(0.0f, fminf(1.0f, hSliderValue_b));
      scroll_x_b = (int16_t)(hSliderValue_b * WIDTH_RAY);
      scroll_x_b = CLAMP(scroll_x_b, MIN_Y_BORDER, X_WORKSPACE); // Обмежуємо scroll_x_b
    }
  }
}

/* Оновлення вертикального повзунка для тригера */
void update_trigger_slider(void) 
{
  // Оновлення вертикального повзунка тригера
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
  CheckCollisionPointRec(GetMousePosition(), vSlider_trig_a)) 
  {
    vSlider_trigValue_a = (GetMouseY() - vSlider_trig_a.y) / vSlider_trig_a.height;

    if (vSlider_trigValue_a < 0.0f) vSlider_trigValue_a = 0.0f;
    if (vSlider_trigValue_a > 1.0f) vSlider_trigValue_a = 1.0f;

    trigger_a = ADC_RESOLUTION - (int16_t)(vSlider_trigValue_a * ADC_RESOLUTION);
    printf("trigger_a: %d\n", trigger_a);
  }
  // char valueText[10];
  // sprintf(valueText, "%d", trigger_a);  // Перетворюємо значення регулятора в текст
  // DrawText(valueText, 40, MAX_Y_BORDER + 20, 20, YELLOW); // Відображення значення
  DrawText(TextFormat("T: %04i", trigger_a), 40, MAX_Y_BORDER + 20, 20, GREEN);
}

/* Виклик обробки повзунків */
void update_sliders(void)
{
  // Обробка вертикальних повзунків для каналів A та B
  update_vertical_sliders();
  // Обробка горизонтальних повзунків для каналів A та B
  update_horizontal_sliders();
  // Обробка вертикального повзунка тригера
  update_trigger_slider();
}

/* Малюємо повзунки */
void draw_sliders(void)
{
  /* Малюємо вертикальні повзунки */
  draw_sliders_vertical();
  /* Малюємо горизонтальні повзунки */
  draw_sliders_horizontal();
  /* Малюємо вертикальний повзунок тригера */
  draw_slider_trigger();
}

/* Малюємо вертикальні повзунки */
void draw_sliders_vertical(void) 
{
  uint16_t KNOB = 10;
  // Малюємо вертикальний повзунок A
  DrawRectangleRoundedLines(vSlider_a, 2.0f, 10, SKYBLUE);
  DrawRectangle(vSlider_a.x, vSlider_a.y + vSliderValue_a * vSlider_a.height - 5, vSlider_a.width, KNOB, YELLOW);

  // Малюємо вертикальний повзунок B
  DrawRectangleRoundedLines(vSlider_b, 2.0f, 10, SKYBLUE);
  DrawRectangle(vSlider_b.x, vSlider_b.y + vSliderValue_b * vSlider_b.height - 5, vSlider_b.width, KNOB, GREEN);
}

/* Малюємо горизонтальні повзунки */
void draw_sliders_horizontal(void) 
{
  uint16_t KNOB = 10;
  //* Малюємо горизонтальні повзунки */ A
  DrawRectangleRoundedLines(hSlider_a, 2.0f, 10, SKYBLUE);
  DrawRectangle(hSlider_a.x + hSliderValue_a * hSlider_a.width - 5, hSlider_a.y, KNOB, hSlider_a.height, YELLOW);

  //* Малюємо горизонтальні повзунки */ B
  DrawRectangleRoundedLines(hSlider_b, 2.0f, 10, SKYBLUE);
  DrawRectangle(hSlider_b.x + hSliderValue_b * hSlider_b.width - 5, hSlider_b.y, KNOB, hSlider_b.height, GREEN);
}

/* Малюємо вертикальний повзунок тригера */
void draw_slider_trigger(void) 
{
  uint16_t KNOB = 10;
  // Малюємо вертикальний повзунок тригера
  DrawRectangleRoundedLines(vSlider_trig_a, 2.0f, 10, buffering ? GREEN : GRAY);
  DrawRectangle(vSlider_trig_a.x, vSlider_trig_a.y + vSlider_trigValue_a * vSlider_trig_a.height - 5, vSlider_trig_a.width, KNOB, buffering ? GREEN : GRAY);
}
