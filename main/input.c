#include "game.h"
#include "button.h"

void Input()
{
    while(1){
        resetInput();
        for(int i = 0; i < 4; i++){
            if(get_button_state(i)){
                selectMenu(i);
                sendInput();
                return;
            }
        }
    } 
}
