#include "state_machine.h"

#include <stdio.h>

STATE currentState = Null;

void set_state(STATE newState)
{
    currentState = newState;
}


STATE get_state()
{
    return currentState;
}
