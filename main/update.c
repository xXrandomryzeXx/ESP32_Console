#include "game.h"

#include <string.h>

#include "state_machine.h"

#include "display.h"
#include "text.h"
#include "sprite.h"
#include "question.h"

#define TEXT_PADDING 20

/* IDs */
uint8_t text_id[4];
uint8_t kanji_sprite_id;
uint8_t result_sprite_id;
uint8_t result_text_id;
int8_t corrects_text_id;
int8_t lives_text_id;

uint8_t options[4];
uint8_t initial = 1;
char *ans[4];

/* Game variables */
struct question q;
uint8_t can_play = 0;
int8_t answered = 0;
uint8_t background_loaded = 0;
uint8_t correct_answers = 0;
uint8_t lives = 3;

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
                answered = 1;
            }else{                 /* The answer is wrong */
                printf("Wrong answer!\n");
                answered = -1;
                if(q.type == 2){
                    result_text_id = create_text(0, 0, q.answers[q.answer]);
                }else{
                    result_text_id = create_japanese_text(0, 0, q.answers[q.answer]);
                }

                uint16_t xPos = 160; 
                uint8_t yPos = 130;
                if(q.type == 2){
                    xPos = xPos - (8 * get_text_len(result_text_id))/2; 
                }else{
                    xPos = xPos - (16 * get_text_len(result_text_id))/2;
                }
                update_text(xPos, yPos, result_text_id);
                set_text_layer(1, result_text_id);
            }
            switch(answered){
                case 1:
                    ++correct_answers;
                    result_sprite_id = create_sprite("crct", 96, 56, 5);
                    break;
                case -1:
                    --lives;
                    result_sprite_id = create_sprite("wrng", 96, 56, 5);
                    break;
            }
            set_sprite_layer(1, result_sprite_id);
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
        text_id[0] = create_text(10, 100, "Play");
        text_id[3] = create_text(10, 170, "Quit");
    }else if(getState() == 2){ /* Game state */
        if(initial){
            initial = 0;
            setState(Generate);
            ignoreInput();
            return;
        }

        if(answered){ 
            answered = 0;
            return;
        }else{
            if(result_sprite_id){
                delete_sprite(result_sprite_id);
            }
            if(result_text_id){
                delete_text(result_text_id);
            }
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
        if(!background_loaded){
            init_background_image_from_sd("/sdcard/bckg2.jpg");
            background_loaded = 1;
        }


        /* Create the kanji sprite */
        char *filename = malloc(14*sizeof(char));
        sprintf(filename, "kanji/%d", q.index + 1);
        kanji_sprite_id = create_sprite(filename, 128, 20, 1);
        free(filename); 

        /* Create answers text */
        for(int i = 0; i < 4; i++){

            options[i] = 1;
            if(q.type == 2){ /* English Text */
                text_id[i] = create_text(0, 0, q.answers[i]);
            }else{           /* Japanese Text */
                text_id[i] = create_japanese_text(0, 0, q.answers[i]);
            }
            uint8_t a = i%2;
            uint16_t xPos = a*(319 - TEXT_PADDING) + (!a*TEXT_PADDING); 
            /* Depedning on the type of answer (english or japanese), we choose 16px
             * per char, or 8px per char */
            if(q.type == 2){
                xPos = xPos - ((a*8) * get_text_len(text_id[i]));
            }else{
                xPos = xPos - ((a*16) * get_text_len(text_id[i]));
            }
            uint8_t yPos = ((i >= 2) * 80) + 110;

            update_text(xPos, yPos, text_id[i]);
        }

        printf("Corrects id: %d\n", corrects_text_id);
        /* Create question number text */
        if(corrects_text_id >= 0){
            printf("deleting score\n");
            delete_text(corrects_text_id);
            corrects_text_id = -1;
        }
        if(corrects_text_id < 0){
            printf("Creating score\n");
            char *string = malloc(14);
            memset(string, 0, 14);
            sprintf(string, "Correct:%d", correct_answers);
            corrects_text_id = create_text(10, 10, string);
            free(string);
        }


        /* Create sprites for number of lives */

        can_play = 1;

    } else if(getState() == 3) { /* Question generation */
        generateQuestion(&q);
        setState(Game);
        ignoreInput();
        return;
    }
    resetInput();
}
