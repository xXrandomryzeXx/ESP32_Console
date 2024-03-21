#include "sprite.h"
#include "decode_image.h"
#include "memory.c"

#include <string.h>

struct sprite sprites[SPRITES];

uint8_t get_free_index()
{
    for(uint8_t i = 0; i < SPRITES; i++){
        if(!sprites[i].is_active)
            return i;
    }
    return -1;
}

uint8_t create_sprite(char* file_name, uint16_t x, uint8_t y, uint8_t size)
{
    uint8_t index = get_free_index();

    if(index != -1){
        sprites[index].x = x;
        sprites[index].y = y;
        if(size == 1){
            sprites[index].width = 64;
            sprites[index].height = 64;
            sprites[index].pixels = malloc(64*64*sizeof(uint16_t));
        }
        sprites[index].is_active = 1;

        // Load image from SD card

        // Create path
        char *path = malloc(strlen("/sdcard/kanji/") + strlen(file_name) + strlen(".jpg") + 1);
        strcpy(path, "/sdcard/kanji/");
        strcat(path, file_name);
        strcat(path, ".jpg");
        printf("%s\n", path);
        
        s_load_image(path, &sprites[index].pixels, size);
    }
    return index;
}

uint16_t get_sprite_pixel(uint16_t x, uint8_t y, uint8_t id)
{
   return (uint16_t) *(sprites[id].pixels + (y * sprites[id].width) + x);
}
