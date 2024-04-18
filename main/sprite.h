#pragma once
#include <stdint.h>

#define SPRITES 10

/*
 * width, height - Width and Height of the sprite
 * x             - X position 16-bit due to width of the screen being 320 > 8-bit(255)
 * y             - Y position 8-bit because height of the screen is 240 < 8-bit(255)
 * is_active     - Tells the code that this sprite is not null, should be drawn
 * pixels        - A pointer/array that stores all pixel data of the sprite
 * layer         - Layer between 1 and 2 to layer text and sprites
 */
struct sprite{
    uint8_t width, height;
    uint16_t x;
    uint8_t y;
    uint8_t is_active;
    uint16_t *pixels;
    uint8_t layer;
};

/*
 * @brief Create a new sprite, find a free spot in the sprites array and load the image from SD
 *
 * @param file_name    - Name of the file without the extention
 *        x            - 16-bit X position
 *        y            - 8-bit Y position
 *        size         - Size of the sprite 1-5(64x64, 32x32, 16x16, 8x8, 128x128)
 *
 * @return             - ID of the sprite (an index)
 */
uint8_t create_sprite(char* file_name, uint16_t x, uint8_t y, uint8_t size);

/*
 * @breif Used when drawing to the screen, returns the color of the pixel
 *
 * @param x            - 16-bit X position
 *        y            - 8-bit Y position
 *        id           - ID of the sprite
 *
 * @return             - Color of the pixel
 */
uint16_t get_sprite_pixel(uint16_t x, uint8_t y, uint8_t id);

/*
 * @brief Set the layer of the sprite
 *
 * @param layer        - New layer of the sprite
 *        id           - ID of the sprite
 */
void set_sprite_layer(uint8_t layer, uint8_t id);

/*
 * @brief Delete the sprite and free the memory used by it
 *
 * @param id           - ID of the sprite to be deleted
 */
void delete_sprite(uint8_t id);
