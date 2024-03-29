#include "text.h"
#include "font.h"
#include <math.h>

struct ui_string strings[10];
struct ui_string japanese_strings[10];

char* kat[85] = {
   "ー","ァ","ア","ィ","イ","ゥ","ウ","ェ","エ","ォ","オ","カ","ガ","キ","ギ","ク",
   "グ","ケ","ゲ","コ","ゴ","サ","ザ","シ","ジ","ス","ズ","セ","ゼ","ソ","ゾ","タ",
   "ダ","チ","ヂ","ッ","ツ","ヅ","テ","デ","ト","ド","ナ","ニ","ヌ","ネ","ノ","ハ",
   "バ","パ","ヒ","ビ","ピ","フ","ブ","プ","ヘ","ベ","ペ","ホ","ボ","ポ","マ","ミ",
   "ム","メ","モ","ャ","ヤ","ュ","ユ","ョ","ヨ","ラ","リ","ル","レ","ロ","ヮ","ワ",
   "ヰ","ヱ","ヲ","ン"," "
};

char* hir[85] = {
"ー","ぁ","あ","ぃ","い","ぅ","う","ぇ","え","ぉ","お","か","が","き","ぎ","く",
"ぐ","け","げ","こ","ご","さ","ざ","し","じ","す","ず","せ","ぜ","そ","ぞ","た",
"だ","ち","ぢ","っ","つ","づ","て","で","と","ど","な","に","ぬ","ね","の","は",
"ば","ぱ","ひ","び","ぴ","ふ","ぶ","ぷ","へ","べ","ぺ","ほ","ぼ","ぽ","ま","み",
"む","め","も","ゃ","や","ゅ","ゆ","ょ","よ","ら","り","る","れ","ろ","ゎ","わ",
"ゐ","ゑ","を","ん"," "
};

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

    for(int i = 0; i < len; i++){
        strings[index].char_codes[i] = (int)text[i] - 32;
    }
    return index;
}

uint8_t create_japanese_text(uint16_t x, uint8_t y, char *text, uint8_t len)
{
    /* find free index for the text */
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

    japanese_strings[index].char_codes = calloc(len, len * sizeof(uint8_t));
    
    if(!japanese_strings[index].char_codes){
        free(japanese_strings[index].char_codes);
        return -1;  // Failed to allocate memory
    }
    
    /* Convert text to "separate" chars.
     * Each japanese char from Katakana and Hiragana consist of 3 chars */

    char *ch = malloc(3 * sizeof(char));
    uint8_t offset = 0;
    for(int i = 0; i < len; ++i){
        /* If we get a 1 byte char, the set an offset, and decrease i,
         * this way we only go 1 byte forward */
        switch(text[i*3 + offset]){
            case '(':
               japanese_strings[index].char_codes[i + offset] = 84;
               ++offset;
               --i;
               continue;
            case ')':
               japanese_strings[index].char_codes[i + offset] = 85;
               ++offset;
               --i;
               continue;
        }
        /* Loop through 3 characters to get 1 japanese char */
        for(int j = 0; j < 3; ++j){
            ch[j] = text[(i*3 + offset) + j];
        }
        /* After getting the char, decide which alphabet
         * it is from */
        uint8_t code = 94; // unset char, used for space
        for(int k = 0; k < 85; ++k){
           uint8_t kat_match = 1;
           uint8_t hir_match = 1;
           /* Another for loop to compare the 3 chars of our current char
            * with the 3 chars of the array of chars we are looping through */
           for(int l = 0; l < 3; ++l){
               if(ch[l] != kat[k][l]){
                  kat_match = 0;
               }
               if(ch[l] != hir[k][l]){
                  hir_match = 0;
               }
           }
           
           if(kat_match){
               code = k + 100; /* We add +100 to differentiate Katakana from Hiragana */
           }else if(hir_match){
               code = k; /* Nothing to add because this is Hiragana */
           }
            
        }
        japanese_strings[index].char_codes[i + offset] = code;
    }

    return index + 10;
}

uint8_t delete_text(uint8_t id)
{
    if(id < 10){
        if(strings[id].len > 0){
             strings[id].x = 0;
             strings[id].y = 0;
             strings[id].len = 0;
             free(strings[id].char_codes);
             return 1;
        }
    }else{
        id = id - 10;
        if(japanese_strings[id].len > 0){
             japanese_strings[id].x = 0;
             japanese_strings[id].x = 0;
             japanese_strings[id].len = 0;
             free(japanese_strings[id].char_codes);
        }
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
        pixel_data_index = ((char_code) * 32) + (y*2) + low_byte;
        pixel_data = hiragana[pixel_data_index];
    }else{
        pixel_data_index = ((char_code-100) * 32) + (y*2) + low_byte;
        pixel_data = katakana[pixel_data_index];
    }

    if(pixel_data & (128 >> (rel_x - (8 * low_byte)))){
        return 0xffff;
    }
    return 0x0000;
    
}
