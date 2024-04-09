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
int8_t kanji_sprite_id = -1;
int8_t result_sprite_id = -1;
int8_t result_text_id = -1;
int8_t corrects_text_id = -1;
int8_t lives_text_id = - 1;

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
    if(!options[input] && !initial && (get_state() == Game || get_state() == MainMenu))
        return;

    /* Check inputs
     * Game logic goes in here */
    if(options[input] && !initial){
        if(get_state() == MainMenu){ /* Main input */
            if(input == 0){ /* Play button pressed */
                set_state(Game);
                initial = 1;
                can_play = 0;
                answered = 0;
                background_loaded = 0;
                correct_answers = 0;
                lives = 3;
            }
        }else if(get_state() == Game && can_play){ /* Game logic */
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
                printf("Result text id is %d\n", result_text_id);
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
            set_state(Generate);
            ignoreInput();
            can_play = 0;
        }
    }


    /* This activates on state change
     * Load and set up the scenes */
    if(get_state() == MainMenu){ /* Main input */
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

        /* Load background image */
        init_background_image_from_sd("/sdcard/bckg1.jpg");

    }else if(get_state() == Game){ /* Game state */
        if(initial){
            initial = 0;
            set_state(Generate);
            ignoreInput();
            return;
        }

        if(answered){ /* Skip this update cycle if an answer has been given */
            answered = 0;
            return;
        }else{ /* Delete result sprite and text if they exist after an answer hasn't been given */
            if(result_sprite_id >= 0){
                delete_sprite(result_sprite_id);
                result_sprite_id = -1;
            }
            if(result_text_id >= 0){
                printf("Deleting result text id %d\n", result_text_id);
                delete_text(result_text_id);
                result_text_id = -1;
            }
        }
        /* Delete text of number of correct answers */
        if(corrects_text_id >= 0){
            printf("deleting score text id %d\n", corrects_text_id);
            delete_text(corrects_text_id);
            corrects_text_id = -1;
        }
        
        /* Delete text of number of lives */
        if(lives_text_id >= 0){
            printf("deleting lives text id %d\n", lives_text_id);
            delete_text(lives_text_id);
            lives_text_id = -1;
        }

        /* Reset options */
        for(int i = 0; i < 4; i++){
            options[i] = 0; /* Set all options to 0 - meaning nothing to select */
            delete_text(text_id[i]); /* Delete all texts */
        }

        /* Delete last kanji sprite if it exists */
        if(kanji_sprite_id >= 0){
            delete_sprite(kanji_sprite_id);
            kanji_sprite_id = -1;
        }

        /* After cleanup, check if the player has lost the game 
         * It's important that this is done before anything is created
         * otherwise we can have a memory leak */
        if(lives <= 0){
            set_state(End);
            ignoreInput();
            initial = 1;
            printf("Changing state\n");
            return;
        }

        /* Change background */
        if(!background_loaded){
            init_background_image_from_sd("/sdcard/bckg2.jpg");
            background_loaded = 1;
        }


        /* Create the kanji sprite */
        if(kanji_sprite_id < 0){
            char *filename = malloc(14*sizeof(char));
            sprintf(filename, "kanji/%d", q.index + 1);
            kanji_sprite_id = create_sprite(filename, 128, 20, 1);
            free(filename); 
        }

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
        
        /* Create text for number of correct answers */
        if(corrects_text_id < 0){
            char *string = malloc(14);
            memset(string, 0, 14);
            sprintf(string, "Correct:%d", correct_answers);
            corrects_text_id = create_text(10, 10, string);
            printf("creating score text id %d\n", corrects_text_id);
            free(string);
        }
        
        /* Create text for number of lives */
        if(lives_text_id < 0){
            char *string = malloc(10);
            memset(string, 0, 10);
            sprintf(string, "Lives:%d", lives);
            lives_text_id = create_text(250, 10, string);
            printf("creating lives text id %d\n", lives_text_id);
            free(string);
        }

        can_play = 1;

    } else if(get_state() == Generate) { /* Question generation */
        generateQuestion(&q);
        set_state(Game);
        ignoreInput();
        return;
    } else if(get_state() == End) {  /* Game over screen */
        if(initial){
            initial = 0;
            printf("End screen\n");
            init_background_image_from_sd("/sdcard/gmovr.jpg");
        }else{
            printf("Going to main menu\n");
            set_state(MainMenu);
            ignoreInput();
            initial = 1;
            return;
        }
    }
    resetInput();
}
