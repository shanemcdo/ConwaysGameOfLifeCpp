#include<iostream>
#include "ConwaysGame.h"

// private

Vector2 ConwaysGame::get_grid_size(){
    return Vector2 {
        static_cast<unsigned>(window_size.x / scale),
        static_cast<unsigned>(window_size.y / scale)
    };
}

void ConwaysGame::create_grid(){
    Vector2 size = get_grid_size();
    int x = size.x;
    int y = size.y;
    grid = new bool*[y];
    for(int i = 0; i < y; i++){
        grid[i] = new bool[x];
        for(int j = 0; j < x; j++){
            grid[i][j] = false;
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
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
