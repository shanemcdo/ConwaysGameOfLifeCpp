#ifndef CONWAYS_GAME
#define CONWAYS_GAME

#include<raylib.h>

class ConwaysGame{
private:
    Vector2 window_size;
    float scale;
    int fps;
    int** grid;
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
