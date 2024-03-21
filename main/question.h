#pragma once
#include <stdlib.h>
#include <stdio.h>

struct question{
    uint16_t index;
    uint8_t type;   // 0 - On reading 1 - Ku reading 2 - English meaning
    uint8_t answer;
    char *answers[4];
};

void generateQuestion(struct question *this_question);
