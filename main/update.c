#include "game.h"

#include <string.h>

#include "state_machine.h"

#include "display.h"
#include "text.h"
#include "sprite.h"
#include "question.h"

uint8_t options_text[4];
uint8_t options[4];
uint8_t initial = 1;
char *ans[4];

// Game variables
struct question q;
uint8_t can_play = 0;

void Update()
{
    uint8_t menu = getSelectedMenu();

    // If input is not from the options, skip
    if(!options[menu] && !initial)
        return;

    // Check inputs
    // Game logic goes in here
    if(options[menu] && !initial){
        if(getState() == 1){ // Main menu
            if(menu == 0){ // Play button pressed
                printf("Play!\n");
                setState(Game);
                initial = 1;
            }
        }else if(getState() == 2 && can_play){ // Game logic
        
        }
    }


    // This activates on state change
    // Load and set up the scenes
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
    }else if(getState() == 2){ // Game state 
        if(initial){
            initial = 0;
            setState(Generate);
            ignoreInput();
            return;
        }
        
        // Reset options
        for(int i = 0; i < 4; i++){
            options[i] = 0; // Set all options to 0 - meaning nothing to select
            delete_text(options_text[i]); // Delete all texts
            //delete_japanese_text(options_text[i]);
        }

        // Change background
        init_background_image_from_sd("/sdcard/bckg2.jpg");

        // Create the kanji sprite
        char *filename = malloc(8*sizeof(char));
        sprintf(filename, "%d", q.index + 1);
        uint8_t kanji_sprite = create_sprite(filename, 128, 20, 1);
        
        // Create answers text
        for(int i = 0; i < 4; i++){
            options[i] = 1;
            if(q.type == 2){ /* English Text */
               options_text[i] = create_text(20, 50 + (20*i), q.answers[i], 8);
            }else{           /* Japanese Text */
               options_text[i] = create_japanese_text(20, 50 + (20*i), q.answers[i], 16);
            }
        }

        can_play = 1;

    } else if(getState() == 3) { // Question generation
        printf("Generating question\n");
        generateQuestion(&q);
        /*for(int i = 0; i < 4; i++){
            ans[i] = malloc(sizeof(char) * 64);
            strcpy(ans[i], q.answers[i]);
        }*/
        setState(Game);
        ignoreInput();
        return;
    }
    resetInput();
}
