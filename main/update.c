#include "game.h"

#include <string.h>

#include "state_machine.h"

#include "display.h"
#include "text.h"
#include "sprite.h"
#include "question.h"

/* IDs */
uint8_t text_id[4];
uint8_t kanji_sprite_id;

uint8_t options[4];
uint8_t initial = 1;
char *ans[4];

/* Game variables */
struct question q;
uint8_t can_play = 0;

void Update()
{
    uint8_t input = getSelectedMenu();

    /* If input is not from the options, skip */
    if(!options[input] && !initial)
        return;

    /* Check inputs
     * Game logic goes in here */
    if(options[input] && !initial){
        if(getState() == 1){ /* Main input */
            if(input == 0){ /* Play button pressed */
                setState(Game);
                initial = 1;
            }
        }else if(getState() == 2 && can_play){ /* Game logic */
            /* Answer questions */
            if(input == q.answer){ /* The answer is correct */
                printf("Correct answer!\n"); 
            }else{                 /* The answer is wrong */
                printf("Wrong answer!\n");
            }
            setState(Generate);
            ignoreInput();
            can_play = 0;
        }
    }


    /* This activates on state change
     * Load and set up the scenes */
    if(getState() == 1){ /* Main input */
        if(initial)
            initial = 0;
        /* Create options */
        /* Reset the options */
        for(int i = 0; i < 4; i++){
            options[i] = 0;
            delete_text(text_id[i]);
        }
        
        options[0] = 1; /* Play */
        options[2] = 1; /* Quit */

        /* Create text for input options */
        text_id[0] = create_text(10, 100, "Play", 4);
        text_id[3] = create_text(10, 170, "Quit", 4);
    }else if(getState() == 2){ /* Game state */
        if(initial){
            initial = 0;
            setState(Generate);
            ignoreInput();
            return;
        }
        
        /* Reset options */
        for(int i = 0; i < 4; i++){
            options[i] = 0; /* Set all options to 0 - meaning nothing to select */
            delete_text(text_id[i]); /* Delete all texts */
        }

        /* Delete last sprite if it exists */
        if(kanji_sprite_id){
           delete_sprite(kanji_sprite_id);
        }

        /* Change background */
        init_background_image_from_sd("/sdcard/bckg2.jpg");

 
        /* Create the kanji sprite */
        char *filename = malloc(8*sizeof(char));
        sprintf(filename, "%d", q.index + 1);
        kanji_sprite_id = create_sprite(filename, 128, 20, 1);
        free(filename); 

        /* Create answers text */
        for(int i = 0; i < 4; i++){
            uint16_t xPos = ((i%2) * 220) + 20;
            uint8_t yPos = ((i >= 2) * 80) + 110;
            options[i] = 1;
            if(q.type == 2){ /* English Text */
               text_id[i] = create_text(xPos, yPos, q.answers[i], 8);
            }else{           /* Japanese Text */
               text_id[i] = create_japanese_text(xPos, yPos, q.answers[i], 16);
            }
        }

        can_play = 1;

    } else if(getState() == 3) { /* Question generation */
        generateQuestion(&q);
        setState(Game);
        ignoreInput();
        return;
    }
    resetInput();
}
