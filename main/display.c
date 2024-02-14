#include "display.h"
#include "decode_image.h"
#include "sdkconfig.h"

#include "memory.h"

uint16_t *pixels;

static inline uint16_t get_background_pixel(int x, int y)
{
    return (uint16_t) * (pixels + (y * IMAGE_W) + x);
}

void get_screen_lines(uint16_t *dest, int line, int n)
{
    for(int y = line; y < line + n; y++){
        for(int x = 0; x < 320; x++) {
            *dest++ = get_background_pixel(x, y);
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
