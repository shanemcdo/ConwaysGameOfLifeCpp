#include "ConwaysGame.h"

// private

void ConwaysGame::create_grid(){
    int x = window_size.x / scale;
    int y = window_size.y / scale;
    grid_size.x = x;
    grid_size.y = y;
    grid = new bool*[y];
    for(int i = 0; i < y; i++){
        grid[i] = new bool[x];
        for(int j = 0; j < x; j++){
            grid[i][j] = false;
        }
    }
}

// public

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
    for(int i = 0; i < grid_size.y; i++){
        delete grid[i];
    }
    delete grid;
    grid = nullptr;
}

void ConwaysGame::run(){
    InitWindow(window_size.x, window_size.y, "Conway's Game of Life");
    SetTargetFPS(fps);
    create_grid();
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        EndDrawing();
    }
}
