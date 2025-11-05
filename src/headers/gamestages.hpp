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
            // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CALCULATE the GLOBAL VARIABLE mine_count
            // ((double)(difficulty*5 + 20)/100.0)) is a percentage from 25% to 70% of the minefield will be mines !!!
            mine_count = (int)((double)(minefield_y*minefield_x) * ((double)(difficulty*5 + 20)/100.0));
            return; // exit this function
        }
    }
}

void generate_minefield() {
    // INITIALIZE THE DIMENTIONS OF THE MINEFIELD TO BE minefield_y rows and minefield_x collumns
    MINEFIELD = std::vector<std::vector<GRID>>(minefield_y, std::vector<GRID>(minefield_x));
    for(int y = 0; y < minefield_y; y++){
        for(int x = 0; x < minefield_x; x++){
            // MINEFIELD[y][x];
        }
    }
}
// #3 LET THE GAMES BEGIN AHH FUNCTION
void game_starts() {
    int real_minefield_width = minefield_x*3; // because later each grid is printed as 3 chars fixed

    return;
}


// for (std::size_t y = 0; y < MINEFIELD.size(); ++y) {
//     for (std::size_t x = 0; x < MINEFIELD[y].size(); ++x) {
//         GRID& cell = MINEFIELD[y][x];
//         // You can now read or modify cell properties
//         cell.isMine = true; // example
//     }
// }
// for (auto& row : MINEFIELD) {
//     for (auto& cell : row) {
//         cell.isRevealed = false; // example
//     }
// }
// #include <random>
// // Create a random engine and distribution
// std::random_device rd;  // Seed
// std::mt19937 gen(rd()); // Mersenne Twister engine
// std::uniform_int_distribution<> dist(0, 100); // Range: 0 to 100