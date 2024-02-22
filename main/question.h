#pragma once
#include <stdlib.h>
#include <stdio.h>

struct question{
    uint16_t index;
    uint8_t type;   // 0 - On reading 1 - Ku reading 2 - English meaning
    char *text;
    char *answer;
    char *wrongs[];
};

void generateQuestion();
