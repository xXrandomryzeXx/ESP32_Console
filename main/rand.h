#include <stdint.h>

/*
 * @brief   Generates a pseudo random value between 0 and range, saves and loads a seed
 *          from time.txt. The seed is seconds passed and it's added every time this
 *          function is used
 *
 * @param range - 16-bit value for the max value for range
 *
 * @return      - A random value between 0 and range
 *              - If range is 0, return will always be 0
 */
uint16_t getRandNum(uint16_t range);
