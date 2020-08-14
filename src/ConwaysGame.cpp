#include <time.h>
#include <math.h>
#include <fstream>
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

void ConwaysGame::destroy_grid(){
    for(int i = 0; i < grid_size.y; i++){
        delete[] grid[i];
    }
    delete[] grid;
    grid = nullptr;
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
    destroy_grid();
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
    selection_start.x = std::min(selection_start.x, grid_size.x - 1);
    selection_start.y = std::min(selection_start.y, grid_size.y - 1);
    selection_end.x = std::min(selection_end.x, grid_size.x - 1);
    selection_end.y = std::min(selection_end.y, grid_size.y - 1);
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
    const int arr_size = 21;
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
        "1, 2, 3, 4, 5 - Select color Scheme",
        "Shift click drag - select",
        "Shift X - Cut",
        "Shift C - Copy",
        "Shift V - Paste",
        "Shift S - save clipboard to",
        "Shift L - load clipboard from file"
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
    if(showing_selection)
        draw_selection();
    if(!loop_walls)
        draw_walls();
    if(input_needed != None)
        draw_input_string();
    else if(showing_help)
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
    DrawRectangleV(start, Vector2{scale, scale}, Color{0, 0, 255, 80});
    DrawRectangleV(end, Vector2{scale, scale}, Color{0, 0, 255, 80});
    Vector2 corner = get_selection_corner();
    corner.x *= scale;
    corner.y *= scale;
    Vector2 size = get_selection_size();
    size.x *= scale;
    size.y *= scale;
    DrawRectangleV(corner, size, Color{0, 255, 0, 120});
}

void ConwaysGame::draw_input_string(){
    int font_size = 90;
    int padding = 10;
    int height = (window_size.y - font_size + padding) / 2;
    int input_width;
    int header_text_font_size = 30;
    std::string header_text = "Enter the name of a file to ";
    header_text += (input_needed == LoadFile ? "load" : "save");
    while(1){
        input_width = MeasureText(input_string.c_str(), font_size);
        if(input_width > window_size.x){
            font_size--;
            if(font_size == 1)
                break;
        }else
            break;
    }
    DrawText(header_text.c_str(), (window_size.x - MeasureText(header_text.c_str(), header_text_font_size)) / 2, height - header_text_font_size, header_text_font_size, Color{20, 20, 20, 255});
    DrawRectangle(0, height, window_size.x, font_size + padding, Color{0, 0, 0, 200});
    DrawText(input_string.c_str(), (window_size.x - input_width) / 2, height + padding / 2, font_size, WHITE);
}

void ConwaysGame::copy_selection(bool cut = false){
    if(!showing_selection)
        return;
    destroy_clipboard();
    clipboard_size = get_selection_size();
    Vector2 corner = get_selection_corner();
    clipboard = new Tile*[static_cast<unsigned>(clipboard_size.y)];
    for(int i = 0; i < clipboard_size.y; i++){
        clipboard[i] = new Tile[static_cast<unsigned>(clipboard_size.x)];
        for(int j = 0; j < clipboard_size.x; j++){
            clipboard[i][j] = grid[static_cast<unsigned>(corner.y) + i][static_cast<unsigned>(corner.x) + j];
            if(cut)
                grid[static_cast<unsigned>(corner.y) + i][static_cast<unsigned>(corner.x) + j].alive = false;
        }
    }
}

void ConwaysGame::cut_selection(){
    copy_selection(true);
}

void ConwaysGame::flip_selection_horiz(){
    for(int i = 0; i < clipboard_size.y; i++)
        for(int j = 0; j < clipboard_size.x / 2; j++){
            Tile temp;
            temp = clipboard[i][j];
            clipboard[i][j] = clipboard[i][static_cast<int>(clipboard_size.x - 1) - j];
            clipboard[i][static_cast<int>(clipboard_size.x - 1) - j] = temp;
        }
}

void ConwaysGame::paste_clipboard(){
    if(clipboard != nullptr){
        showing_selection = false;
        int mx = GetMouseX() / scale;
        int my = GetMouseY() / scale;
        for(int i = 0; i < clipboard_size.y; i++)
            for(int j = 0; j < clipboard_size.x; j++)
                if(my + i >= 0 && mx + j >= 0 && my + i < grid_size.y && mx + j < grid_size.x)
                    grid[my + i][mx + j] = clipboard[i][j];
    }
}

void ConwaysGame::destroy_clipboard(){
    if(clipboard != nullptr){
        for(int i = 0; i < clipboard_size.y; i++)
            delete[] clipboard[i];
        delete[] clipboard;
    }
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
    if(input_needed != None){
        if(IsKeyPressed(KEY_ENTER)){
            if(input_needed == SaveFile){
                clipboard_to_file(ASSETS_PATH + input_string);
            }else{
                file_to_clipboard(ASSETS_PATH + input_string);
            }
            set_input_needed(None);
            input_string = "";
        }else if(IsKeyPressed(KEY_ESCAPE)){
            set_input_needed(None);
            input_string = "";
        }else if(IsKeyPressed(KEY_BACKSPACE)){
            if(input_string != "")
                input_string.pop_back();
        }else if(key){
            input_string += key;
        }
    }else if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
        switch(key){
            case 'X':
            case 'x':
                cut_selection();
                break;
            case 'C':
            case 'c':
                copy_selection();
                break;
            case 'V':
            case 'v':
                paste_clipboard();
                break;
            case 'L':
            case 'l':
                set_input_needed(LoadFile);
                SetExitKey(0);
                break;
            case 'S':
            case 's':
                set_input_needed(SaveFile);
                SetExitKey(0);
                break;
            case 'F':
            case 'f':
                flip_selection_horiz();
                break;
            case '+':
                fps += 2;
                SetTargetFPS(fps);
                break;
            default:
                break;
        }
    }else{
        switch(key){
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
}

void ConwaysGame::mouse_input(){
    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            selection_start = Vector2{static_cast<int>(GetMouseX() / scale), static_cast<int>(GetMouseY() / scale)};
            if(selection_start.x < 0)
                selection_start.x = 0;
            else if(selection_start.x >= grid_size.x)
                selection_start.x = grid_size.x - 1;
            if(selection_start.y < 0)
                selection_start.y = 0;
            else if(selection_start.y >= grid_size.y)
                selection_start.y = grid_size.y - 1;
            showing_selection = true;
        }else if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            selection_end = Vector2{static_cast<int>(GetMouseX() / scale), static_cast<int>(GetMouseY() / scale)};
            if(selection_end.x < 0)
                selection_end.x = 0;
            else if(selection_end.x >= grid_size.x)
                selection_end.x = grid_size.x - 1;
            if(selection_end.y < 0)
                selection_end.y = 0;
            else if(selection_end.y >= grid_size.y)
                selection_end.y = grid_size.y - 1;
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

void ConwaysGame::file_to_clipboard(std::string file_path){
    std::string line;
    std::ifstream f(file_path);
    int lines = 0;
    int line_width;
    while(getline(f, line)){
        lines++;
        line_width = line.size();
    }
    f.clear();
    f.seekg(0);
    destroy_clipboard();
    clipboard_size.x = line_width;
    clipboard_size.y = lines;
    clipboard = new Tile*[lines];
    for(int i = 0; i < lines; i++){
        getline(f, line);
        clipboard[i] = new Tile[line_width];
        for(int j = 0; j < line_width; j++){
            switch(line[j]){
                case 'x':
                    clipboard[i][j] = Tile{true, 0};
                    break;
                default:
                    clipboard[i][j] = Tile{false, 0};
            }
        }
    }
    f.close();
}
void ConwaysGame::clipboard_to_file(std::string file_path){
    if(clipboard == nullptr)
        return;
    std::ofstream f(file_path);
    for(int i = 0; i < clipboard_size.y; i++){
        for(int j = 0; j < clipboard_size.x; j++){
            if(clipboard[i][j].alive)
                f << 'x';
            else
                f << '.';
        }
        f << '\n';
    }
    f.close();
}

void ConwaysGame::set_input_needed(FileType f){
    input_needed = f;
    SetExitKey(f == None ? KEY_ESCAPE : 0);
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

Vector2 ConwaysGame::get_selection_corner(){
    return Vector2{
        std::min(selection_start.x, selection_end.x),
        std::min(selection_start.y, selection_end.y)
    };
}

Vector2 ConwaysGame::get_selection_size(){
    return Vector2{
        abs(selection_end.x - selection_start.x) + 1,
        abs(selection_end.y - selection_start.y) + 1
    };
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
    destroy_grid();
    destroy_clipboard();
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
