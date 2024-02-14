#include "text.h"
#include "font.h"
#include <math.h>

struct ui_string strings[10];

uint8_t create_text(uint8_t x, uint8_t y, char *text, int len)
{
    uint8_t index = -1;
    for(int i = 0; i < 10; i++)
    {
        if(strings[i].len == 0){
            index = i;
            break;
        }
    }

    if(index == -1)
        break;      // No free space in the ui_string array
    
    strings[index].x = x;
    strings[index].y = y;
    strings[index].len = text_len;

    strings[index].char_codes = maloc(len * sizeof(uint8_t));
    
    if(!strings[index].char_codes){
        free(strings[index].char_codes);
        return -1;  // Failed to allocate memory
    }

    // Convert text input to char codes
    // TODO: Add a check for japanese characters
    for(int i = 0; i < len; i++){
        strings[index].char_codes[i] = (int)text[i] - 32;
    }
}
