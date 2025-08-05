// LoadFontUnicode.h
#ifndef LOADFONTUNICODE_H
#define LOADFONTUNICODE_H

Font LoadFontUnicode(char * fnt, int fontSize, int LineSpacing)
{
    int codepointCount = 512;
    int codepoints[512] = { 0 };

    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;
    for (int i = 96; i < 351; i++) codepoints[i] = 928 + i;
    Font font = LoadFontEx(fnt, fontSize, codepoints, codepointCount);
    // Встановлення білінійної фільтрації для текстури шрифту
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    // Set line spacing for multiline text (when line breaks are included '\n')
    SetTextLineSpacing(LineSpacing);
    return font;
}

#endif /* LOADFONTUNICODE_H */

