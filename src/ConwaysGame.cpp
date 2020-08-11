#include "ConwaysGame.h"

ConwaysGame::ConwaysGame():ConwaysGame(900, 600){
}

ConwaysGame::ConwaysGame(float x, float y){
    window_size.x = x;
    window_size.y = y;
}

void ConwaysGame::run(){
    InitWindow(window_size.x, window_size.y, "Conway's Game of Life");
    while(!WindowShouldClose()){
    }
}
