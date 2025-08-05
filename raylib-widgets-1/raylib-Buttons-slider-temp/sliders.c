// sliders.c
#include "sliders.h"

#define WIDTH 800
#define HEIGHT 480
#define CELL_SIZE 40
#define BEGIN_RAY (CELL_SIZE * 4)
#define WIDTH_RAY (WIDTH - (CELL_SIZE - 1))
#define HALF_RAY ((WIDTH_RAY - BEGIN_RAY) / 2)
#define MIN_Y_BORDER CELL_SIZE
#define MAX_Y_BORDER (HEIGHT - CELL_SIZE)

#define ADC_RESOLUTION 4096
#define BUFFER_NOISE_REDUCTION 1.0f
#define TEST_SIGNAL_MULTIPLIER 0.5f
#define TEST_SIGNAL_OFFSET 0.0f
#define TRIGGER_MIN 50
#define TRIGGER_MAX 4000

#define MAX_X_BUFFER WIDTH_RAY
#define MIN_X_BUFFER 0
#define MAX_Y_BUFFER (HEIGHT - CELL_SIZE)
#define MIN_Y_BUFFER CELL_SIZE

#define Y_WORKSPACE (MAX_Y_BUFFER - MIN_Y_BUFFER)
#define X_WORKSPACE (WIDTH - (BEGIN_RAY + CELL_SIZE))

#define SLIDER_WIDTH 24
#define SLIDER_KNOB_SIZE 10

#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

extern uint16_t buffering;
extern int16_t scroll_x_a, scroll_x_b;
extern int16_t scroll_y_a, scroll_y_b;
extern int16_t trigger_a;

// // Структура Slider з полем is_active
// typedef struct {
//   Rectangle rect;               // Прямокутник, що визначає область повзунка
//   float value;                  // Поточне значення повзунка (від 0.0 до 1.0)
//   int16_t *scroll;              // Вказівник на змінну, що змінюється при русі повзунка
//   bool is_vertical;             // Ознака вертикального (true) або горизонтального (false) повзунка
//   bool is_active;               // Чи активний слайдер для взаємодії
// } Slider;

// Масив повзунків для керування сигналом
Slider sliders[] = {
  {{BEGIN_RAY - 24, MIN_Y_BORDER, SLIDER_WIDTH, MAX_Y_BORDER - CELL_SIZE}, 0.5f, &scroll_y_a, true, false},    // Лівий вертикальний повзунок
  {{WIDTH_RAY + 12, MIN_Y_BORDER, SLIDER_WIDTH, MAX_Y_BORDER - CELL_SIZE}, 0.5f, &scroll_y_b, true, false},    // Правий вертикальний повзунок
  {{BEGIN_RAY, 12, WIDTH - BEGIN_RAY - CELL_SIZE, SLIDER_WIDTH}, 0.5f, &scroll_x_a, false, false},     // Верхній горизонтальний повзунок
  {{BEGIN_RAY, HEIGHT - 28, WIDTH - BEGIN_RAY - CELL_SIZE, SLIDER_WIDTH}, 0.5f, &scroll_x_b, false, false},    // Нижній горизонтальний повзунок
  {{24, MIN_Y_BORDER, SLIDER_WIDTH, MAX_Y_BORDER - CELL_SIZE}, 0.5f, &trigger_a, true, false}  // Вертикальний повзунок тригера
};

Rectangle triggerBtn;           // Кнопка тригера

// Змінна для збереження активного слайдера
Slider *active_slider = NULL;

// ==================================================================

// Функція для отримання позиції миші та перевірки натискання кнопки
bool get_mouse_input(Vector2 *mouse_pos)
{
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    *mouse_pos = GetMousePosition();

    // Якщо миша натискається, перевіряємо, чи вона над активним слайдером
    for (int i = 0; i < sizeof(sliders) / sizeof(Slider); i++) {
      if (is_mouse_over_slider(&sliders[i], *mouse_pos)) {
        active_slider = &sliders[i];  // Встановлюємо активним поточний слайдер
        active_slider->is_active = true; // Позначаємо слайдер активним
        return true;
      }
    }
  } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && active_slider != NULL) {
    *mouse_pos = GetMousePosition();
    return true;
  } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    // Коли кнопка відпускається, деактивуємо слайдер
    if (active_slider != NULL) {
      active_slider->is_active = false;  // Відключаємо активність слайдера
      active_slider = NULL;  // Скидаємо активний слайдер
    }
  }
  return false;
}

// Функція для перевірки, чи миша знаходиться в межах слайдера
bool is_mouse_over_slider(Slider *slider, Vector2 mouse_pos)
{
    return CheckCollisionPointRec(mouse_pos, slider->rect);
}

// Функція для деактивації всіх слайдерів
void deactivate_all_sliders(void)
{
  active_slider = NULL;  // Неактивний слайдер
}

// Функція для малювання слайдера з урахуванням кольору ручки
void draw_slider(Slider s, Color color)
{
  uint16_t KNOB = SLIDER_KNOB_SIZE;

  // Малюємо фон повзунка
  DrawRectangleRoundedLines(s.rect, 2.0f, 10, SKYBLUE);

  // Колір ручки: червоний, якщо слайдер активний
  Color knob_color = s.is_active ? RED : color;  // Якщо слайдер активний, то ручка червона, інакше базовий колір

  // Малюємо ручку повзунка у відповідному положенні
  if (s.is_vertical) {
    DrawRectangle(s.rect.x, s.rect.y + s.value * s.rect.height - 5, s.rect.width, KNOB, knob_color);
  } else {
    DrawRectangle(s.rect.x + s.value * s.rect.width - 5, s.rect.y, KNOB, s.rect.height, knob_color);
  }
}

// Оновлена функція малювання всіх слайдерів з урахуванням кольору ручки
void draw_sliders(void)
{
  for (int i = 0; i < (sizeof(sliders) / sizeof(Slider)) - 1; i++) { // усі крім тригера
    Color color = (i % 2 == 0) ? YELLOW : GREEN;  // Початковий колір ручки
    draw_slider(sliders[i], color);  // Малюємо слайдери з відповідним кольором
  }
  draw_slider(sliders[4], buffering ? GREEN : GRAY);   // Малюємо повзунок тригера
  // draw_slider(t_slider[0], buffering ? GREEN : GRAY);   // Малюємо повзунок тригера
}

// Оновлена функція для обробки натискання миші і перевірки активного слайдера
void update_sliders(void)
{
  Vector2 mouse_pos;
  if (get_mouse_input(&mouse_pos)) {
    // Оновлюємо тільки активний слайдер
    if (active_slider != NULL) {
      update_slider_value(active_slider, mouse_pos);
    }
  } else {
    if (active_slider != NULL) {
      // Якщо миша відпущена, зупиняємо оновлення
      active_slider->is_active = false;  // Деактивуємо слайдер після відпускання кнопки миші
      active_slider = NULL;
    }
  }
}

// Функція для оновлення значення повзунка на основі положення миші
void update_slider_value(Slider *s, Vector2 mouse)
{
  if (s->is_active) {
    s->value = (s->is_vertical ? (mouse.y - s->rect.y) / s->rect.height : (mouse.x - s->rect.x) / s->rect.width);
    s->value = CLAMP(s->value, 0.0f, 1.0f);

    if (s->is_vertical) {
      *s->scroll = (int16_t) (s->value * Y_WORKSPACE - Y_WORKSPACE / 2);
      *s->scroll = CLAMP(*s->scroll, MIN_Y_BUFFER - Y_WORKSPACE / 2, MAX_Y_BUFFER - Y_WORKSPACE / 2);
    } else {
      *s->scroll = (int16_t) (s->value * WIDTH_RAY);
      *s->scroll = CLAMP(*s->scroll, MIN_X_BUFFER, MAX_X_BUFFER);
    }
    if (s == sliders+4) // якщо це слайдер тригера
    {
      *s->scroll = (int16_t) Y_WORKSPACE - (s->value * Y_WORKSPACE);
      *s->scroll = CLAMP(*s->scroll, TRIGGER_MIN, TRIGGER_MAX);
      trigger_a = *s->scroll;
    }
  }
}
/*
// Оновлена функція для обробки вертикального слайдера тригера
void update_trigger_slider(void)
{
  Vector2 mouse_pos;
  if (get_mouse_input(&mouse_pos)) {
    for (int i = 0; i < sizeof(t_slider) / sizeof(Slider); i++) {
      Slider *s = &t_slider[i];
      if (s->is_active && CheckCollisionPointRec(mouse_pos, s->rect)) { // Перевірка активності слайдера
        s->value = (mouse_pos.y - s->rect.y) / s->rect.height;
        s->value = CLAMP(s->value, 0.0f, 1.0f);
        *s->scroll = (int16_t) Y_WORKSPACE - (s->value * Y_WORKSPACE);
        *s->scroll = CLAMP(*s->scroll, TRIGGER_MIN, TRIGGER_MAX);
        trigger_a = *s->scroll;
      }
    }
  }
}*/

// Функція обробки натискання кнопки тригера
void update_trigger_button(void)
{
  if (CheckCollisionPointRec(GetMousePosition(), triggerBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    buffering = !buffering;     // Зміна стану буферизації
    printf("Тригер перемикається: %d\n", buffering); // Вивід у консоль нового стану
  }
}

// Функція малювання кнопки тригера
void draw_trigger_button(void)
{
  triggerBtn = (Rectangle) {
  24, 450, 16, 16};             // Визначаємо позицію та розмір кнопки тригера
  DrawRectangleRounded(triggerBtn, 2.0f, 10, buffering ? GREEN : GRAY); //
  // Вивід тексту на кнопку (закоментовано)
  // DrawText("Buffering", triggerBtn.x + 20, triggerBtn.y, 20, buffering ? GREEN : GRAY);
}

