#include "text.h"
#include "font.h"
#include <math.h>

struct ui_string strings[10];
struct ui_string japanese_strings[10];

uint8_t create_text(uint16_t x, uint8_t y, char *text, uint8_t len)
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

uint8_t create_japanese_text(uint16_t x, uint8_t y, uint8_t *codes, uint8_t len)
{
    int index = -1;
    for(int i = 0; i < 10; i++)
    {
        if(japanese_strings[i].len == 0){
            index = i;
            break;
        }
    }

    if(index == -1)
        return index;      // No free space in the ui_string array
    
    japanese_strings[index].x = x;
    japanese_strings[index].y = y;
    japanese_strings[index].len = len;

    japanese_strings[index].char_codes = malloc(len * sizeof(uint8_t));
    
    if(!japanese_strings[index].char_codes){
        free(japanese_strings[index].char_codes);
        return -1;  // Failed to allocate memory
    }

    printf("こんいちは consists of these char codes: \n");
    printf("こ -> %d\n", (int)'こ');
    printf("ん -> %d\n", (int)'ん');
    printf("に -> %d\n", (int)'に');
    printf("ち -> %d\n", (int)'ち');
    printf("は -> %d\n", (int)'は');

    // Convert text input to char codes
    // TODO: Add a check for japanese characters
    for(int i = 0; i < len; i++){
        japanese_strings[index].char_codes[i] = codes[i];
        printf("%d\n", japanese_strings[index].char_codes[i]);
    }
    return index;
}

uint8_t delete_text(uint8_t id)
{
    if(strings[id].len > 0){
        strings[id].x = 0;
        strings[id].y = 0;
        strings[id].len = 0;
        free(strings[id].char_codes);
        return 1;
    }
    return 0;
}

uint8_t set_text_color(uint16_t color, uint8_t id)
{
    if(strings[id].len == 0)
        return 0;

    strings[id].color = color;
    return 1;
}

uint8_t set_japanese_text_color(uint16_t color, uint8_t id)
{
    if(japanese_strings[id].len == 0)
        return 0;
    
    strings[id].color = color;
    return 1;
}

uint16_t get_text_pixel(uint16_t x, uint8_t y, uint8_t id)
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

uint16_t get_japanese_text_pixel(uint16_t x, uint8_t y, uint8_t id)
{
    uint8_t char_index = floor(x / 16);
    uint8_t rel_x = x - char_index * 16;

    uint8_t low_byte = (rel_x >= 8);

    uint16_t pixel_data_index;
    uint8_t pixel_data;
    uint8_t char_code = japanese_strings[id].char_codes[char_index];

    if(char_code < 100){
        pixel_data_index = ((char_code-1) * 32) + (y*2) + low_byte;
        pixel_data = hiragana[pixel_data_index];
    }else{
        pixel_data_index = ((char_code-101) * 32) + (y*2) + low_byte;
        pixel_data = katakana[pixel_data_index];
    }

    if(pixel_data & (128 >> (rel_x - (8 * low_byte)))){
        return 0xffff;
    }
    return 0x0000;
    
}
