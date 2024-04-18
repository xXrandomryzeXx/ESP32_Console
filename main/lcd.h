#include <stdlib.h>
#include "esp_system.h"
#include "driver/spi_master.h"

#define LCD_BK_LIGHT_ON_LEVEL   1

#define PARALLEL_LINES 2 

typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes;
} lcd_init_cmd_t;

/* 
 * @brief Callback for before each SPI transaction, to set C/D pin
 *
 * @param t     - spi_transaction, gets the t->user variable to determine if it's a comman or data
 *
 */
void lcd_spi_pre_transfer_callback(spi_transaction_t *t);

/*
 * @brief Initializes the LCD, sets pin modes, runs initializing commands on the display
 *
 * @param spi   - An spi device handle to the display
 *
 */
void lcd_init(spi_device_handle_t spi);

/*
 * @brief   Prepared variables for displaying pixels to the display
 *
 */
void display_lcd_prepare();

/*
 * @brief   Draw pixels to the screen by calling functions from display.h/c. An array of line data
 *          is created and sent to these functions which then are set with the pixel data, which
 *          is then sent through an SPI transaction to the display
 *          
 * @param spi   - An spi device handle to the display
 *
 */
void display_lcd_screen(spi_device_handle_t spi);
