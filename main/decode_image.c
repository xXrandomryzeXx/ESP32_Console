#include "decode_image.h"
#include "jpeg_decoder.h"
#include "esp_log.h"
#include "esp_check.h"
#include <string.h>
#include "freertos/FreeRTOS.h"

// Reference binary included jpeg file called background.jpg
extern const uint8_t image_jpg_start[] asm("_binary_background_jpg_start");
extern const uint8_t image_jpg_end[] asm("_binary_background_jpg_end");

const char *TAG = "ImageDec";

esp_err_t decode_image(uint16_t **pixels, uint8_t size)
{
    *pixels = NULL;
    esp_err_t ret = ESP_OK;

    // Allocate pixel memory. Each line is an array of IMAGE_W 16-bit pixels; the '*pixels' array itself contains pointers to these lines
    //*pixels = calloc(IMAGE_H * IMAGE_W, sizeof(uint16_t));
    if(size == 0){
        *pixels = heap_caps_malloc(IMAGE_H * IMAGE_W * sizeof(uint16_t), MALLOC_CAP_DEFAULT);
    } else if(size == 1){
        *pixels = calloc(IMAGE_64 * IMAGE_64, sizeof(uint16_t));
    } else if(size == 2){
        *pixels = calloc(IMAGE_32 * IMAGE_32, sizeof(uint16_t));
    } else if(size == 3){
        *pixels = calloc(IMAGE_16 * IMAGE_16, sizeof(uint16_t));
    } else/* if(size == 4)*/{
        *pixels = calloc(IMAGE_8 * IMAGE_8, sizeof(uint16_t));
    }

    ESP_GOTO_ON_FALSE((*pixels), ESP_ERR_NO_MEM, err, TAG, "Error when trying to allocate memory for image");

    // JPEG decode config
    esp_jpeg_image_cfg_t jpeg_cfg = {
        .indata = (uint8_t *)image_jpg_start,
        .indata_size = image_jpg_end - image_jpg_start,
        .outbuf = (uint8_t*)(*pixels),
        //.outbuf_size = IMAGE_W * IMAGE_H * sizeof(uint16_t),
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
    } else/* if(size == 4)*/{
        jpeg_cfg.outbuf_size = IMAGE_8 * IMAGE_8 * sizeof(uint16_t);
    }

    // JPEG decode
    esp_jpeg_image_output_t outimg;
    esp_jpeg_decode(&jpeg_cfg, &outimg);

    ESP_LOGI(TAG, "JPEG image decoded; Size %dpx x %dpx", outimg.width, outimg.height);
    return ret;
err:
    // If something went wrong, exit cleaning, de-allocate
    if(*pixels != NULL){
        free(*pixels);
    }
    return ret;
}


esp_err_t decode_image_from_sd(uint16_t **pixels, uint8_t *input_data, uint32_t image_size,
        uint8_t size)
{
    esp_err_t ret = ESP_OK;

    /* JPEG decode config */
    esp_jpeg_image_cfg_t jpeg_cfg = {
        .indata = input_data,
        .indata_size = image_size,
        .outbuf = (uint8_t*)(*pixels),
        /* .outbuf_size = IMAGE_W * IMAGE_H * sizeof(uint16_t), */
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
    } else/* if(size == 4)*/{
        jpeg_cfg.outbuf_size = IMAGE_8 * IMAGE_8 * sizeof(uint16_t);
    }

    /* JPEG decode */
    esp_jpeg_image_output_t outimg;
    esp_jpeg_decode(&jpeg_cfg, &outimg);

    /* Free the input data */
    free(input_data);

    return ret;
}

