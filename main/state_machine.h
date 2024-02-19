typedef enum STATE {
    Null = 0, // Initial state, when everything is being set up
    MainMenu,
    Game,
    Generate,
    Quit,
}STATE;

void setState(STATE newState);
enum STATE getState();

