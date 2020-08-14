#ifndef CONWAYS_GAME
#define CONWAYS_GAME

#include <raylib.h>
#include <string>
#include "Tile.h"

enum ColorScheme{
    BlackAndWhite,
    Rainbow,
    Red,
    Green,
    Blue,
    LOOP_TO_START
};

enum FileType{
    LoadFile,
    SaveFile,
    None
};

class ConwaysGame{
private:
    std::string ASSETS_PATH = "C:\\Users\\Shane\\Dropbox\\Desktop\\Coding\\C++\\ConwaysGame\\assets\\";
    static constexpr Vector2 DEFAULT_WINDOW_SIZE{1300, 660};
    static constexpr Vector2 MINIMUM_WINDOW_SIZE{600, 480};
    static constexpr float DEFAULT_SCALE = 10;
    static constexpr ColorScheme DEFAULT_SCHEME = BlackAndWhite;
    static const int DEFAULT_FPS = 30;
    ColorScheme scheme = DEFAULT_SCHEME;
    Vector2 previous_toggled{-1, -1};
    Vector2 window_size;
    Vector2 grid_size;
    Vector2 selection_start;
    Vector2 selection_end;
    Vector2 clipboard_size;
    FileType input_needed = None;
    Tile** grid;
    Tile** clipboard = nullptr;
    std::string input_string = "";
    float scale;
    bool paused = false;
    bool loop_walls = false;
    bool showing_help = true;
    bool showing_grid_lines = true;
    bool showing_selection = false;
    int fps;
    void create_grid();
    void destroy_grid();
    void resize_grid();
    void draw_grid_lines();
    void draw_grid();
    void draw_walls();
    void draw_help();
    void draw();
    void draw_paused();
    void draw_target_fps();
    void draw_selection();
    void draw_input_string();
    void copy_selection(bool cut);
    void cut_selection();
    void clear_selection();
    void flip_clipboard_horiz();
    void flip_clipboard_vert();
    void paste_clipboard();
    void destroy_clipboard();
    void randomize_grid();
    void clear_grid();
    void invert_grid();
    void step();
    void count_neighbors(int x, int y);
    void toggle_tile(int x, int y, bool click);
    void keyboard_input();
    void mouse_input();
    void cycle_schemes();
    void reset_to_default();
    void file_to_clipboard(std::string file_path);
    void clipboard_to_file(std::string file_path);
    void set_input_needed(FileType f);
    Color get_color(int i, int j);
    Vector2 get_selection_corner();
    Vector2 get_selection_size();
public:
    ConwaysGame();
    ConwaysGame(float scl);
    ConwaysGame(float x, float y);
    ConwaysGame(float x, float y, float scl);
    ConwaysGame(float x, float y, float scl, int f);
    ~ConwaysGame();
    void run();
};

#endif //CONWAYS_GAME
