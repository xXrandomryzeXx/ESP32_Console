#include "rand.h"

#include <time.h>
#include <string.h>
#include <esp_system.h>

#include "memory.h"

uint16_t getRandNum(uint16_t range)
{
    // Get total seconds
    char *line = malloc(64*sizeof(char));
    esp_err_t ret = s_read_line(time_path, &line, 0);

    if(ret == ESP_OK){
        long long total_secs = atoi(line);
        time_t secs;

        // Generate seed
        srand(total_secs + time(&secs));

        // Add current seconds to the total seconds
        total_secs += time(&secs);

        // Get length of total_secs string
        uint16_t length = snprintf(NULL, 0, "%lld", total_secs);

        snprintf(line, length+1, "%lld", total_secs);

        s_write_line(time_path, &line);
    }
    free(line);
    return rand() % (int)range;
}
