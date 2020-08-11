#include "ConwaysGame.h"

ConwaysGame::ConwaysGame():ConwaysGame(900, 600, 10, 30){
}

ConwaysGame::ConwaysGame(float scl):ConwaysGame(900, 600, scl, 30){
}

ConwaysGame::ConwaysGame(float x, float y):ConwaysGame(x, y, 10, 30){
}

ConwaysGame::ConwaysGame(float x, float y, float scl):ConwaysGame(x, y, scl, 30){
}

ConwaysGame::ConwaysGame(float x, float y, float scl, int f){
    window_size.x = x;
    window_size.y = y;
    scale = scl;
    fps = f;
}

ConwaysGame::~ConwaysGame(){
}

void ConwaysGame::run(){
    InitWindow(window_size.x, window_size.y, "Conway's Game of Life");
    SetTargetFPS(fps);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        EndDrawing();
    }
}
