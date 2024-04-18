#include "decode_image.h"
#include "jpeg_decoder.h"
#include "esp_log.h"
#include "esp_check.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_heap_caps.h"

const char *TAG = "ImageDec";

esp_err_t decode_image_from_sd(uint16_t **pixels, uint8_t *input_data, uint32_t image_size, uint8_t size)
{
    esp_err_t ret = ESP_OK;

    /* JPEG decode config */
    esp_jpeg_image_cfg_t jpeg_cfg = {
        .indata = input_data,
        .indata_size = image_size,
        .outbuf = (uint8_t*)(*pixels),
        .out_format = JPEG_IMAGE_FORMAT_RGB565,
        .flags = {
            .swap_color_bytes = 1,
        }
    };

    if(size == 0){
        jpeg_cfg.outbuf_size = IMAGE_W * IMAGE_H * sizeof(uint16_t);
    } else if(size == 1){
        jpeg_cfg.outbuf_size = IMAGE_64 * IMAGE_64 * sizeof(uint16_t);
    } else if(size == 2){
        jpeg_cfg.outbuf_size = IMAGE_32 * IMAGE_32 * sizeof(uint16_t);
    } else if(size == 3){
        jpeg_cfg.outbuf_size = IMAGE_16 * IMAGE_16 * sizeof(uint16_t);
    } else if(size == 5){
        jpeg_cfg.outbuf_size = IMAGE_128 * IMAGE_128 * sizeof(uint16_t);
    }else/* if(size == 4)*/{
        jpeg_cfg.outbuf_size = IMAGE_8 * IMAGE_8 * sizeof(uint16_t);
    }

    /* JPEG decode */
    esp_jpeg_image_output_t outimg;
    esp_jpeg_decode(&jpeg_cfg, &outimg);

    /* Avoid a memory leak */
    free(input_data);

    return ret;
}

