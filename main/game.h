#pragma once
/*
 * game.h contians the base of the logic of the game
 * It has the main methods - Update(), Render()
 * It connects all parts of the code together
 */

#include <stdint.h>

/*
 * @brief   Sends a signal that an input has been received
 */
void sendInput();

/*
 * @brief   Reset the signal receiver
 */
void resetInput();

/*
 * @brief   Sets the selected option
 *
 * @param   menu - Which menu from all 4 has been selected
 */
void selectMenu(uint8_t menu);

/*
 * @brief   Returns the selected menu
 *
 * @return  uint8_t value of the selected menu
 */
uint8_t getSelectedMenu();

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
