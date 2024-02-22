#include "memory.h"

#include "decode_image.h"

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

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

esp_err_t s_read_line(const char *path, char **data, uint16_t line)
{
    FILE *f = fopen(path, "r");
    if(f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }

    uint16_t current_line = 0;
    char *line_data = malloc(64 * sizeof(char));
    void* result = NULL;
    while(current_line <= line){
        result = fgets(line_data, 64, f);
        current_line++;
    }

    fclose(f);
    
    if(result == NULL){
        ESP_LOGE(TAG, "Couldn't read line %d\n", line);
        free(line_data);
        return ESP_FAIL;
    } 

    *data = line_data;

    return ESP_OK; 
}
esp_err_t s_write_line(const char *path, char **new_data, uint16_t line)
{
    FILE *f = fopen(path, "w+");
    if(f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }
    
    uint16_t current_line = 0;
    char *line_data = malloc(64 * sizeof(char));
    void *result = NULL;
    while(current_line <= line){
        result = fgets(line_data, 64, f);
        current_line++;
    }

    // Reposition cursor
    long int position = ftell(f);
    fseek(f, position, SEEK_SET);

    // Write the new data
    fprintf(f, "%s\n", *new_data);
    fclose(f);
    
    return ESP_OK;
}

esp_err_t s_load_image(char *path, uint16_t **pixels)
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

    sdmmc_card_print_info(stdout, card);
}


void sd_unmount()
{
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(TAG, "Card unmounted");
}
