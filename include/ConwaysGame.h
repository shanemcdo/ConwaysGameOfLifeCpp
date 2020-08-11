#ifndef CONWAYS_GAME
#define CONWAYS_GAME

#include<raylib.h>

class ConwaysGame{
private:
    Vector2 window_size;
    float scale;
    int fps;
    bool** grid;
    Vector2 grid_size;
    void create_grid();
    void draw_grid_lines();
    void draw_grid();
    void draw();
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
