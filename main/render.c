#include "game.h"
#include "lcd.h"

extern spi_device_handle_t lcd_spi;

void Render()
{
    display_lcd_screen(lcd_spi);
}
