#include "question.h"

#include <time.h>

#include "memory.h"

const char *path = "/sdcard/kanji.txt";

uint16_t getRandNum(uint16_t range)
{
    // Get total seconds
    char *line = malloc(64*sizeof(char));
    s_read_line(path, &line, 1);
    long long total_secs = atoi(line); 
    time_t secs;

    // Generate seed
    srand(total_secs + time(&secs));

    // Add current seconds to the total seconds
    total_secs += time(&secs);
    uint16_t length = snprintf(NULL, 0, "%lld", total_secs);
    snprintf(line, length+1, "%lld", total_secs);

    s_write_line(path, line, 1);

    return rand() % range;
}

void generateQuestion()
{
    struct question this_question;
    // Allocate memory for line reading
    char *line = malloc(64*sizeof(char));
    // Get the number of kanji available
    uint16_t all = 0;

    s_read_line(path, &line, 0);

    all = atoi(line);  // Convert string to int

    printf("Available kanji to choose from: %d\n", all);
    this_question.index = getRandNum(all);

    // Has to be +3 and not +2 because of the first two lines in the file
    uint16_t target_line = this_question.index * 4 + 3; // The line of the kanji number
    printf("Kanji line: %d\n", target_line);

    // Generate type
    this_question.type = getRandNum(3);
    printf("Type: %d\n", this_question.type);
    uint16_t answer_line = target_line + 1*(this_question.type);

    // Get an answer
    s_read_line(path, &line, answer_line);
    printf("Answer to kanji: %d is %s\n", target_line, line);

    // Get the kanji image name 
}
