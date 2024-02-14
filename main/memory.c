#include "memory.h"

#include "decode_image.h"

#include <dirent.h>

static const char *TAG = "sd_card";

sdmmc_card_t *card;
const char mount_point[] = MOUNT_POINT;

static esp_err_t s_write_file(const char *path, char *data)
{
    ESP_LOGI(TAG, "Opening file %s", path);
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);
    ESP_LOGI(TAG, "File written");

    return ESP_OK;
}

static esp_err_t s_read_file(const char *path)
{
    ESP_LOGI(TAG, "Reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }
    char line[MAX_CHAR_SIZE];
    fgets(line, sizeof(line), f);
    fclose(f);

    // strip newline
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file '%s'", line);

    return ESP_OK;
}

esp_err_t s_load_image(const char *path, uint16_t **pixels)
{
    ESP_LOGI(TAG, "Opening image %s", path);
    FILE *img = fopen(path, "rb");
    if(img == NULL){
        perror("fopen");
        ESP_LOGE(TAG, "Failed to open image");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Image opened");

    // Getting size of file
    fseek(img, 0, SEEK_END);
    uint32_t len = ftell(img);
    fseek(img, 0, SEEK_SET);

    // Create pointer to store the data from the file
    uint8_t *read_pixels = malloc(len * sizeof(uint8_t));
    fread(read_pixels, len, 1, img);
    fclose(img);

    ESP_LOGI(TAG, "Decoding image");
    return decode_image_from_sd(pixels, read_pixels, len, 0);
}

// Should be called before sd_init
esp_err_t sd_spi_init(spi_host_device_t spi_host, const uint8_t cs_pin)
{ 
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing SD card");

    ESP_LOGI(TAG, "Using SPI peripheral");

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = cs_pin;
    slot_config.host_id = spi_host;

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);
    if(ret != ESP_OK) {
        if(ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                    "Make sure the SD card lines have pull-up resistors in place.", esp_err_to_name(ret)); 
        }
        return ret;
    }
    ESP_LOGI(TAG, "Filesystem mounted");
    return ret;
}

void sd_init()
{
    esp_err_t ret;
     
    /*sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if(ret != ESP_OK){
        ESP_LOGE(TAG, "Failed to initialize bus");
        return;
    }*/

    sdmmc_card_print_info(stdout, card);

    /*const char *file_hello = MOUNT_POINT "/hello.txt";
    char data[MAX_CHAR_SIZE];
    snprintf(data, MAX_CHAR_SIZE, "%s %s!\n", "Hello", card->cid.name);
    ret = s_write_file(file_hello, data);
    if(ret != ESP_OK) {
        return;
    }

    const char *file_foo = MOUNT_POINT "/foo.txt";

    struct stat st;
    if(stat(file_foo, &st) == 0) {
        // Delete foo.txt if it exists
        unlink(file_foo);
    }

    // Rename the hello.txt file
    ESP_LOGI(TAG, "Renaming file %s to %s", file_hello, file_foo);
    if(rename(file_hello, file_foo) != 0) {
        ESP_LOGE(TAG, "Rename failed");
        return;
    }

    ret = s_read_file(file_foo);
    if(ret != ESP_OK){
        return;
    }*/

    // Format FATFS
    /*ret = esp_vfs_fat_sdcard_format(mount_point, card);
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to format FATFS(%s)", esp_err_to_name(ret));
        return;
    }

    if(stat(file_foo, &st) == 0) {
        ESP_LOGI(TAG, "file still exists");
        return;
    } else {
        ESP_LOGI(TAG, "file doesn't exist, format done");
    }*/

    /*const char *file_nihao = MOUNT_POINT"/nihao.txt";
    memset(data, 0, MAX_CHAR_SIZE);
    snprintf(data, MAX_CHAR_SIZE, "%s %s!\n", "Nihao", card->cid.name);
    ret = s_write_file(file_nihao, data);
    if(ret != ESP_OK) {
        return;
    }

    // Open file for reading
    ret = s_read_file(file_nihao);
    if(ret != ESP_OK) {
        return;
    }*/

    // Done, unmount partition and disable SPI peripheral
    //esp_vfs_fat_sdcard_unmount(mount_point, card);
    //ESP_LOGI(TAG, "Card unmounted");

    // De-init the bus
    //spi_bus_free(host.slot);
}


void sd_unmount()
{
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(TAG, "Card unmounted");
}
