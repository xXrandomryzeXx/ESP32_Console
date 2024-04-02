#pragma once
#include <stdint.h>
#include "esp_err.h"

#define IMAGE_W 320
#define IMAGE_H 240

#define IMAGE_128 128
#define IMAGE_64 64
#define IMAGE_32 32
#define IMAGE_16 16
#define IMAGE_8  8


/**
 * @biref Decode a jpeg image embeded into the program file and turn it into pixel data
 *
 * @param pixels - A pointer for an array of rows of arrays of pixels
 *        size   - Select from the 5 available sizes, 0 > 4 from largest to smallest
 *
 * @return  - ESP_ERR_NOT_SUPPORTED if image is malformed or a progressive jpeg file
 *          - ESP_ERR_NO_MEM if there is no memory (ran out of RAM)
 *          - ESP_OK successful decode
 */
esp_err_t decode_image(uint16_t **pixels, uint8_t size);



/**
 * @brief Decodes a jpeg image from the loaded SD card
 *
 * @param pixels - A pointer for an array of rows of arrays of pixels
 *        image_start - The pointer, pointing to the start of the image in memeory
 *        image_size - The size of the image that will be loaded
 *        size   - Select from the 5 available sizes, 0 > 4 from largest to smallest
 *
 * @return  - ESP_ERR_NOT_SUPPORTED if image is malformed or a progressive jpeg file
 *          - ESP_ERR_NO_MEM if there is no memory (ran out of RAM)
 *          - ESP_OK successful decode
 */
esp_err_t decode_image_from_sd(uint16_t **pixels, uint8_t *image_start, uint32_t image_size,
        uint8_t size);
