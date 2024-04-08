typedef enum STATE {
    Null = 0, // Initial state, when everything is being set up
    MainMenu,
    Game,
    Generate,
    End,
    Quit,
}STATE;

void set_state(STATE newState);
enum STATE get_state();

