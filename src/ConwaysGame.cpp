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

void ConwaysGame::clear_grid(){
    for (int i = 0; i < grid_size.y; i++)
        for (int j = 0; j < grid_size.x; j++)
            grid[i][j].alive = false;
}

void ConwaysGame::step(){
    for(int i = 0; i < grid_size.y; i++)
        for(int j = 0; j < grid_size.x; j++)
            count_neighbors(j, i);
    for(int i = 0; i < grid_size.y; i++)
        for(int j = 0; j < grid_size.x; j++)
            grid[i][j].act();
}

void ConwaysGame::count_neighbors(int x, int y){
    for(int i = -1; i <= 1; i++)
        for(int j = -1; j <= 1; j++)
            if(y + i >= 0 && y + i < grid_size.y && x + j >= 0 && x + j < grid_size.x && grid[y + i][x + j].alive && (i != 0 || j != 0))
                grid[y][x].neighbors++;
}

void ConwaysGame::toggle_tile(int x, int y){
    x /= scale;
    y /= scale;
    grid[y][x].alive = !grid[y][x].alive;
}

void ConwaysGame::keyboard_input(){
    switch (GetKeyPressed()) {
        case 'R':
        case 'r':
            randomize_grid();
            break;
        case 'P':
        case 'p':
            paused = !paused;
            break;
        case ' ':
            step();
            break;
        case 'C':
        case 'c':
            clear_grid();
            break;
        default:
            break;
    }
}

void ConwaysGame::mouse_input(){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        toggle_tile(GetMouseX(), GetMouseY());
    }
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
        draw();
        if(!paused)
            step();
        keyboard_input();
        mouse_input();
        EndDrawing();
    }
}
