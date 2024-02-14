#include "display.h"
#include "decode_image.h"
#include "sdkconfig.h"

#include "memory.h"
#include "text.h"

uint16_t *pixels;

extern struct ui_string strings[10];

static inline uint16_t get_background_pixel(int x, int y)
{
    return (uint16_t) * (pixels + (y * IMAGE_W) + x);
}

void get_screen_lines(uint16_t *dest, int line, int n)
{
    for(int y = line; y < line + n; y++){
        // Draw background
        for(int x = 0; x < 320; x++) {
            *dest++ = get_background_pixel(x, y);
            // Draw text
            for(int i = 0; i < 10; i++){
                if(strings[i].len > 0){
                    if(y >= strings[i].y && y < strings[i].y + 16 &&
                       x >= strings[i].x && x < strings[i].x + strings[i].len * 8) {
                        uint16_t ret = get_text_pixel(x - strings[i].x, y - strings[i].y, i);
                        if(ret){
                            *(dest-1) = (strings[i].color)? strings[i].color : ret;
                        }
                    }
                }
            }
        }

    }
}

esp_err_t init_background_image(void)
{
    return decode_image(&pixels, 0);
    /*pixels = calloc(320 * 240 * sizeof(uint16_t), 0);
    printf("memory of pixels: %p\n", pixels);
    return ESP_OK;*/
}

esp_err_t init_background_image_from_sd(void)
{
    return s_load_image("/sdcard/bckg2.jpg", &pixels);
}
