/*#include <stdlib.h>
#include <stdio.h>
#include "esp_system.h"
#include "driver/spi_master.h"

#include "spi.h"
#include "lcd.h"
#include "display.h"
#include "button.h"
#include "memory.h"
#include "text.h"*/

#include "game.h"

//spi_device_handle_t lcd_spi;
//spi_device_handle_t sd_spi;

void app_main(void)
{
    Start();
    // Setup on startup
    /*esp_err_t ret;

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
    // Hello World in japanese "konnichiha sekai"
    uint8_t arr[9] = {20, 84, 44, 34, 48, 95, 28, 12, 5};
    uint8_t hello_japanese_text = create_japanese_text(50, 100, arr,  9);
    
    //display_screen(lcd_spi);*/
}
