#include <stdlib.h>
#include <stdio.h>
#include "esp_system.h"
#include "driver/spi_master.h"

#include "spi.h"
#include "lcd.h"
#include "display.h"
#include "button.h"
#include "memory.h"
#include "text.h"

spi_device_handle_t lcd_spi;
spi_device_handle_t sd_spi;

void app_main(void)
{
    esp_err_t ret;

    /*spi_device_handle_t lcd_spi;
    spi_device_handle_t sd_spi;

    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = PARALLEL_LINES * 320 * 2 + 8
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10 * 1000 * 1000,     // Clock out at 10MHz
        .mode = 0,                              // SPI mode 0
        .spics_io_num = PIN_NUM_CS,             // CS pin
        .queue_size = 7,                        // We want to be able to queue 7 transactions at a time
        .pre_cb = lcd_spi_pre_transfer_callback,// Specify pre-transfer callback to handle D/C line
    };

    // Initialize the SPI bus
    ret = spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    // Attach the LCD to the spi bus
    ret = spi_bus_add_device(LCD_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);*/
    
    int starting_memory = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    int last_memory = starting_memory;

    printf("Free heap memory at start: %d\n", starting_memory);
    printf("Largest free memory block: %d\n", heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));
  
    // Initialize stuff, images, text, etc.
    ret = init_background_image();
    ESP_ERROR_CHECK(ret);

    printf("Used memory for graphics: %d\n", last_memory - heap_caps_get_free_size(MALLOC_CAP_8BIT));

  
    // Initialize the SPI and it's slaves
    ret = spi_init();
    ESP_ERROR_CHECK(ret);

    printf("Used memory for spi_init: %d\n", last_memory - heap_caps_get_free_size(MALLOC_CAP_8BIT));
    
    // Initialize the LCD
    lcd_init(lcd_spi);
    
  
    // Initialize the Input buttons
    buttons_init();
    last_memory = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    
    // Initialize the SD card reader
    sd_init();

    printf("Loading background image from SD card\n");
    ret = init_background_image_from_sd();
    ESP_ERROR_CHECK(ret);

    printf("Used memory for sd_init: %d\n", last_memory - heap_caps_get_free_size(MALLOC_CAP_8BIT));
    printf("Total free memory: %d\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    last_memory = heap_caps_get_free_size(MALLOC_CAP_8BIT);

    sd_unmount();

    printf("Used heap memory: %d\n", starting_memory - heap_caps_get_free_size(MALLOC_CAP_8BIT));

    printf("Free memory: %d\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));

    uint8_t hello_text = create_text(50, 50, "Hello World", 11);
    uint8_t arr[9] = {20, 84, 44, 34, 48, 95, 28, 12, 5};
    //uint8_t arr[5] = {0, 0, 0, 0, 0};
    //uint8_t arr[1] = {82, 83};
    uint8_t hello_japanese_text = create_japanese_text(50, 100, arr,  9);
    
    display_screen(lcd_spi);
}
