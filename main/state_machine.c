#include "state_machine.h"

#include <stdio.h>

STATE currentState = Null;

void setState(STATE newState)
{
    printf("Switching current State: %d to State: %d\n", currentState, newState);
    currentState = newState;
}


STATE getState()
{
    return currentState;
}
