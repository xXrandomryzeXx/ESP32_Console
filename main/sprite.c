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
        }else if(size == 2){
            sprites[index].width = 32;
            sprites[index].height = 32;
            sprites[index].pixels = malloc(32*32*sizeof(uint16_t));
        }else if(size == 3){
            sprites[index].width = 16;
            sprites[index].height = 16;
            sprites[index].pixels = malloc(16*16*sizeof(uint16_t));
        }else if(size == 4){
            sprites[index].width = 8;
            sprites[index].height = 8;
            sprites[index].pixels = malloc(8*8*sizeof(uint16_t));
        }else if(size == 5){
            sprites[index].width = 128;
            sprites[index].height = 128;
            sprites[index].pixels = malloc(128*128*sizeof(uint16_t));
        }
        sprites[index].is_active = 1;
        sprites[index].layer = 0; /* default layer is 0 */

        /* Create path */
        char *path = malloc(strlen("/sdcard/") + strlen(file_name) + strlen(".jpg") + 1);
        strcpy(path, "/sdcard/");
        strcat(path, file_name);
        strcat(path, ".jpg");

        /* Load image from SD card */
        s_load_image(path, &sprites[index].pixels, size);
        free(path); /* Avoid memory leak */
    }
    return index;
}

void set_sprite_layer(uint8_t layer, uint8_t id)
{
    if(sprites[id].is_active){
        if(layer > 4){
            return;
        }

        sprites[id].layer = layer;
    }
}

uint16_t get_sprite_pixel(uint16_t x, uint8_t y, uint8_t id)
{
   return (uint16_t) *(sprites[id].pixels + (y * sprites[id].width) + x);
}

void delete_sprite(uint8_t id)
{
    if(sprites[id].is_active){
       sprites[id].width = 0;
       sprites[id].height = 0;
       sprites[id].x = 0;
       sprites[id].y = 0;
       sprites[id].is_active = 0;
       free(sprites[id].pixels);
    }
}
