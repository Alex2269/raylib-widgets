#include "raylib.h"
#include "stdint.h"


extern int16_t scroll_x_a;
extern int16_t scroll_x_b; // Зміщення буферів по осі X для каналів A і B
extern int16_t scroll_y_a;
extern int16_t scroll_y_b; // Зміщення буферів по осі Y для каналів A і B
extern uint16_t trigger_a; // Змінна тригера
extern uint16_t buffering; // Статус буферизації (1 - буферизація включена, 0 - вимкнена)



#define SLIDER_WIDTH 10 // Ширина повзунка
// Вертикальні повзунки для налаштування каналів
extern Rectangle vSlider_a; // Повзунок для каналу A
extern float vSliderValue_a; // Початкове значення повзунка для каналу A (центр)

extern Rectangle vSlider_b; // Повзунок для каналу B
extern float vSliderValue_b; // Початкове значення повзунка для каналу B (центр)

// Горизонтальні повзунки для налаштування каналів
extern Rectangle hSlider_a; // Горизонтальний повзунок для каналу A
extern float hSliderValue_a; // Початкове значення горизонтального повзунка для каналу A (центр)

extern Rectangle hSlider_b; // Горизонтальний повзунок для каналу B
extern float hSliderValue_b; // Початкове значення горизонтального повзунка для каналу B (центр)

// Повзунок для налаштування тригера
extern Rectangle vSlider_trig_a;
extern float vSlider_trigValue_a; // Початкове значення повзунка тригера (центр)


/* Оновлення вертикальних повзунків для каналів A і B */
void update_vertical_sliders(void);
/* Оновлення горизонтальних повзунків для каналів A і B */
void update_horizontal_sliders(void);
/* Оновлення вертикального повзунка для тригера */
void update_trigger_slider(void);
/* Виклик обробки повзунків */
void update_sliders(void);
/* Малюємо повзунки */
void draw_sliders(void);
/* Малюємо вертикальні повзунки */
void draw_sliders_vertical(void);
/* Малюємо горизонтальні повзунки */
void draw_sliders_horizontal(void);
/* Малюємо вертикальний повзунок тригера */
void draw_slider_trigger(void);

