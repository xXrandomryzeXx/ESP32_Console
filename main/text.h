#pragma once
#include <stdint.h>
#include "esp_err.h"

/*
 * char_codes - The indexes of the character inside the font array
 * len        - The number of characters, including spaces
 * x, y       - X and Y coordinates of the top left edge of the text
 * color      - 16-bit color value for the color of the char, usually black or white
 */
struct ui_string{
    uint8_t *char_codes;
    uint8_t len;
    uint8_t x, y;
    uint16_t color;
};

uint8_t create_text(uint8_t x, uint8_t y, char *text, int len);
uint8_t set_text_color(uint16_t color, uint8_t id);
uint16_t get_text_pixel(uint8_t x, uint8_t y, uint8_t id);
