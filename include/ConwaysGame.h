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

enum InputNeeded{
    LoadFile,
    SaveFile,
    GridCellSize,
    None
};

class ConwaysGame{
private:
    std::string ASSETS_PATH = "/Users/shane/coding/c++/ConwaysGameOfLifeCpp/assets/";
    static constexpr Vector2 DEFAULT_WINDOW_SIZE{1440, 760};
    static constexpr Vector2 MINIMUM_WINDOW_SIZE{720, 380};
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
    Vector2 prev_mouse_pos{0, 0};
    InputNeeded input_needed = None;
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
    void draw_asset_files(float box_bottom_y);
    void draw_text_in_rec(std::string text, Rectangle rec, int line_padding);
    void copy_selection(bool cut);
    void cut_selection();
    void clear_selection();
    void invert_selection();
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
    void set_input_needed(InputNeeded input);
    void toggle_line_between(Vector2 a, Vector2 b);
    void toggle_column(int x);
    void toggle_row(int y);
    void set_grid(int cell_size);
    float distance(Vector2 a, Vector2 b);
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
