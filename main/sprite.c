#include "sprite.h"
#include "decode_image.h"

sprite sprite_array[SPRITES];

const char *TAG = "SpriteMan";

uint8_t get_free_index()
{
    for(uint8_t i = 0; i < SPRITES; i++){
        if(!sprite_array[i].is_active)
            return i;
    }
    return -1;
}

esp_err_t create_sprite(uint8_t size, char* file_name, uint8_t id)
{
    uint8_t index = get_free_index();
    esp_err_t ret = ESP_OK;
   
    // If there is no free space in the sprite array, return an error 
    ESP_GOTO_ON_FALSE((index>=0), ESP_FAIL, err, TAG, "Error when trying to create sprite: No free space in the array");

    // Decoding the image from the files
    ret = decode_image(sprite_array[index].pixels, size);
    ESP_ERROR_CHECK(ret);

    

    ESP_LOGI(TAG, "Sprite has been created");
    return ret;
err:
   return ret; 
}
