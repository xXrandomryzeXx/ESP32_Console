#pragma once
#include <stdlib.h>
#include <stdio.h>

struct question{
    uint16_t index;
    uint8_t type;   // 0 - On reading 1 - Ku reading 2 - English meaning
    uint8_t answer;
    char *answers[4];
};

/*
 * @brief Generate a question from the available kanji in the kanji.txt
 *
 * @param this_question - A pointer to a struct variable which will store the generated question
 */
void generateQuestion(struct question *this_question);
