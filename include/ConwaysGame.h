#ifndef CONWAYS_GAME
#define CONWAYS_GAME

#include<raylib.h>

class ConwaysGame{
private:
    Vector2 window_size;
public:
    ConwaysGame();
    ConwaysGame(float x, float y);
    void run();
};

#endif //CONWAYS_GAME
