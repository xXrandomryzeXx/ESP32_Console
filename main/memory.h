#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/spi_master.h"

#define MAX_CHAR_SIZE   64

#define MOUNT_POINT "/sdcard"

static const char *kanji_path = MOUNT_POINT"/kanji.txt";
static const char *time_path = MOUNT_POINT"/time.txt";

esp_err_t sd_spi_init(spi_host_device_t spi_host, const uint8_t cs_pin);
esp_err_t s_load_image(char *path, uint16_t **pixels, uint8_t size);
esp_err_t s_read_line(const char *path, char **data, uint16_t line);
esp_err_t s_write_line(const char *path, char **new_data);
void sd_init();
void sd_unmount();
