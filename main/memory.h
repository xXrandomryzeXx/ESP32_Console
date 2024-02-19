#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/spi_master.h"

#define MAX_CHAR_SIZE   64

#define MOUNT_POINT "/sdcard"

esp_err_t sd_spi_init(spi_host_device_t spi_host, const uint8_t cs_pin);
esp_err_t s_load_image(char *path, uint16_t **pixels);
void sd_init();
void sd_unmount();
