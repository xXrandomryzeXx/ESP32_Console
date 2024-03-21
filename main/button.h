#include <stdlib.h>
#include "esp_system.h"
#include "esp_err.h"

#define BUTTON_1    36
#define BUTTON_2    39
#define BUTTON_3    34
#define BUTTON_4    35

typedef struct{
    uint8_t id;
    uint8_t state;
    uint8_t pin;
} button;

uint8_t get_button_state(uint8_t button_id);
void buttons_init();

