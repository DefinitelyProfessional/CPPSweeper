// DEFINED STAGES OF THE GAME
#pragma once
#include "function_definitions.hpp"

// #1 FIRST WINDOW IS A WELCOME SCREEN !!!
void welcome_screen(void) {
    // START with getting the user used to keyboard input
    WINDOW* press_enter_scr = newwin(10, 30, 0, 0); box(press_enter_scr,0,0);
    mvwprintw(press_enter_scr, 4, 2, "Press any key to continue.");
    wrefresh(press_enter_scr); wgetch(press_enter_scr); delwin(press_enter_scr);
    // game intro screen
    WINDOW* wel_scr = newwin(10,30,0,0); box(wel_scr, 0, 0);
    mvwprintw(wel_scr, 1, 1, "#######1____=__=____1#######");
    mvwprintw(wel_scr, 2, 1, "###1                    1###");
    mvwprintw(wel_scr, 3, 1, "#1   << MINESWEEPER >>    1#");
    mvwprintw(wel_scr, 5, 1, "Made with C++ with a pint of");
    mvwprintw(wel_scr, 6, 1, "  blood, sweat, and tears.  ");
    mvwprintw(wel_scr, 8, 1, "(powered by pain and agony.)");
    // display the screen, get input to continue, del and exit
    wrefresh(wel_scr); wgetch(press_enter_scr); delwin(wel_scr); return;
}
// HELPER FUNCTION FOR THE prompt_window_setup()
bool confirm_preview(const int &l, const int &w) {
    WINDOW* preview_scr = newwin(l, w, 0, 0); box(preview_scr, 0, 0);
    PANEL* preview_panel = new_panel(preview_scr);
    mvwprintw(preview_scr, 1, 1, "This will be the size of the minefield");
    mvwprintw(preview_scr, 2, 1, "ENTER to CONFIRM | any key to reconfigure");
    update_panels(); doupdate();
    
    int ch = wgetch(preview_scr); // prompt for input
    hide_panel(preview_panel);
    delwin(preview_scr);// delete this preview before return and exit
    return (ch == '\n' || ch == '\r' || ch == KEY_ENTER) ? true : false;
}
// #2 PROMPT THE WIDTH AND LENGTH OF THE WINDOW
void prompt_minefield_assignment(void) {
    WINDOW* prompt_scr = newwin(LINES - 1, COLS - 1, 0, 0); box(prompt_scr, 0, 0);
    PANEL* prompt_panel = new_panel(prompt_scr);
    mvwprintw(prompt_scr, LINES/2-2, 1, "<<< MINE FIELD  AREA ASSIGNMENT >>>");
    mvwprintw(prompt_scr, LINES/2-1, 1, "How much do you want to work with ?");
    update_panels(); doupdate();
    int width, length;
    while (true) {
        width = prompt_int(prompt_scr, LINES/2+1, 1, "Enter width : ", 20, 100) * 3;
        length = prompt_int(prompt_scr, LINES/2+2, 1, "Enter length : ", 20, 100);
        if (confirm_preview(length, width)) {delwin(prompt_scr); return;}
    }
}
