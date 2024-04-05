#pragma once
#include <stdint.h>

#define SPRITES 10

struct sprite{
    uint8_t width, height;
    uint16_t x;
    uint8_t y;
    uint8_t is_active;
    uint16_t *pixels;
    uint8_t layer;
};

uint8_t create_sprite(char* file_name, uint16_t x, uint8_t y, uint8_t size);

uint16_t get_sprite_pixel(uint16_t x, uint8_t y, uint8_t id);

void set_sprite_position(uint16_t x, uint8_t y, uint8_t id);
void set_sprite_layer(uint8_t layer, uint8_t id);

void delete_sprite(uint8_t id);
