#include "spi.h"
#include "driver/spi_master.h"

#include "memory.h"
#include "lcd.h"

static const char *TAG = "spi";

extern spi_device_handle_t lcd_spi;
extern spi_device_handle_t sd_spi;

esp_err_t spi_init()
{
    esp_err_t ret;

    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4094
    };

    spi_device_interface_config_t lcd_devconfig = {
        .clock_speed_hz = 10 * 1000 * 1000,         // Clock out at 10MHz
        .mode = 0,                                  // SPI mode 0
        .spics_io_num = LCD_PIN_NUM_CS,
        .queue_size = 7,
        .pre_cb = lcd_spi_pre_transfer_callback
    }; 


       
    ret = spi_bus_initialize(SPI_HOST_INSTANCE, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    
    ret = sd_spi_init(SPI_HOST_INSTANCE, SD_PIN_NUM_CS);
    
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "Failed to initialize SD SPI device");
        return ret;
    }

    ret = spi_bus_add_device(SPI_HOST_INSTANCE, &lcd_devconfig, &lcd_spi);
    ESP_ERROR_CHECK(ret);

    return ret;
}
