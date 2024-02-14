#pragma once
#include <stdint.h>

#define SPRITES 10

typedef struct{
    uint8_t width, height;
    uint8_t x, y;
    uint8_t is_active;
    uint16_t **pixels;
    uint8_t id;
} sprite;

void create_sprite(uint8_t size, char* file_name, uint8_t id);

void set_sprite_position(uint8_t x, uint8_t y, uint8_t id);

void delete_sprite(uint8_t id);
