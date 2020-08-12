#ifndef CONWAYS_GAME
#define CONWAYS_GAME

#include <raylib.h>
#include "Tile.h"

enum ColorScheme{
    BlackAndWhite,
    Rainbow,
    Red,
    Green,
    Blue,
    LOOP_TO_START
};

class ConwaysGame{
private:
    Vector2 window_size;
    float scale;
    int fps;
    Tile** grid;
    Vector2 grid_size;
    bool paused = false;
    ColorScheme scheme = BlackAndWhite;
    bool loop_walls = false;
    void create_grid();
    void draw_grid_lines();
    void draw_grid();
    void draw_walls();
    void draw();
    void draw_paused();
    void draw_target_fps();
    void randomize_grid();
    void clear_grid();
    void invert_grid();
    void step();
    void count_neighbors(int x, int y);
    void toggle_tile(int x, int y);
    void keyboard_input();
    void mouse_input();
    void cycle_schemes();
    Color get_color(int i, int j);
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
