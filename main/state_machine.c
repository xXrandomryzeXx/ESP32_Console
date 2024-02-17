#include "state_machine.h"

STATE currentState = STATE.Null;

void setState(STATE newState)
{
    currentState = newState;
}


STATE getState()
{
    return currentState;
}
