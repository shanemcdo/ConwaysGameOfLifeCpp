#include <time.h>
#include <math.h>
#include <iostream>
#include <algorithm>
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

void ConwaysGame::resize_grid(){
    Tile** temp;
    Vector2 temp_size = grid_size;
    temp = new Tile*[static_cast<unsigned>(temp_size.y)];
    for(int i = 0; i < temp_size.y; i++){
        temp[i] = new Tile[static_cast<unsigned>(temp_size.x)];
        for(int j = 0; j < temp_size.x; j++){
            temp[i][j] = grid[i][j];
        }
    }
    this->~ConwaysGame();
    create_grid();
    int x_length = std::min(temp_size.x, grid_size.x);
    int y_length = std::min(temp_size.y, grid_size.y);
    for(int i = 0; i < y_length; i++){
        for(int j = 0; j < x_length; j++){
            grid[i][j] = temp[i][j];
        }
    }
    for(int i = 0; i < temp_size.y; i++){
        delete[] temp[i];
    }
    delete[] temp;
}

void ConwaysGame::draw_grid_lines(){
    for(int i = 0; i <= window_size.y; i += scale)
        DrawLine(0, i, window_size.x, i, Color{125, 125, 125, 100});
    for(int i = 0; i <= window_size.x; i += scale)
        DrawLine(i, 0, i, window_size.y, Color{125, 125, 125, 100});
}

void ConwaysGame::draw_grid(){
    for(int i = 0; i < grid_size.y; i++)
        for(int j = 0; j < grid_size.x; j++)
            if(grid[i][j].alive)
                DrawRectangle(j * scale, i * scale, scale, scale, get_color(i, j));
}

void ConwaysGame::draw_walls(){
    DrawRectangleLinesEx(
            Rectangle{0, 0, window_size.x, window_size.y},
            2,
            RED
            );
}

void ConwaysGame::draw_help(){
    const int arr_size = 15;
    const int font_size = 20;
    int widest = -1;
    const char* s[arr_size] = {
        "H - Toggle help menu",
        "Click - Toggle tile",
        "Space - step manually",
        "Esc - Close window",
        "L - Toggle loopable walls",
        "C - Clear tiles",
        "I - Invert tiles",
        "R - Randomize tiles",
        "P - Pause/Resume auto-stepping",
        "D - Reset to default values",
        "B - Toggle grid lines",
        "+/- - Increase/Decrease framerate",
        "[/] - Increase/Decrease scale",
        "S - Cycle through color schemes",
        "1, 2, 3, 4, 5 - Select color Scheme"
    };
    for(int i = 0; i < arr_size; i++){
        int s_width = MeasureText(s[i], font_size);
        if(widest < s_width){
            widest = s_width;
        }
        DrawText(s[i], (window_size.x - s_width) / 2, 10 + font_size * i, font_size, BLACK);
    }
    DrawRectangle((window_size.x - widest) / 2 - 10, 5, widest + 20, arr_size * font_size + 10, Color{0, 0, 0, 90});
}

void ConwaysGame::draw(){
    draw_grid();
    if(showing_grid_lines)
        draw_grid_lines();
    draw_target_fps();
    if(!loop_walls)
        draw_walls();
    if(showing_selection)
        draw_selection();
    if(showing_help)
        draw_help();
}

void ConwaysGame::draw_paused(){
    int font_size = 50;
    int padding = 10;
    const char* s = "Paused";
    int text_width = MeasureText(s, font_size);
    DrawText(s, window_size.x - text_width - padding, padding, font_size, Color{200, 0, 0, 120});
}

void ConwaysGame::draw_target_fps(){
    DrawText(TextFormat("%i FPS target", fps), 5, 5, 20, LIME);
    DrawFPS(5, 25);
}

void ConwaysGame::draw_selection(){
    Vector2 start{selection_start.x * scale, selection_start.y * scale};
    Vector2 end{selection_end.x * scale, selection_end.y * scale};
    Vector2 size{abs(end.x - start.x) + scale , abs(end.y - start.y) + scale};
    DrawRectangleV(start, Vector2{scale, scale}, Color{0, 0, 255, 60});
    DrawRectangleV(end, Vector2{scale, scale}, Color{0, 0, 255, 60});
    if(end.x < start.x)
        start.x = end.x;
    if(end.y < start.y)
        start.y = end.y;
    DrawRectangleV(start, size, Color{0, 255, 0, 120});
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

void ConwaysGame::invert_grid(){
    for (int i = 0; i < grid_size.y; i++)
        for (int j = 0; j < grid_size.x; j++)
            grid[i][j].alive = !grid[i][j].alive;
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
        for(int j = -1; j <= 1; j++){
            int xx = x + j;
            int yy = y + i;
            if((xx < 0 || xx >= grid_size.x || yy < 0 || yy >= grid_size.y)){
                if(loop_walls){
                    if(xx < 0)
                        xx += grid_size.x;
                    else if(xx >= grid_size.x)
                        xx -= grid_size.x;
                    if(yy < 0)
                        yy += grid_size.y;
                    else if(yy >= grid_size.y)
                        yy -= grid_size.y;
                }else
                    continue;
            }
            if(grid[yy][xx].alive && (i != 0 || j != 0))
                grid[y][x].neighbors++;
        }
}

void ConwaysGame::toggle_tile(int x, int y, bool click){
    x /= scale;
    y /= scale;
    if(y >= 0 && x >= 0 && y < grid_size.y && x < grid_size.x){
        if(click || (x != previous_toggled.x || y != previous_toggled.y)){
            grid[y][x].alive = !grid[y][x].alive;
            previous_toggled = Vector2{x, y};
        }
    }
}

void ConwaysGame::keyboard_input(){
    int key = GetKeyPressed();
    switch (key) {
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
        case 'I':
        case 'i':
            invert_grid();
            break;
        case 'S':
        case 's':
            cycle_schemes();
            break;
        case 'L':
        case 'l':
            loop_walls = !loop_walls;
            break;
        case 'H':
        case 'h':
            showing_help = !showing_help;
            break;
        case 'D':
        case 'd':
            reset_to_default();
            break;
        case 'B':
        case 'b':
            showing_grid_lines = !showing_grid_lines;
            break;
        case '+':
            fps += 2;
            SetTargetFPS(fps);
            break;
        case '-':
            fps -= 2;
            if(fps < 1)
                fps = 1;
            SetTargetFPS(fps);
            break;
        case '[':
            scale -= 1;
            if(scale <= 1)
                scale = 1;
            resize_grid();
            break;
        case ']':
            scale += 1;
            resize_grid();
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            scheme = static_cast<ColorScheme>(key - '1');
            break;
        default:
            break;
    }
}

void ConwaysGame::mouse_input(){
    if(IsKeyDown(KEY_LEFT_SHIFT)){
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            selection_start = Vector2{static_cast<int>(GetMouseX() / scale), static_cast<int>(GetMouseY() / scale)};
            showing_selection = true;
        }else if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            selection_end = Vector2{static_cast<int>(GetMouseX() / scale), static_cast<int>(GetMouseY() / scale)};
            showing_selection = true;
        }
    }else{
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            toggle_tile(GetMouseX(), GetMouseY(), true);
            showing_selection = false;
        }else if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            toggle_tile(GetMouseX(), GetMouseY(), false);
            showing_selection = false;
        }
    }
    int wheel_move = GetMouseWheelMove();
    if(wheel_move){
        scale += wheel_move;
        if(scale < 1)
            scale = 1;
        resize_grid();
    }
}

void ConwaysGame::cycle_schemes(){
    scheme = static_cast<ColorScheme>(scheme + 1);
    if(scheme ==  LOOP_TO_START){
        scheme = BlackAndWhite;
    }
}

void ConwaysGame::reset_to_default(){
    scale = DEFAULT_SCALE;
    fps = DEFAULT_FPS;
    scheme = DEFAULT_SCHEME;
    resize_grid();
    SetTargetFPS(fps);
}

Color ConwaysGame::get_color(int i, int j){
    switch (scheme) {
        case  BlackAndWhite:
            return BLACK;
            break;
        case Rainbow:
            return ColorFromHSV(Vector3{360 * (i / grid_size.y + j / grid_size.x) / 2 ,1, 1});
            break;
        case Red:
            return Color{255 * (i / grid_size.y + j / grid_size.x) / 2 ,0, 0, 255};
            break;
        case Green:
            return Color{0, 255 * (i / grid_size.y + j / grid_size.x) / 2, 0, 255};
            break;
        case Blue:
            return Color{0, 0, 255 * (i / grid_size.y + j / grid_size.x) / 2, 255};
            break;
        default:
            return BLACK;
    }
}

// public

ConwaysGame::ConwaysGame():ConwaysGame(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y, DEFAULT_SCALE, DEFAULT_FPS){
}

ConwaysGame::ConwaysGame(float scl):ConwaysGame(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y, scl, DEFAULT_FPS){
}

ConwaysGame::ConwaysGame(float x, float y):ConwaysGame(x, y, DEFAULT_SCALE, DEFAULT_FPS){
}

ConwaysGame::ConwaysGame(float x, float y, float scl):ConwaysGame(x, y, scl, DEFAULT_FPS){
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
        delete[] grid[i];
    }
    delete[] grid;
    grid = nullptr;
}

void ConwaysGame::run(){
    SetTraceLogLevel(LOG_NONE);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(window_size.x, window_size.y, "Conway's Game of Life");
    SetWindowMinSize(MINIMUM_WINDOW_SIZE.x, MINIMUM_WINDOW_SIZE.y);
    SetTargetFPS(fps);
    create_grid();
    while(!WindowShouldClose()){
        if(IsWindowResized()){
            window_size.x = GetScreenWidth();
            window_size.y = GetScreenHeight();
            resize_grid();
        }
        BeginDrawing();
        ClearBackground(WHITE);
        draw();
        if(!paused)
            step();
        else
            draw_paused();
        keyboard_input();
        mouse_input();
        EndDrawing();
    }
}
