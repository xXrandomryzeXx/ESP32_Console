#pragma once
#include <stdint.h>
#include "esp_err.h"

void get_screen_lines(uint16_t *dest, int line, int n);

esp_err_t init_background_image(void);
esp_err_t init_background_image_from_sd(char* path);
