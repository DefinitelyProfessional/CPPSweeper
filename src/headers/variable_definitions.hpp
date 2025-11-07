// GLOBAL VARIABLE DEFINITIONS ARE HERE !!!

#pragma once

#define PDC_STATIC // somewhat important for include curses.h
#define PDC_RGB // somewhat important as well
#include <curses.h>
#include <panel.h>

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <random>
#include <chrono>
#include <thread>

// custom data structure to coordinate PDCurses based display
struct WINPAN { // aka WindowPanel
    WINDOW *window_; PANEL *panel_;
    int line_height, coll_width; // looks like this havent found a use...
    int text_attr = A_NORMAL; // font style attribute
    short color_pair_id = 0; // color pair id
    // initializer aka CONSTRUCTOR
    WINPAN(const int &h, const int &w, const int &y, const int &x, bool box_) {
        window_ = newwin(h, w, y, x); // create its window
        panel_ = new_panel(window_); // create its panel
        if (box_) {box(window_, ACS_VLINE, ACS_HLINE);} // create simple box
        line_height = h; coll_width = w;}
    // deleter aka DECONSTRUCTOR
    ~WINPAN() {hide_panel(panel_); del_panel(panel_); delwin(window_);}
    
    // quick show hide functions for this WindowPanel
    void show() {show_panel(panel_);}
    void hide() {hide_panel(panel_);}
    // get input from this WindowPanel
    int input() {return wgetch(window_);}
        
    // set the STYLE of this WindowPanel
    void set_style(const short &color_id, const int &attr_flags) {
        color_pair_id = color_id;
        text_attr = attr_flags;}

    // apply the set styles
    void apply_style() {wattron(window_, COLOR_PAIR(color_pair_id) | text_attr);}
    
    // clear the set styles
    void clear_style() {wattroff(window_, COLOR_PAIR(color_pair_id) | text_attr);}
    
    // print into this window
    void wprint(const int &row_, const int &col_, const std::string &message) {
        mvwprintw(window_, row_, col_, "%s", message.c_str());}

        // move cursor within this window
    void move_cursor(const int &row_, const int &col_) {wmove(window_, row_, col_);}
    
    // print into this window with s t y l e
    void wsprint(const int &row_, const int &col_, const std::string &message) {
        apply_style(); mvwprintw(window_, row_, col_, "%s", message.c_str()); clear_style();}
        
    void draw_border( // UNUSED
            chtype ls = '|', chtype rs = '|',
        chtype ts = '-', chtype bs = '-',
        chtype tl = '+', chtype tr = '+',
        chtype bl = '+', chtype br = '+') {
        apply_style(); wborder(window_, ls, rs, ts, bs, tl, tr, bl, br); clear_style();
    }
    // clear the window's internal buffer
    void wclear() {werase(window_);}
};



// <<<<<<<<<<<<<<<<<<<<<<<<<< GAMEPLAY VARIABLES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// variables defined later by prompt_minefield_assignment()
int minefield_y, minefield_x, mine_count, flagged_mines = 0;

struct GRID {
    bool isMine = false;
    bool isHidden = true;
    bool isFlagged = false;
    int adjacentMines = 0;
};
std::vector<std::vector<GRID>> MINEFIELD;
// TO MAKE A FAST EFFICIENT 3X3 SEARCH OF EACH MINEFIELD GRID
const int MATRIX_Y[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
const int MATRIX_X[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
// TO SAVE THE COORDS OF THE MINES TO SPEED UP WIN CHECKS
std::vector<std::pair<int, int>> MINE_COORDS;

// THE AMAZING CURSOR FOR SELECTING MINES
void print_per_grid(WINPAN &, const int &, const int &, bool); // prototype that gets used by GRIDCURSOR objects
void DFS_reveal(WINPAN &, const int&, const int &); // prototype for GRIDCURSOR as well to reveal grids with DFS
void BFS_reveal(WINPAN &, const int&, const int &); // prototype for GRIDCURSOR as well to reveal grids with BFS
struct GRIDCURSOR {
    int y_cursor = 0, x_cursor = 0;
    int y_prev, x_prev, y_max, x_max;
    WINPAN &target_win;
    // the peculiar constructor ... initialize variables before constructor body executes
    GRIDCURSOR(const int &MAX_Y, const int &MAX_X, WINPAN &win_) :
        y_max(MAX_Y), x_max(MAX_X), target_win(win_) {} // {} is the constructor body
    
    // move the cursor on window
    void move(const int &y_, const int &x_) {
        // save the previous cursor coords
        x_prev = x_cursor; y_prev = y_cursor;
        // move the cursor with wrapping 
        y_cursor = (y_prev + y_) % y_max; // top wrap back to bottom
        if (y_cursor < 0) {y_cursor += y_max;} // bottom wrap back to top
        x_cursor = (x_prev + x_) % x_max; // right wrap back to left
        if (x_cursor < 0) {x_cursor += x_max;} // left wrap back to right

        // undo the colors for the previous highlighted grid
        print_per_grid(target_win, y_prev, x_prev, false);

        // color the grid highlighted by this cursor
        print_per_grid(target_win, y_cursor, x_cursor, true);
    }

    // flag grids and  display them as flagged grids
    bool flag_this() {
        GRID &grid_ = MINEFIELD[y_cursor][x_cursor]; // reference the real grid
        // TOGGLE OFF IF ITS ALREADY FLAGGED
        if (grid_.isFlagged) {
            grid_.isFlagged = false; // this toggles off
            flagged_mines--; // decrease the flagged mines
            print_per_grid(target_win, y_cursor, x_cursor, true);
        // TOGGLE ON ONLY IF ITS NOT FLAGGED AND STILL HAS FLAGS TO GIVE
        } else if (flagged_mines <= mine_count) {
            grid_.isFlagged = true;
            flagged_mines++;
            print_per_grid(target_win, y_cursor, x_cursor, false);
        } else {return false;} // false indicates out of flags
        return true; // reach here if all is fine
    }

    // handle ENTER action to DIG for MINES
    bool dig_this() {
        GRID &grid_ = MINEFIELD[y_cursor][x_cursor]; // reference the real grid
        if (grid_.isMine) {return true;} // immidiately signal game over
        else if (!grid_.isHidden) {return false;} // return if its already not hidden
        else if (grid_.adjacentMines != 0) { // simply show the grids that are not 0
            grid_.isHidden = false;
            print_per_grid(target_win, y_cursor, x_cursor, true);}
        // this is the case when the grid revealed is 0 and requires DFS to reveal surrounding grids
        else {
            grid_.isHidden = false;
            // DFS_reveal(target_win, y_cursor, x_cursor); // initiate BFS reveal
            BFS_reveal(target_win, y_cursor, x_cursor); // initiate BFS reveal
            move(0, 0); // return the cursor to highlight the current position
        } return false;
    }
};



// color definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> THESE ARE GAME INITIALIZATION
constexpr short C_BLACK = 0, C_WHITE = 1, C_RED = 2, C_ORANGE = 3, C_GOLD = 4, C_MAGENTA = 5, C_HIGHLIGHT = 6, C_HIT = 7;
// a function called from int main(void) to init and build PDCurses environment
void initialize_cursed_environment() {
    initscr();
    start_color();
    keypad(stdscr, TRUE); // enable reading special keys
    refresh(); // Ensure stdscr is drawn
    cbreak(); // turns off line buffering, any key press went straight into program input
    noecho(); // disable echoing typed characters to the screen -- full control of output
    
    // color initializations NOTE: THIS METHOD IS RATHER UNRELIABLE AND UNPREDICTABLE BRUH
    if (can_change_color()){
        init_color(C_BLACK, 0, 0, 0);
        init_color(C_WHITE, 1000, 1000, 1000);
        init_color(C_RED, 862, 78, 235);   // bright_crimson_red
        init_color(C_ORANGE, 1000, 647, 0);   // orange
        init_color(C_GOLD, 1000, 843, 0);   // gold
        init_color(C_MAGENTA, 1000, 0, 1000);  // magenta

        // init_pair(target, foreground, background)
        init_pair(C_BLACK, C_BLACK, C_WHITE);
        init_pair(C_WHITE, C_WHITE, C_BLACK);
        init_pair(C_RED, C_RED, C_BLACK);
        init_pair(C_ORANGE, C_ORANGE, C_BLACK);
        init_pair(C_GOLD, C_GOLD, C_BLACK);
        init_pair(C_MAGENTA, C_MAGENTA, C_BLACK);
        init_pair(C_HIGHLIGHT, C_BLACK, C_GOLD);
        init_pair(C_HIT, C_GOLD, C_RED);
    }
}