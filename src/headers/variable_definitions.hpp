// GLOBAL VARIABLE DEFINITIONS ARE HERE !!!

#pragma once

#include <string>
#include <vector>
#include <cctype>

struct CELL {
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int adjacentMines = 0;
};
// custom data structure to coordinate PDCurses based display
struct WINPAN { // aka WindowPanel
    WINDOW* window_; PANEL* panel_;
    int line_height, coll_width;
    // initializer aka CONSTRUCTOR
    WINPAN(const int &h, const int &w, const int &y, const int &x) {
        window_ = newwin(h, w, y, x);
        panel_ = new_panel(window_);
        line_height = h; coll_width = w;}
    // deleter aka DECONSTRUCTOR
    ~WINPAN() {hide_panel(panel_); del_panel(panel_); delwin(window_);}


    // print stuff into this WindowPanel
    void wprint(const int &line_, const int &coll_, const std::string &message) {
        mvwprintw(window_, line_, coll_, "%s", message.c_str());}
    // clear the window's internal buffer
    void wclear() {werase(window_);}
    // quick show hide functions for this WindowPanel
    void show() {show_panel(panel_);}
    void hide() {hide_panel(panel_);}
};
std::vector<std::vector<CELL>> mine_field;