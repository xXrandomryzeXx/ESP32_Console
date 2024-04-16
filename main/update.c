#include "game.h"

#include <string.h>

#include "state_machine.h"

#include "display.h"
#include "text.h"
#include "sprite.h"
#include "question.h"
#include "esp_heap_caps.h"
#include "rand.h"
#include "memory.h"

#define TEXT_PADDING 20

/* IDs */
uint8_t text_id[4];
int8_t kanji_sprite_id = -1;
int8_t result_sprite_id = -1;
int8_t result_text_id = -1;
int8_t corrects_text_id = -1;
int8_t lives_text_id = - 1;
int8_t final_result_text_id = -1;

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
uint8_t highscore = 0;

uint8_t once = 1;

int8_t cleanup();

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
                answered = 1;
            }else{                 /* The answer is wrong */
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

            printf("Creating result sprite\n");
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

        /* Reset the options */
        for(int i = 0; i < 4; i++){
            options[i] = 0;
            delete_text(text_id[i]);
        }

        /* Create options */
        options[0] = 1; /* Play */
        /*options[2]*/ = 1; /* Quit */

        /* Create text for input options */
        text_id[0] = create_text(10, 100, "Play");
        /*text_id[2] = create_text(10, 170, "Quit");*/

        /* Load high score from SD */
        highscore = 0;
        char* data = malloc(4);
        esp_err_t ret = s_read_line(score_path, &data, 0);

        if(ret == ESP_OK){
            /* Convert  string to int */
            highscore = atoi(data);

            /* Create text for high score */
            char* score_string = malloc(16);
            sprintf(score_string, "Highscore: %d", highscore);
            text_id[1] = create_text(10, 60, score_string);
            free(score_string);
        }

        /* Create test sprite */
        //create_sprite("kanji/1", 0, 0, 1);

        /* Load background image */
        init_background_image_from_sd("/sdcard/bckg1.jpg");

    }else if(get_state() == Game){ /* Game state */ 
        /* At the start of a new game, generate a question */
        if(initial){
            initial = 0;
            set_state(Generate);
            ignoreInput();
            return;
        }

        if (cleanup() == -1){
            return;
        }

        /* After cleanup, check if the player has lost the game 
         * It's important that this is done before anything is created
         * otherwise we can have a memory leak */
        if(lives <= 0){
            set_state(End);
            ignoreInput();
            initial = 1;
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
            free(string);
        }

        /* Create text for number of lives */
        if(lives_text_id < 0){
            char *string = malloc(10);
            memset(string, 0, 10);
            sprintf(string, "Lives:%d", lives);
            lives_text_id = create_text(250, 10, string);
            free(string);
        }

        can_play = 1;

    } else if(get_state() == Generate) { /* Question generation */
        generateQuestion(&q); 
        once = 0;
        set_state(Game);
        ignoreInput();
        return;
    } else if(get_state() == End) {  /* Game over screen */
        if(initial){
            initial = 0;
            init_background_image_from_sd("/sdcard/gmovr.jpg");

            /* Create final result text */
            char *string = malloc(10);
            memset(string, 0, 10);
            sprintf(string, "%d", correct_answers);
            final_result_text_id = create_text(0, 0, string);
            uint16_t xPos = 160 - (8 * get_text_len(final_result_text_id));
            update_text(xPos, 140, final_result_text_id);
            free(string);

            if(correct_answers > highscore){
                char* new_data = malloc(5);
                sprintf(new_data, "%d", correct_answers);
                s_write_line(score_path, &new_data);
            }else{
                printf("%d is less than or equal to %d\n", correct_answers, highscore);
            }

        }else{
            /* Delete final result text */
            delete_text(final_result_text_id);

            set_state(MainMenu);
            ignoreInput();
            initial = 1;
            return;
        }
    }
    resetInput();
}

int8_t cleanup()
{
    printf("Total memory before cleanup %d\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    if(answered){ /* Skip this update cycle if an answer has been given */
        answered = 0;
        can_play = 0;
        printf("Wont delete result sprite\n");
        return -1;
    }else{ /* Delete result sprite and text if they exist after an answer hasn't been given */
        if(result_sprite_id >= 0){
            delete_sprite(result_sprite_id);
            result_sprite_id = -1;
        }
        if(result_text_id >= 0){
            delete_text(result_text_id);
            result_text_id = -1;
        }
    }
    /* Delete text of number of correct answers */
    if(corrects_text_id >= 0){
        delete_text(corrects_text_id);
        corrects_text_id = -1;
    }

    /* Delete text of number of lives */
    if(lives_text_id >= 0){
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

    printf("Total memory after cleanup %d\n", heap_caps_get_free_size(MALLOC_CAP_8BIT));
    return 0;
}
