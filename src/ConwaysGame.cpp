#include "ConwaysGame.h"

ConwaysGame::ConwaysGame():ConwaysGame(900, 600, 10){
}

ConwaysGame::ConwaysGame(float scl):ConwaysGame(900, 600, scl){
}

ConwaysGame::ConwaysGame(float x, float y):ConwaysGame(x, y, 10){
}

ConwaysGame::ConwaysGame(float x, float y, float scl){
    window_size.x = x;
    window_size.y = y;
    scale = scl;
}

void ConwaysGame::run(){
    InitWindow(window_size.x, window_size.y, "Conway's Game of Life");
    while(!WindowShouldClose()){
    }
}
