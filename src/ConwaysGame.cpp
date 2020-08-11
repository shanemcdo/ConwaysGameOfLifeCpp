#include <time.h>
#include <math.h>
#include <iostream>
#include "ConwaysGame.h"

// private

void ConwaysGame::create_grid(){
    int x = ceil(window_size.x / scale);
    int y = ceil(window_size.y / scale);
    grid_size.x = x;
    grid_size.y = y;
    grid = new Tile*[y];
    for(int i = 0; i < y; i++){
        grid[i] = new Tile[x];
    }
}

void ConwaysGame::draw_grid_lines(){
    for(int i = 0; i <= window_size.y; i += scale)
        DrawLine(0, i, window_size.x, i, Color{125, 125, 125, 125});
    for(int i = 0; i <= window_size.x; i += scale)
        DrawLine(i, 0, i, window_size.y, Color{125, 125, 125, 125});
}

void ConwaysGame::draw_grid(){
    for(int i = 0; i < grid_size.y; i++)
        for(int j = 0; j < grid_size.x; j++)
            if(grid[i][j].alive)
                DrawRectangle(j * scale, i * scale, scale, scale, BLACK);
}

void ConwaysGame::draw(){
    draw_grid_lines();
    draw_grid();
}

void ConwaysGame::randomize_grid(){
    for (int i = 0; i < grid_size.y; i++)
        for (int j = 0; j < grid_size.x; j++)
            grid[i][j].alive = rand() % 2;
}

// public

ConwaysGame::ConwaysGame():ConwaysGame(1200, 660, 10, 10){
}

ConwaysGame::ConwaysGame(float scl):ConwaysGame(1200, 660, scl, 10){
}

ConwaysGame::ConwaysGame(float x, float y):ConwaysGame(x, y, 10, 10){
}

ConwaysGame::ConwaysGame(float x, float y, float scl):ConwaysGame(x, y, scl, 10){
}

ConwaysGame::ConwaysGame(float x, float y, float scl, int f){
    window_size.x = x;
    window_size.y = y;
    scale = scl;
    fps = f;
    srand(time(0));
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
        randomize_grid();
        draw();
        EndDrawing();
    }
}
