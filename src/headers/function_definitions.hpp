// FUNCTION DEFINITIONS ARE HERE !!!

#pragma once // header files good practice
#include "variable_definitions.hpp"

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// HELPER FUNCTION FOR prompt_int()
void display_prompt(WINPAN &win_, const int &row_, const int &col_, const std::string& message){
    win_.wprint(row_, col_, message + "   "); // print the message prompt
    int cursor_x = col_ + message.size(); // calculate where the cursor starts
    win_.move_cursor(row_, cursor_x); // position the cursor at the end of the message
    update_panels(); doupdate(); // update and flush to screen
}
// PROMPT INTEGER INPUTS
int prompt_int(WINPAN &win_, const int &row_, const int &col_,
const std::string& message, const int &low_, const int &high_) {
    std::string input_; // this is the buffer to receive inputs
    constexpr int MAX_DIGITS = 3;

    display_prompt(win_, row_, col_, message);
    // keypad(win_.window_, TRUE);// Ensure keypad for special keys and immediate input

    int ch, message_length = message.size(); 
    // THIS IS THE JANKY SINGLE CHARACTER BY CHARACTER INPUT
    while (true) {
        ch = win_.input(); // This is what prompts for single key press inputs, blocking.

        // ENTER aka CONFIRM action
        if (ch == '\n' || ch == '\r' || ch == KEY_ENTER) {
            if (input_.empty()) {continue;}
            // THIS IS THE SUCCESSFUL ACCEPTABLE INPUT CASE
            try {
                int i = std::stoi(input_); // will trigger an error if input_ is unconvertible..
                if (low_ <= i && i <= high_) {return i;} // return only if its within specified range
                else {throw std::__throw_out_of_range;} // trigger the catch to reuse its block.
            }
            // failure to convert will be met with re-prompt
            catch (...) {input_ = ""; display_prompt(win_, row_, col_, message); continue;}
        }
        // HANDLE BACKSPACE DELETES
        else if ((ch == KEY_BACKSPACE || ch == 127 || ch == '\b') && !input_.empty()) {
            input_.pop_back();
            // redraw last char space and move cursor
            mvwaddch(win_.window_, row_, col_ + message_length + input_.size(), ' ');
            win_.move_cursor(row_, col_ + message_length + input_.size());
        }
        // HANDLE ACCEPTABLE CHARACTERS
        else if (std::isdigit(ch) && input_.size() < MAX_DIGITS) {
            input_.push_back((char)ch);
            // print the new char and advance cursor
            win_.wprint(row_, col_ + message_length, input_);
            win_.move_cursor(row_, col_ + message_length + input_.size());
        }
        update_panels(); doupdate(); // finally, update and flush to screen
    }
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void generate_minefield() { // use random seed and uniform distribution to generate coordinates of the mines.
    // INITIALIZE THE DIMENTIONS OF THE MINEFIELD TO BE minefield_y rows and minefield_x collumns
    MINEFIELD = std::vector<std::vector<GRID>>(minefield_y, std::vector<GRID>(minefield_x));
    std::random_device randseed_; // create the nondeterministic seed
    std::mt19937 generate_(randseed_()); // mersene twister creates pseudorandom numbers from randseed_
    std::uniform_int_distribution<> rand_y(0, minefield_y-1); // defines a uniform distribution from a range
    std::uniform_int_distribution<> rand_x(0, minefield_x-1); // different for each x and y coordinate
    // GENERATE THE COORDINATES OF MINES
    for (int i = 0, x_, y_; i < mine_count; i++) {
        while (true) {
            y_ = rand_y(generate_);
            x_ = rand_x(generate_);
            GRID &grid_ = MINEFIELD[y_][x_];
            if (grid_.isMine) {continue;} // regenerate if its already turned into a mine
            grid_.isMine = true; break; // turn into a mine then exit while loop !
        }
    } return;
    // std::cout << "SUCCESSFUL MINEFIELD GENERATION" << std::endl; return;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void compute_minefield(){ // compute how many adjacent mines in each minefield grid
    int y_, x_, i, check_y, check_x, mines;
    // CALCULATE ADJACENT MINES
    for (y_ = 0; y_ < minefield_y; y_++) {
        for (x_ = 0; x_ < minefield_x; x_++) {
            GRID &grid_ = MINEFIELD[y_][x_];
            if (grid_.isMine) {continue;} // skip if its a mine which we dont need to check...
            // search the 3x3 space around
            mines = 0;
            for (i = 0; i < 8; i++) {
                check_y = y_ + MATRIX_Y[i]; // calculate the coordinates of the 3x3 search
                if (check_y < 0 || check_y >= minefield_y) {continue;}
                check_x = x_ + MATRIX_X[i]; // condition checks to avoid accessing out of range
                if (check_x < 0 || check_x >= minefield_x) {continue;}
                // count the mines if it found a grid being mines !
                if (MINEFIELD[check_y][check_x].isMine) {mines++;}
            }
            // assign the number of adjacent mines found in this coordinate
            grid_.adjacentMines = mines;
        }
    } return;
    // std::cout << "SUCCESSFUL COMPUTE MINEFIELD !!!" << std::endl; return;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
std::string print_grid(GRID &grid_) { // help display_minefield() to print characters according to each grid's state
    if (grid_.isHidden) {return " # ";}
    else if (grid_.isFlagged) {return "!M!";}
    else if (grid_.adjacentMines == 0) {return "   ";}
    else {return " " + std::to_string(grid_.adjacentMines) + " ";}
    // return // sexy ternary operators I love to use..
    // (grid_.isHidden) ? ".#`" :
    // (grid_.isFlagged) ? "!M!" :
    // (grid_.adjacentMines == 0) ? "   " :
    // " " + std::to_string(grid_.adjacentMines) + " ";
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void display_minefield(WINPAN &win_) { // display by calculated printing to a selected WINPAN
    int y_, x_, win_y, win_x;
    for (y_ = 0; y_ < minefield_y; y_++) {
        for (x_ = 0; x_ < minefield_x; x_++) {
            // calculate the cursor start positions for each print to win_
            win_y = y_ + 1;
            win_x = 3*x_ + 1;
            win_.wprint(win_y, win_x, print_grid(MINEFIELD[y_][x_]));
        }
    } return;
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