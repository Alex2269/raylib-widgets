// GlyphToImage.c
#include "GlyphToImage.h"

// Створює Image з гліфа PSF з заданим кольором
Image GlyphToImage(PSF_Font font, int glyphIndex, Color color) {
    int width = font.width;
    int height = font.height;
    int bytes_per_row = (width + 7) / 8;
    unsigned char* glyph = font.glyphBuffer + glyphIndex * font.charsize;

    Image img = GenImageColor(width, height, BLANK);

    for (int y = 0; y < height; y++) {
        for (int byte = 0; byte < bytes_per_row; byte++) {
            unsigned char bits = glyph[y * bytes_per_row + byte];
            for (int bit = 0; bit < 8; bit++) {
                int x = byte * 8 + bit;
                if (x >= width) break;
                if (bits & (0x80 >> bit)) {
                    // Малюємо білим кольором (255,255,255,255) - альфа маска
                    ImageDrawPixel(&img, x, y, WHITE);
                }
            }
        }
    }
    return img;
}

// Конвертує Image у Texture2D з вибором фільтра залежно від масштабу
Texture2D GlyphToTexture(PSF_Font font, int glyphIndex, float scale, Color color) {
    Image img = GlyphToImage(font, glyphIndex, color);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    // Вибір фільтра для масштабування: чіткість при цілочисельному, згладжування при дробовому
    if ((int)scale == scale) {
        SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    } else {
        SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
    }

    return tex;
}

