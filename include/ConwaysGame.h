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
/* TODO: create copy paste <12-08-20, Shane> */
/* TODO: Drag to highlight squares <12-08-20, Shane> */
private:
    static constexpr Vector2 DEFAULT_WINDOW_SIZE = Vector2{1300, 660};
    static constexpr float DEFAULT_SCALE = 10;
    static constexpr ColorScheme DEFAULT_SCHEME = BlackAndWhite;
    static const int DEFAULT_FPS = 30;
    Vector2 window_size;
    float scale;
    int fps;
    Tile** grid;
    Vector2 grid_size;
    bool paused = false;
    ColorScheme scheme = BlackAndWhite;
    bool loop_walls = false;
    bool showing_help = true;
    bool showing_grid_lines = true;
    void create_grid();
    void resize_grid();
    void draw_grid_lines();
    void draw_grid();
    void draw_walls();
    void draw_help();
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
    void reset_to_default();
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
