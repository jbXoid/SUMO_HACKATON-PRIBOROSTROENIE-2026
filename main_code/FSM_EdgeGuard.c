#include "main.h"

bool Edgeguard(void) {

    uint8_t line = PINC * 0x03

    if( line == 3 ) {

        return false;
        
    }

    last_edge = line;

    DC_motors(STOP,0,0);
    battle_state = EDGE_STOP;

    return true;


}
