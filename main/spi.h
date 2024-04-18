#pragma once
#include "esp_system.h"

// SPI LINES
#define SPI_HOST_INSTANCE   SPI3_HOST
#define PIN_NUM_MISO        19
#define PIN_NUM_MOSI        23
#define PIN_NUM_CLK         18

// LCD LINES
#define LCD_PIN_NUM_DC      4
#define LCD_PIN_NUM_RST     0
#define LCD_PIN_NUM_BCKL    22
#define LCD_PIN_NUM_CS      5

// SD LINES
#define SD_PIN_NUM_CS       15

/*
 * @brief Initialize all SPI connections
 *
 * @return      - ESP_OK Everything was successful
 *              - ESP_FAIL or else Initialization of some device failed
 */
esp_err_t spi_init();
