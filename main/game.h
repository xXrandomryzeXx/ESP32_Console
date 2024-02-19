#pragma once
/*
 * game.h contians the base of the logic of the game
 * It has the main methods - Update(), Render()
 * It connects all parts of the code together
 */

#include <stdint.h>

static uint8_t menu_selection;
static uint8_t input_handled;

/*
 * @brief   Tracks and updates all logic. Loads files, generates questions, manages memory, etc.
 */
void Update();


/*
 * @brief   Renders all graphics to the screen, background image, sprites and texts
 */
void Render();

/*
 * @brief   Starts the game loop
 */
void Start();

/*
 * @brief   Wait for user input
 */
void Input();
