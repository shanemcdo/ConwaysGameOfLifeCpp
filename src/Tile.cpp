#include "Tile.h"

void Tile::act(){
    if(alive){
        if(neighbors < 2) alive = false;
        else if(neighbors > 3) alive = false;
        else alive = true;
    }else if(neighbors == 3) alive = true;
    neighbors = 0;
}
