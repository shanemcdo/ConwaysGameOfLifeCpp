#ifndef CONWAYS_GAME
#define CONWAYS_GAME

#include <raylib.h>
#include "Tile.h"

class ConwaysGame{
private:
    Vector2 window_size;
    float scale;
    int fps;
    Tile** grid;
    Vector2 grid_size;
    bool paused = false;
    void create_grid();
    void draw_grid_lines();
    void draw_grid();
    void draw();
    void randomize_grid();
    void step();
    void count_neighbors(int x, int y);
    void toggle_tile(int x, int y);
    void keyboard_input();
    void mouse_input();
public:
    ConwaysGame();
    ConwaysGame(float scl);
    ConwaysGame(float x, float y);
    ConwaysGame(float x, float y, float scl);
    ConwaysGame(float x, float y, float scl, int f);
    ~ConwaysGame();
    void run();
};

#endif //CONWAYS_GAME
