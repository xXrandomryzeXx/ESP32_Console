#include "game.h"

#include "state_machine.h"

#include "display.h"
#include "text.h"

uint8_t options_text[4];
uint8_t options[4];
uint8_t initial = 1;

void Update()
{
    // If input is not from the options, skip
    if(!options[menu_selection] && !initial)
        return;

    // Check inputs
    if(options[menu_selection] && !initial){
        if(getState() == 1){ // Main menu
            if(menu_selection == 0){ // Play button pressed
                setState(Game);
                initial = 1;
                printf("Play!\n");
            }
        }
    }


    // This activates on state change
    if(getState() == 1){ // Main menu
        if(initial)
            initial = 0;
        // Create options
        // Reset the options
        for(int i = 0; i < 4; i++){
            options[i] = 0;
            delete_text(options_text[i]);
        }
        
        options[0] = 1; // Play
        options[2] = 1; // Quit

        // Create text for menu options
        options_text[0] = create_text(10, 100, "Play", 4);
        options_text[3] = create_text(10, 170, "Quit", 4);
    }else if(getState() == 2){
        if(initial)
            initial = 0;

        // Reset options
        for(int i = 0; i < 4; i++){
            options[i] = 0;
            delete_text(options_text[i]);
        }

        // Change background
        init_background_image_from_sd("/sdcard/bckg2.jpg");

    }
    input_handled = 0;
}
