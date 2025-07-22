// button.h
#ifndef BUTTON_H
#define BUTTON_H

typedef struct {
    Rectangle bounds;
    bool pressed;
    float animProgress; // 0..1
} AnimatedButton;

Color ColorLerp(Color c1, Color c2, float t);
void UpdateButton(AnimatedButton *btn);
void DrawButton(AnimatedButton *btn, const char *text, Font font, int fontSize, int lineSpacing, Color colorNormal, Color colorPressed);


#endif // BUTTON_H

