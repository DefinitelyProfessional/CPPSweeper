// DEFINED STAGES OF THE GAME
#pragma once
#include "function_definitions.hpp"

// #1 FIRST WINDOW IS A WELCOME SCREEN !!!
void welcome_screen() {
    // START with getting the user used to keyboard input
    WINPAN press_enter_scr(10, 30, 0, 0, true);
    press_enter_scr.wprint(4, 2, "Press any key to continue.");
    update_panels(); doupdate();
    press_enter_scr.input(); // WAIT FOR ENTER
    press_enter_scr.hide();
    // game intro screen
    WINPAN wel_scr(10, 30, 0, 0, true);
    wel_scr.set_style(C_ORANGE, A_BOLD); wel_scr.apply_style();
    wel_scr.wprint( 1, 1, "#######1____=__=____1#######");
    wel_scr.wprint( 2, 1, "###1                    1###");
    wel_scr.wprint( 3, 1, "#1   << MINESWEEPER >>    1#");
    wel_scr.wprint( 5, 1, "Made with C++ with a pint of");
    wel_scr.wprint( 6, 1, "  blood, sweat, and tears.  ");
    wel_scr.clear_style();
    wel_scr.set_style(C_GOLD, A_BOLD);
    wel_scr.apply_style();
    wel_scr.wprint( 8, 1, "(powered by pain and agony.)");
    update_panels(); doupdate();
    // press enter to continue then exit
    wel_scr.input(); return;
}


// HELPER FUNCTION FOR THE prompt_window_setup()
bool confirm_preview() {
    WINPAN preview_scr(minefield_y, minefield_x * 3, 0, 0, true); // * 3 because the minefields are gonna be printed with 3 chars each
    preview_scr.set_style(C_RED, A_BOLD);
    preview_scr.wsprint(minefield_y/2-1, minefield_x*3/2-19, "This will be the size of the minefield");
    preview_scr.wsprint(minefield_y/2+1, minefield_x*3/2-19, "ENTER to CONFIRM. or any key to cancel");
    update_panels(); doupdate();
    int ch = preview_scr.input(); // prompt for input
    return (ch == '\n' || ch == '\r' || ch == KEY_ENTER) ? true : false;
}


// #2 PROMPT THE WIDTH AND LENGTH OF THE WINDOW
void prompt_minefield_assignment() {
    WINPAN prompt_scr(LINES - 1, COLS - 1, 0, 0, true);
    prompt_scr.set_style(C_GOLD, A_BOLD);
    prompt_scr.wsprint(LINES/2-2, 1, "<<< MINE FIELD  AREA ASSIGNMENT >>>");
    prompt_scr.wprint(LINES/2-1, 1, "How much do you want to work with ?");
    prompt_scr.wprint(LINES/2+5, 1, "Choose between 15 to 50");
    update_panels(); doupdate();
    int width, length, difficulty;
    while (true) { // prompt until a confirmed valid length and width
        width = prompt_int(prompt_scr, LINES/2+1, 1,  "Enter width  : ", 15, 50);
        length = prompt_int(prompt_scr, LINES/2+2, 1, "Enter length : ", 15, 50);
         // ensure y vertical is less than x horizontal >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> THIS SETS THE GLOBAL VARIABLE minefield_y _x
        minefield_y = std::min(width, length);
        minefield_x = std::max(width, length);
        if (confirm_preview()) {
            // PROCEED TO PROMPT PLAYER TO CHOOSE DIFFICULTY
            prompt_scr.wsprint(LINES/2+5, 1, "Choose a rate from 1 to 10");
            update_panels(); doupdate();
            prompt_scr.set_style(C_MAGENTA, A_BOLD); prompt_scr.apply_style();
            difficulty = prompt_int(prompt_scr, LINES/2+3, 1, "Difficulty : ", 1, 10);
            // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CALCULATE the GLOBAL VARIABLE mine_count AND flagged_mines
            // ((double)(difficulty*5 + 20)/100.0)) is a percentage from 7% to 25% of the minefield will be mines !!!
            mine_count = (int)((double)(minefield_y*minefield_x) * ((double)(difficulty*2 + 5)/100.0));
            flagged_mines = 0; // set the flagged to 0 at start
            return; // exit this function
        }
    }
}

// handle game over
void game_over(WINPAN &minesweeper_, WINPAN &side_scr_, const int &y_hit, const int &x_hit) {
    // show the mines
    minesweeper_.set_style(C_HIGHLIGHT, A_BOLD);
    for (int y_ = 0, x_; y_ < minefield_y; y_++) {
        for (x_ = 0; x_ < minefield_x; x_++) {
            if (MINEFIELD[y_][x_].isMine) {
                int win_y = y_ + 1; // compute the real coordinates on the window
                int win_x = 3*x_ + 1;
                minesweeper_.wsprint(win_y, win_x, "(M)");
            }
        }
    }
    // color the grid where the player hit the mine
    minesweeper_.set_style(C_HIT, A_BOLD);
    minesweeper_.wsprint(y_hit + 1, 3*x_hit + 1, "(M)");
    // clear the side_scr window and print the game over message
    side_scr_.wclear(); std::string x = "( " + std::to_string(x_hit) + "," + std::to_string(y_hit) + " )";
    side_scr_.set_style(C_MAGENTA, A_BOLD); side_scr_.wsprint(1, 1, "     ... WHAT HAVE YOU DONE ...     "); 
    update_panels(); doupdate(); minesweeper_.input();
    side_scr_.set_style(C_RED, A_BOLD); side_scr_.wsprint(2, 1, "  You hit a mine at " + x + "       ");
    update_panels(); doupdate(); minesweeper_.input();
    side_scr_.set_style(C_GOLD, A_BOLD); side_scr_.wsprint(3, 1, "  'violently explodes into pieces'  ");
    update_panels(); doupdate(); minesweeper_.input();
    return;
}

// display the win screen
void win_screen() {
    WINPAN winners_scr(LINES/3, COLS/2, LINES/2-LINES/6, COLS/2-COLS/4,  false);
    winners_scr.draw_border();
    winners_scr.set_style(C_RED, A_BOLD);
    winners_scr.wsprint(winners_scr.line_height/2-1, 1, "< CONGRATULATIONS !!! YOU WON !!! >");
    winners_scr.wsprint(winners_scr.line_height/2, 1, "~ THE MINEFIELD IS NOW MINE FREE! ~");
    winners_scr.wsprint(winners_scr.line_height/2+1, 1, "You shall be rewarded in prayer. God Bless.");
    update_panels(); doupdate(); winners_scr.input(); return;
}

// ask if player wants to replay
bool prompt_replay() {
    WINPAN replay_scr(LINES/3, COLS/2, LINES/2-LINES/6, COLS/2-COLS/4,  true);
    replay_scr.set_style(C_MAGENTA, A_BOLD);
    replay_scr.wsprint(replay_scr.line_height/2, 1, "In for another round ?");
    replay_scr.wprint(replay_scr.line_height/2+1, 1, "ENTER to confirm or any key to cancel");
    update_panels(); doupdate;
    int ch = replay_scr.input();
    if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {return true;}
    else {return false;}
}