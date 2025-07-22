// button.c
#include "button.h"
#include "raylib.h"

// Масив станів кнопок
bool buttonStates[10] = { false, false, false, false, false,
  false, false, false, false, false
};

// Массив прямокутників для кнопок
Rectangle buttons[10] = {
  {50, 50, 16, 16},
  {50, 90, 16, 16},
  {50, 130, 16, 16},
  {50, 170, 16, 16},
  {50, 210, 16, 16},
  {50, 250, 16, 16},
  {50, 290, 16, 16},
  {50, 330, 16, 16},
  {50, 370, 16, 16},
  {50, 410, 16, 16}
};

// массив надписів для кнопок
// button.c
const char *buttonLabels[10] = {
    "Ch 1", "Ch 2", "Bt 3",
    "Bt 4", "Bt 5", "Bt 6",
    "Bt 7", "Bt 8", "Trig",
    "Scale" // Додано мітку, хоча це не кнопка
};

Color buttonColors[10] = {
  YELLOW, GREEN, GREEN, GREEN, GREEN, GREEN,
  GREEN, GREEN, GREEN, GREEN
};

// Функція для малювання кнопок
void DrawButton(Rectangle buttonRect, bool buttonState, const char *label, Color color)
{
  Color buttonColor = buttonState ? color : LIGHTGRAY;
  DrawRectangleRec(buttonRect, buttonColor);
  DrawText(label, (int)(buttonRect.x + 25), (int)(buttonRect.y), 20, buttonState ? color : LIGHTGRAY);
}

// Ініціалізація кнопок вже здійснюється через масиви
void buttons_init(void)
{
  for (int i = 0; i < 10; i++)
  {
    Rectangle buttons[i];
  }
}

// Виклик малювання кнопок
void DrawButtons(void)
{
  for (int i = 0; i < 10; i++)
  {
    DrawButton(buttons[i], buttonStates[i], buttonLabels[i], buttonColors[i]);
  }
}

// Обробка натискання на кнопки
void HandleButtonClicks(void)
{
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
  {
    for (int i = 0; i < 10; i++)
    {
      HandleButtonClick(buttons[i], &buttonStates[i]);
    }
  }
}

// Функція для обробки натискання кнопок
void HandleButtonClick(Rectangle buttonRect, bool *buttonState)
{
  if (CheckCollisionPointRec(GetMousePosition(), buttonRect))
  {
    *buttonState = !*buttonState;
  }
}

