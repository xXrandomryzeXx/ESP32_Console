#pragma once
#include <stdint.h>
#include "esp_err.h"

#define TEXT_LENGTH 8

/*
 * char_codes - The indexes of the character inside the font array
 * len        - The number of characters, including spaces
 * x, y       - X and Y coordinates of the top left edge of the text
 * color      - 16-bit color value for the color of the char, usually black or white
 */
struct ui_string{
    uint8_t *char_codes;
    uint8_t len;
    uint16_t x;
    uint8_t y;
    uint16_t color;
    uint8_t layer;
};

/*
 * @bried Find a free space in the strings array, and create a new text there
 *
 * @param x     - X position, where to draw the top left position of the text
 *        y     - Y position, where to draw the top left position of the text
 *        text  - The text string that will be drawn to the screen
 *        len   - The length of the the given string in bytes
 *
 * @return      -  -1 Failed to allocate memory or there is no space in the array
 *              -  index Successfully created text
 */
int8_t create_text(uint16_t x, uint8_t y, char *text);


/*
 * @bried Find a free space in the japanese strings array, and create a new text there
 *
 * @param x     - X position, where to draw the top left position of the text
 *        y     - Y position, where to draw the top left position of the text
 *        text  - The text string that will be drawn to the screen
 *        len   - The length of the the given string in bytes
 *
 * @return      -  -1 Failed to allocate memory or there is no space in the array
 *              -  index + 10 Successfully created text
 */
int8_t create_japanese_text(uint16_t x, uint8_t y, char *text);

void update_text(uint16_t x, uint8_t y, uint8_t id);
/*
 * @brief Find and delete the text of the given id
 *
 * @param id    - ID of the text | if ID >= 10 then it's a japanese string
 *
 * @return      - 0 Failed to delete text because it hasn't been created
 *              - 1 Successfulyl deleted text
 */
uint8_t delete_text(uint8_t id);

/*
 * @brief Set the color variable of a string with a given id/index
 *
 * @param color - 16-bit color value to be set
 *        id    - ID of the string (index in the strings array)
 *
 * @return      - -1 The string has not been created
 *              -  1 Color has been successfully set
 */
uint8_t set_text_color(uint16_t color, uint8_t id);

/*
 * @brief Set the layer variable of a string with a given id/index
 *
 * @param layer - The layer on which the string will be
 *        id    - ID of the string (index in the strings array)
 *
 * @return      - 0 The string hasn't been created
 *              - 1 The layer has been successfully set
 */
uint8_t set_text_layer(uint8_t layer, uint8_t id);
/*
 * @breif Get a pixel value, if there should be a text pixel drawn or not, at given (x,y) coordinate
 *
 * @param x     - Global X position
 *        y     - Global Y position
 *        id    - ID of the string
 *
 * @return      - 0x0000 A "blank" pixel, don't draw a pixel
 *              - 0xffff A "draw" pixel, draw pixel with the strings[id].color color
 */
uint16_t get_text_pixel(uint16_t x, uint8_t y, uint8_t id);
uint16_t get_japanese_text_pixel(uint16_t x, uint8_t y, uint8_t id);

/*
 * @brtief Get the length of a text through the ID
 * 
 * @param id    - ID of the ui string
 *
 * @return      - The length of the string
 *              - 0 if it doesn't exist
 */
uint8_t get_text_len(uint8_t id);
