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

#include "state_machine.h"

spi_device_handle_t lcd_spi;
spi_device_handle_t sd_spi;

static uint8_t menu_selection;
int8_t input;
/*
 * 0 -> game loop won't run
 * 1 -> game loop will run
 */
static uint8_t game;


void sendInput()
{
    input = 1;
}

void resetInput()
{
    input = 0;
}

void ignoreInput()
{
    input = -1;
}

void selectMenu(uint8_t menu)
{
    if(menu >= 4)
        return;
    menu_selection = menu;
}

uint8_t getSelectedMenu()
{
    return menu_selection;
}

void Start(){
    // Just in case, set state to Null
    setState(Null);

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

    //ret = init_background_image_from_sd();
    //ESP_ERROR_CHECK(ret);
   
    // Unmount SD for safe turn off
    //sd_unmount(); 

    // Create a test text in english
    //uint8_t hello_text = create_text(50, 50, "Hello World", 11);


    // Create a test text in japanese (hiragana)
    //uint8_t arr[9] = {20, 184, 44, 134, 48, 95, 128, 12, 105};
    //uint8_t japanese_text = create_japanese_text(50, 100, arr, 9);
    
    // Prepare the LCD for graphics input 
    display_lcd_prepare();

    // After the initial setup, state can be set to the main menu
    setState(MainMenu);
    sendInput();
    // Basic game loop
    while(game){
        if(input){
            Update();
        }
        Render();
        Input();
    }
    
}
