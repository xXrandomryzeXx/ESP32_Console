#include "game.h"
#include "button.h"

void Input()
{
    uint8_t input = 0; 
    while(!input){
        input_handled = 0;
        for(int i = 0; i < 4; i++){
            if(get_button_state(i)){
                menu_selection = i;
                input_handled = 1;
            }
        }
        input = input_handled;
    } 
}
