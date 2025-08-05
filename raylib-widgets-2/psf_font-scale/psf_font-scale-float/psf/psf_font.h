// psf_font.h
#ifndef PSF_FONT_H
#define PSF_FONT_H

#include "raylib.h"
#include <stdint.h>

// Структура шрифту PSF1/PSF2
typedef struct {
    int isPSF2;             // 0 - PSF1, 1 - PSF2
    int width;              // Ширина символу
    int height;             // Висота символу
    int charcount;          // Кількість символів
    int charsize;           // Розмір гліфа в байтах
    unsigned char* glyphBuffer; // Дані гліфів
} PSF_Font;

int utf8_decode(const char* str, uint32_t* out_codepoint);
int UnicodeToGlyphIndex(uint32_t codepoint);

PSF_Font LoadPSFFont(const char* filename);
void UnloadPSFFont(PSF_Font font);

/*
// Функція малювання тексту UTF-8 шрифтом PSF з підтримкою переносу рядків '\n'
void DrawPSFText(PSF_Font font, int x, int y, const char* text, int spacing, Color color);
void DrawPSFCharScaled(PSF_Font font, int x, int y, int c, int scale, Color color);
void DrawPSFTextScaled(PSF_Font font, int x, int y, const char* text, int spacing, int scale, Color color); */


#endif // PSF_FONT_H

