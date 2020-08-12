#ifndef TILE_H
#define TILE_H

#include <time.h>

struct Tile {
    bool alive = false;
    int neighbors = 0;
    time_t last_toggled = time(0);
    Tile();
    void act();
};

#endif //TILE_H
