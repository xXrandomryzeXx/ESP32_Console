#include <stdlib.h>
#include <stdio.h>
#include "esp_system.h"
#include "driver/spi_master.h"

#include "game.h"

#include "spi.h"
#include "button.h"
#include "lcd.h"

#include "display.h"
#include "memory.h"
#include "text.h"

spi_device_handle_t lcd_spi;
spi_device_handle_t sd_spi;

void Start(){
    // Setup
    game = 1;

    esp_err_t ret;

    // Load background image from ROM, for memory allocation mainly
    ret = init_background_image();
    ESP_ERROR_CHECK(ret);

    // Initialize the spi bus
    ret = spi_init();
    ESP_ERROR_CHECK(ret);

    // Initialize LCD, setup SPI slave, setup commands
    lcd_init(lcd_spi);

    // Initialize LCD, setp the SPI slave
    sd_init();

    // Setup the button's GPIO
    buttons_init();

    ret = init_background_image_from_sd();
    ESP_ERROR_CHECK(ret);
   
    // Unmount SD for safe turn off
    sd_unmount(); 

    // Create a test text in english
    uint8_t hello_text = create_text(50, 50, "Hello World", 11);


    // Create a test text in japanese (hiragana)
    uint8_t arr[9] = {20, 84, 44, 34, 48, 95, 28, 12, 5};
    uint8_t japanese_text = create_japanese_text(50, 100, arr, 9);
    
    // Prepare the LCD for graphics input 
    display_lcd_prepare();

    // Basic game loop
    while(game){
        Update();
        Render();
    }
    
}
