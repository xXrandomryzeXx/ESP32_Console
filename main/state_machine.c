#include "state_machine.h"

#include <stdio.h>

STATE currentState = Null;

void setState(STATE newState)
{
    currentState = newState;
}


STATE getState()
{
    return currentState;
}
