#include "button.h"
#include "driver/gpio.h"

button buttons[4];

uint8_t get_button_state(uint8_t button_id)
{
    return gpio_get_level(buttons[button_id].pin);
}

void buttons_init()
{
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = ((1ULL << BUTTON_1) | (1ULL << BUTTON_2) | (1ULL << BUTTON_3) | (1ULL << BUTTON_4));
    io_conf.mode = GPIO_MODE_INPUT;
    gpio_config(&io_conf);

    for(uint8_t i = 0; i < 4; i++){
        buttons[i].id = i;
        if(i==0){
            buttons[i].pin = BUTTON_1;
        } else if(i==1){
            buttons[i].pin = BUTTON_2;
        } else if(i==2){
            buttons[i].pin = BUTTON_3;
        } else if(i==3){
            buttons[i].pin = BUTTON_4;
        }
    }
}
