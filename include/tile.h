#ifndef TILE_H
#define TILE_H

struct Tile {
    bool alive = false;
    int neighbors = 0;
    void act();
};

#endif //TILE_H
