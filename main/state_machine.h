/* States of the game:
 * Null - The initial state, everything is being set up and is loading
 * MainMenu - The main menu of the game
 * Game - Answering questions
 * Generate - The state which the game is in while generating a new question
 * End - After the lives run out, show final score
 */
typedef enum STATE {
    Null = 0, // Initial state, when everything is being set up
    MainMenu,
    Game,
    Generate,
    End,
    Quit,
}STATE;

/*
 * @brief Set a new state to the game
 *
 * @param newState     - The new state of the game, uses the enum STATE
 */
void set_state(STATE newState);

/*
 * @brief Returns the current state of the game
 *
 * @return      - The current state of the game, returns the enum STATE
 */
enum STATE get_state();

