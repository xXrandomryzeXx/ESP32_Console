#include "game.h"
#include "button.h"

extern int8_t input;

void Input()
{
    while(1 && input >= 0){
        resetInput();
        for(int i = 0; i < 4; i++){
            if(get_button_state(i)){
                printf("Button #%d pressed\n", i);
                selectMenu(i);
                sendInput();
                return;
            }
        }
    } 
    printf("ignoring input\n");
    input = 1;
}
