#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Магічні числа для розпізнавання форматів PSF1
#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1_256 0x04
#define PSF1_MAGIC1_512 0x05

// Магічне число для PSF2 файлів, використовується для перевірки формату
#define PSF2_MAGIC 0x864ab572

// Максимальна довжина UTF-8 символу в байтах
#define MAX_UTF8_BYTES 4

/**
 * Структура заголовку PSF1 шрифту.
 * Використовується для зчитування основних параметрів PSF1 файлу.
 */
typedef struct {
    unsigned char magic[2];  // Магічні байти для ідентифікації формату PSF1
    unsigned char mode;      // Режим: 256 або 512 гліфів
    unsigned char charsize;  // Розмір одного гліфа в байтах
} psf1_header_t;

/**
 * Структура заголовку PSF2 шрифту.
 * Описує основні параметри PSF2 файлу.
 */
typedef struct {
    uint32_t magic;       // Магічне число PSF2 (має дорівнювати PSF2_MAGIC)
    uint32_t version;     // Версія формату (зазвичай 0)
    uint32_t headersize;  // Розмір заголовку в байтах
    uint32_t flags;       // Прапори (не завжди використовуються)
    uint32_t length;      // Кількість гліфів (символів) у шрифті
    uint32_t charsize;    // Розмір одного гліфа (bitmap) у байтах
    uint32_t height;      // Висота гліфа в пікселях
    uint32_t width;       // Ширина гліфа в пікселях
} psf2_header_t;

/**
 * Структура для зберігання відповідності Unicode символу та індексу гліфа.
 */
typedef struct {
    uint32_t unicode;      // Unicode код символу
    uint32_t glyph_index;  // Індекс гліфа у шрифті
} UnicodeGlyphMap;

/**
 * Декодує UTF-8 послідовність у Unicode кодову точку.
 *
 * @param buf Вказівник на масив байтів UTF-8 послідовності.
 * @param buflen Довжина масиву (максимальна кількість байтів для декодування).
 * @param out_cp Вказівник на змінну, куди буде записаний розкодований Unicode codepoint.
 *
 * @return Кількість байтів, які було прочитано з buf для одного символу UTF-8.
 *         Якщо послідовність некоректна, повертає 0.
 *
 * Пояснення:
 * - UTF-8 кодує символи від 1 до 4 байтів.
 * - Функція перевіряє перший байт, визначає довжину послідовності,
 *   і розкодовує символ у 32-бітне число Unicode.
 */
int utf8_decode(const unsigned char *buf, int buflen, uint32_t *out_cp) {
    if (buflen == 0) return 0;  // Якщо довжина 0 — нічого декодувати

    unsigned char c = buf[0];

    if (c < 0x80) {
        // Однобайтовий символ ASCII (0xxxxxxx)
        *out_cp = c;
        return 1;
    } else if ((c & 0xE0) == 0xC0 && buflen >= 2) {
        // Двобайтовий символ (110xxxxx 10xxxxxx)
        *out_cp = ((c & 0x1F) << 6) | (buf[1] & 0x3F);
        return 2;
    } else if ((c & 0xF0) == 0xE0 && buflen >= 3) {
        // Трибайтовий символ (1110xxxx 10xxxxxx 10xxxxxx)
        *out_cp = ((c & 0x0F) << 12) | ((buf[1] & 0x3F) << 6) | (buf[2] & 0x3F);
        return 3;
    } else if ((c & 0xF8) == 0xF0 && buflen >= 4) {
        // Чотирибайтовий символ (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
        *out_cp = ((c & 0x07) << 18) | ((buf[1] & 0x3F) << 12) | ((buf[2] & 0x3F) << 6) | (buf[3] & 0x3F);
        return 4;
    }

    // Некоректна UTF-8 послідовність
    return 0;
}

/**
 * Зчитує 16-бітне число у форматі little-endian з файлу.
 *
 * @param f Вказівник на відкритий файл.
 * @return Прочитане 16-бітне число, або 0xFFFF у випадку помилки.
 *
 * Пояснення:
 * - Зчитує два байти по одному з файлу.
 * - Перший байт — молодший (little-endian порядок).
 * - Другий байт — старший.
 */
uint16_t read_le16(FILE *f) {
    int lo = fgetc(f);
    int hi = fgetc(f);
    if (lo == EOF || hi == EOF) return 0xFFFF;  // Позначка помилки
    return (uint16_t)(lo | (hi << 8));
}

/**
 * Визначає формат PSF файлу (PSF1 або PSF2).
 *
 * @param f Вказівник на відкритий файл.
 * @param format Вказівник на змінну, куди буде записано номер формату (1 або 2).
 * @return 1, якщо формат визначено успішно, 0 — якщо ні.
 *
 * Пояснення:
 * - Зчитує перші 4 байти файлу.
 * - Порівнює з магічними числами PSF2 та PSF1.
 * - Визначає формат і повертає його через параметр format.
 * - Після перевірки повертає позицію читання в початок файлу.
 */
int detect_psf_format(FILE *f, int *format) {
    unsigned char magic4[4];
    if (fread(magic4, 1, 4, f) != 4) return 0;

    uint32_t magic32 = magic4[0] | (magic4[1] << 8) | (magic4[2] << 16) | (magic4[3] << 24);
    if (magic32 == PSF2_MAGIC) {
        *format = 2;
        fseek(f, 0, SEEK_SET);
        return 1;
    }

    if (magic4[0] == PSF1_MAGIC0 && (magic4[1] == PSF1_MAGIC1_256 || magic4[1] == PSF1_MAGIC1_512)) {
        *format = 1;
        fseek(f, 0, SEEK_SET);
        return 1;
    }

    return 0;
}

/**
 * Конвертує Unicode кодову точку у UTF-8 послідовність.
 *
 * @param code Unicode код символу.
 * @param utf8_char Масив символів, куди буде записано UTF-8 (повинен бути не менше 5 байтів).
 *
 * Пояснення:
 * - Записує UTF-8 представлення коду у масив utf8_char.
 * - Додає нуль-термінатор у кінець.
 */
void unicode_to_utf8(uint32_t code, char *utf8_char) {
    if (code <= 0x7F) {
        utf8_char[0] = (char)code;
        utf8_char[1] = 0;
    } else if (code <= 0x7FF) {
        utf8_char[0] = 0xC0 | (code >> 6);
        utf8_char[1] = 0x80 | (code & 0x3F);
        utf8_char[2] = 0;
    } else if (code <= 0xFFFF) {
        utf8_char[0] = 0xE0 | (code >> 12);
        utf8_char[1] = 0x80 | ((code >> 6) & 0x3F);
        utf8_char[2] = 0x80 | (code & 0x3F);
        utf8_char[3] = 0;
    } else {
        utf8_char[0] = 0xF0 | (code >> 18);
        utf8_char[1] = 0x80 | ((code >> 12) & 0x3F);
        utf8_char[2] = 0x80 | ((code >> 6) & 0x3F);
        utf8_char[3] = 0x80 | (code & 0x3F);
        utf8_char[4] = 0;
    }
}

/**
 * Головна функція програми.
 * Приймає один аргумент командного рядка — шлях до PSF файлу.
 * Визначає формат файлу, зчитує Unicode → гліф відповідність для кирилиці,
 * і виводить її у вигляді статичного масиву.
 */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Використання: %s <psf-файл>\n", argv[0]);
        return 1;
    }

    // Відкриваємо файл у двійковому режимі
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("Не вдалося відкрити файл");
        return 1;
    }

    int psf_format = 0;
    // Визначаємо формат PSF файлу (1 або 2)
    if (!detect_psf_format(f, &psf_format)) {
        fprintf(stderr, "Формат файлу не підтримується або файл пошкоджений\n");
        fclose(f);
        return 1;
    }

    // Вивід заголовку масиву відповідності
    printf("\n// Таблиця відповідності Unicode → індекс гліфа для кирилиці\n");
    printf("// Кожен запис містить пару: {Unicode код, Індекс гліфа}, а також коментар з літерою\n");
    printf("static UnicodeGlyphMap cyr_map[] = {\n");

    int count = 0;

    if (psf_format == 1) {
        // Обробка PSF1 формату
        psf1_header_t header;
        // Зчитуємо заголовок PSF1 (4 байти)
        if (fread(header.magic, 1, 2, f) != 2 ||
            fread(&header.mode, 1, 1, f) != 1 ||
            fread(&header.charsize, 1, 1, f) != 1) {
            fprintf(stderr, "Не вдалося прочитати заголовок PSF1\n");
            fclose(f);
            return 1;
        }

        // Визначаємо кількість гліфів: 256 або 512
        uint32_t glyph_count = (header.magic[1] == PSF1_MAGIC1_512) ? 512 : 256;
        uint32_t charsize = header.charsize;

        // Обчислюємо позицію Unicode mapping у файлі PSF1
        long offset = 4 + (long)charsize * glyph_count;
        if (fseek(f, offset, SEEK_SET) != 0) {
            fprintf(stderr, "Помилка переходу до Unicode mapping PSF1\n");
            fclose(f);
            return 1;
        }

        // Читаємо Unicode коди для кожного гліфа
        for (uint32_t glyph_index = 0; glyph_index < glyph_count; glyph_index++) {
            int end_of_glyph = 0;
            while (!end_of_glyph) {
                uint16_t code = read_le16(f);
                if (code == 0xFFFF) {
                    // Кінець списку Unicode кодів для цього гліфа
                    end_of_glyph = 1;
                    continue;
                }
                // Виводимо лише символи кирилиці (U+0400–U+04FF)
                if (code >= 0x0400 && code <= 0x04FF) {
                    char utf8_char[5] = {0};
                    unicode_to_utf8(code, utf8_char);
                    printf("  {0x%04X ,0x%03X}, /*%s*/", code, glyph_index, utf8_char);
                    count++;
                    if (count % 3 == 0) printf("\n");
                }
            }
        }
    } else if (psf_format == 2) {
        // Обробка PSF2 формату
        psf2_header_t header;
        // Зчитуємо заголовок PSF2
        if (fread(&header, sizeof(header), 1, f) != 1) {
            fprintf(stderr, "Не вдалося прочитати заголовок PSF2\n");
            fclose(f);
            return 1;
        }

        // Перевіряємо магічне число
        if (header.magic != PSF2_MAGIC) {
            fprintf(stderr, "Це не PSF2 файл\n");
            fclose(f);
            return 1;
        }

        uint32_t glyph_count = header.length;
        uint32_t charsize = header.charsize;

        // Обчислюємо позицію Unicode mapping у файлі PSF2
        long offset = header.headersize + (long)glyph_count * charsize;
        if (fseek(f, offset, SEEK_SET) != 0) {
            fprintf(stderr, "Помилка переходу до Unicode mapping PSF2\n");
            fclose(f);
            return 1;
        }

        // Для кожного гліфа читаємо Unicode коди у форматі UTF-8
        for (uint32_t glyph_index = 0; glyph_index < glyph_count; glyph_index++) {
            int end_of_glyph = 0;
            while (!end_of_glyph) {
                unsigned char buf[MAX_UTF8_BYTES];
                int cp_len = 0;
                uint32_t codepoint = 0;

                int c = fgetc(f);
                if (c == EOF) {
                    // Кінець файлу — припиняємо роботу
                    goto psf2_done;
                }
                if (c == 0xFF) {
                    // 0xFF — кінець списку Unicode кодів для поточного гліфа
                    end_of_glyph = 1;
                    continue;
                }

                buf[0] = (unsigned char)c;

                // Визначаємо довжину UTF-8 послідовності за першим байтом
                if ((buf[0] & 0x80) == 0) cp_len = 1;
                else if ((buf[0] & 0xE0) == 0xC0) cp_len = 2;
                else if ((buf[0] & 0xF0) == 0xE0) cp_len = 3;
                else if ((buf[0] & 0xF8) == 0xF0) cp_len = 4;
                else {
                    fprintf(stderr, "Невірний UTF-8 початковий байт: 0x%X\n", buf[0]);
                    fclose(f);
                    return 1;
                }

                // Читаємо залишок UTF-8 послідовності
                for (int i = 1; i < cp_len; i++) {
                    int next_c = fgetc(f);
                    if (next_c == EOF) {
                        fprintf(stderr, "Неочікуваний кінець файлу у UTF-8 послідовності\n");
                        fclose(f);
                        return 1;
                    }
                    buf[i] = (unsigned char)next_c;
                }

                // Декодуємо UTF-8 послідовність у Unicode codepoint
                int decoded_len = utf8_decode(buf, cp_len, &codepoint);
                if (decoded_len != cp_len) {
                    fprintf(stderr, "Помилка декодування UTF-8\n");
                    fclose(f);
                    return 1;
                }

                // Виводимо лише символи кирилиці (U+0400–U+04FF)
                if (codepoint >= 0x0400 && codepoint <= 0x04FF) {
                    printf("  {0x%04X ,0x%03X}, /*%.*s*/", codepoint, glyph_index, cp_len, buf);
                    count++;
                    if (count % 3 == 0) printf("\n");
                }
            }
        }
    psf2_done:
        ;
    }

    printf("};\n");

    fclose(f);
    return 0;
}

