#include "question.h"

#include <esp_system.h>
#include <string.h>

#include "memory.h"
#include "rand.h"

void generateQuestion(struct question *this_question)
{
    //this_question = malloc(sizeof(this_question));
    // Allocate memory for the anwer strings
    for(int i = 0; i < 4; i++){
        // This allocation shouldn't be freed because it's used by another code.
        this_question->answers[i] = malloc(64*sizeof(char));
        memset(this_question->answers[i], 0, 64*sizeof(char));
    }

    // Allocate memory for line reading
    char *line = malloc(64*sizeof(char));

    // Get the number of kanji available
    uint16_t all = 0;

    s_read_line(kanji_path, &line, 0);

    all = atoi(line);  // Convert string to int

    printf("Available kanji to choose from: %d\n", all);
    this_question->index = getRandNum(all);

    // Has to be +2 and not +1 because of the first line in the file
    // The line of the kanji number
    uint16_t target_line = this_question->index * 4 + 2; 

    // Generate type
    this_question->type = getRandNum(3);
    //this_question->type = 1;
    printf("Type: %d\n", this_question->type);
    
    // Store answer indexes
    uint16_t indexes[4];

    // Get an answer
    this_question->answer = getRandNum(4);
    indexes[this_question->answer] = target_line;
    
    printf("Ans index: %d\n", this_question->answer);

    // Generate random indexes for the rest of the answers
    for(uint8_t i = 0; i < 4; i++){
        if(i == this_question->answer){
            continue;
        }

        uint8_t clean;
        uint16_t index;

        do{
            clean = 1;
            index = getRandNum(all);    
            index = index * 4 + 2;
            for(int j = 0; j < 4; j++){
                if(indexes[j] == index && j != i){
                    clean = 0;
                    index = 0;
                }
            }
        }while(!clean);

        indexes[i] = index;
    }

    // Get values for the answers
    for(uint8_t i = 0; i < 4; i++){
        s_read_line(kanji_path, &line, indexes[i] + 1*this_question->type );
        
        // Split answers by the ',' symbol and select one at random
        char answer_line_array[4][64];
        uint8_t end = 0;
        uint8_t ite = 0;

        for(uint8_t j = 3; j < 64; j++){
            if(ite >= 4)
                break;
            if(line[j] == ','){
                answer_line_array[ite][end] = '\0';
                ite++;
                end = 0;
                continue;
            }
            if(line[j] == '\0'){
                answer_line_array[ite][--end] = '\0';
                break;
            }
            answer_line_array[ite][end] = line[j];
            end++;
        }
        strcpy(this_question->answers[i], answer_line_array[0]);
        for(int j = 0; j < 64; j++){
            printf("%c", this_question->answers[i][j]);
        }
        printf("\n");
    } 
    
    free(line);
}
