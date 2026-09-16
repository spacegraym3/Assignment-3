#include "barcode.h"
#include <stdio.h>
#include <string.h>

#define BARCODE_WHITE 255
#define BARCODE_BLACK 0
#define BARCODE_QUIET 9
#define BARCODE_DIGIT_WIDTH 7
#define BARCODE_MIDDLE_WIDTH 5
#define BARCODE_START_WIDTH 3
#define BARCODE_END_WIDTH 3

static void set_pixel(struct image* image, int x, int y, int value) {
    if (x < 0 || y < 0 || x >= image->width || y >= image->height) {
        return;
    }

    image->pixels[y * image->width + x].r = value;
    image->pixels[y * image->width + x].g = value;
    image->pixels[y * image->width + x].b = value;
}

static void draw_bar(struct image* image, int x, int value) {
    for (int y = 0; y < image->height; y++) {
        set_pixel(image, x, y, value);
    }
}

static void draw_pattern_bits(struct image* image, int start_x, int pattern, int bit_count) {
    for (int bit = 0; bit < bit_count; bit++) {
        int x = start_x + bit;
        int value = ((pattern >> (bit_count - 1 - bit)) & 1u) ? BARCODE_WHITE : BARCODE_BLACK;
        draw_bar(image, x, value);
    }
}

struct image* barcode(char* data, int width, int height) {
    if (data == NULL || width <= 0 || height <= 0) {
        return NULL;
    }

    const int left_patterns[10] = {
        0b1110010, 0b1100110, 0b1101100, 0b1010000,
        0b1011100, 0b1001110, 0b1000010, 0b1000100,
        0b1001000, 0b1110100
    };

    struct image* img = malloc(sizeof(struct image));
    if (img == NULL) {
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->pixels = calloc((size_t) width * (size_t) height, sizeof(struct pixel));
    if (img->pixels == NULL) {
        free(img);
        return NULL;
    }

    for (int i = 0; i < width * height; i++) {
        img->pixels[i].r = BARCODE_WHITE;
        img->pixels[i].g = BARCODE_WHITE;
        img->pixels[i].b = BARCODE_WHITE;
    }

    int digits_len = (int) strlen(data);
    if (digits_len != 12) {
        return img;
    }

    int x = BARCODE_QUIET;
    draw_pattern_bits(img, x, 0b010, BARCODE_START_WIDTH);
    x += BARCODE_START_WIDTH;

    for (int i = 0; i < 6; i++) {
        int digit = data[i] - '0';
        int pattern = left_patterns[digit];
        draw_pattern_bits(img, x, pattern, BARCODE_DIGIT_WIDTH);
        x += BARCODE_DIGIT_WIDTH;
    }

    draw_pattern_bits(img, x, 0b10101, BARCODE_MIDDLE_WIDTH);
    x += BARCODE_MIDDLE_WIDTH;

    for (int i = 6; i < 12; i++) {
        int digit = data[i] - '0';
        int pattern = left_patterns[digit] ^ 0b1111111;
        draw_pattern_bits(img, x, pattern, BARCODE_DIGIT_WIDTH);
        x += BARCODE_DIGIT_WIDTH;
    }

    draw_pattern_bits(img, x, 0b010, BARCODE_END_WIDTH);
    x += BARCODE_END_WIDTH;

    return img;
}

