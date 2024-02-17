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

typedef enum {
    LCD_TYPE_ILI = 1,
} type_lcd_t;

void lcd_spi_pre_transfer_callback(spi_transaction_t *t);
void lcd_init(spi_device_handle_t spi);

//void display_screen(spi_device_handle_t spi);

void display_lcd_prepare();
void display_lcd_screen(spi_device_handle_t spi);
