#ifndef TILE_H
#define TILE_H

#include <raylib.h>

struct Tile {
    bool alive = false;
    int neighbors = 0;
    void act();
};

#endif //TILE_H
