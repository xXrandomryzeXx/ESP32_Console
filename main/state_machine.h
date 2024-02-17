enum STATE {
    Null = 0, // Initial state, when everything is being set up
    MainMenu,
    Game,
    Generate,
    Quit,
};

void setState(STATE newState);
STATE getState();

