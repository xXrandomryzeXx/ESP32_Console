#ifndef MEMORY_H
#define MEMORY_H

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/spi_master.h"

#define MAX_CHAR_SIZE   64

#define MOUNT_POINT "/sdcard"

static const char *kanji_path = MOUNT_POINT"/kanji.txt";
static const char *time_path = MOUNT_POINT"/time.txt";
static const char *score_path = MOUNT_POINT"/score.txt";

/*
 * @brief Initialize the spi connection to the microSD reader
 *
 * @param spi_host  - The host instance
 *        cs_pin    - ChipSelect pin, used to differentiate different SPI devices from one another
 *
 * @return          - ESP_OK    Everything was successful
 *                  - ESP_FAIL  Something went wrong
 */
esp_err_t sd_spi_init(spi_host_device_t spi_host, const uint8_t cs_pin);

/*
 * @brief Load an image
 *
 * @param path      - A path to the image "/sdcard/finename.extension"
 *        pixels    - A pointer to the pixel array/pointer which will store this image data
 *        size      - A fixed size for the image 1-5 (128x128, 64x64, 32x32, 16x16,  8x8)
 *
 * @return          - ESP_OK    Everything was successful
 *                  - ESP_FAIL  Failed to open the image
 */
esp_err_t s_load_image(char *path, uint16_t **pixels, uint8_t size);

/*
 * @brief Read one line from the file
 *
 * @param path      - A path to the image "/sdcard/finename.extension"
 *        data      - A pointer to the data array/pointer which will store this data
 *        line      - The line from which to read the data
 *
 * @return          - ESP_OK    Everything was successful
 *                  - ESP_FAIL  Failed to open the file or couldn't read the line
 */

esp_err_t s_read_line(const char *path, char **data, uint16_t line);

/*
 * @brief Clear the file and wrtie one line to it
 *
 * @param path      - A path to the image "/sdcard/filename.extension"
 *        new_data  - A pointer to the new data that will be written to the file
 *
 * @return          - ESP_OK    Everything was successful
 *                  - ESP_FAIL  Failed to open file for writing
 */
esp_err_t s_write_line(const char *path, char **new_data);

/* 
 * @brief Prints out info about the SD card
 */

void sd_init();

/*
 * @brief Unmount the SD card 
 */
void sd_unmount();

#endif
