#include "text.h"
#include "font.h"
#include <math.h>

struct ui_string strings[10];

uint8_t create_text(uint8_t x, uint8_t y, char *text, uint8_t len)
{
    int index = -1;
    for(int i = 0; i < 10; i++)
    {
        if(strings[i].len == 0){
            index = i;
            break;
        }
    }

    if(index == -1)
        return index;      // No free space in the ui_string array
    
    strings[index].x = x;
    strings[index].y = y;
    strings[index].len = len;

    strings[index].char_codes = malloc(len * sizeof(uint8_t));
    
    if(!strings[index].char_codes){
        free(strings[index].char_codes);
        return -1;  // Failed to allocate memory
    }

    // Convert text input to char codes
    // TODO: Add a check for japanese characters
    for(int i = 0; i < len; i++){
        strings[index].char_codes[i] = (int)text[i] - 32;
    }
    return index;
}

uint8_t set_text_color(uint16_t color, uint8_t id)
{
    if(strings[id].len == 0)
        return 0;

    strings[id].color = color;
    return 1;
}

uint16_t get_text_pixel(uint8_t x, uint8_t y, uint8_t id)
{
    // 8 is the width of the char in pixels
    uint8_t char_index = floor(x / 8); // Get the current char 

    uint8_t pixel_data = font[strings[id].char_codes[char_index] * 16 + y];

    uint8_t rel_x = x - char_index * 8;

    if(pixel_data & (128 >> (rel_x))){
        return 0xffff;
    }
    return 0x0000;
}
