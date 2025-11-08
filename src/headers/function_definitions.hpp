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
    MINEFIELD.clear(); MINEFIELD.shrink_to_fit(); // ENSURE A CLEAN SLATE OF THESE VARIABLES
    MINE_COORDS.clear(); MINE_COORDS.shrink_to_fit();
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
            grid_.isMine = true; // turn this into a mine !
            MINE_COORDS.emplace_back(y_, x_); break; // save coords n exit !
        }
    } return;
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
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void print_per_grid(WINPAN &win_, const int &y_, const int &x_, bool highlight) { // help display_minefield() to print characters according to each grid's state
    GRID &grid_ = MINEFIELD[y_][x_];
    int win_y = y_ + 1; // compute the real coordinates on the window
    int win_x = 3*x_ + 1;
    // determine color pririty
    if (highlight) {win_.set_style(9, 0);} // highlight with background
    else if (grid_.isFlagged) {win_.set_style(C_MAGENTA, A_BOLD);}
    else {win_.set_style(C_BLACK, A_BOLD);}

    if (grid_.isFlagged) {
        win_.wsprint(win_y, win_x, "[x]");
    } else if (grid_.isHidden) {
        win_.wsprint(win_y, win_x, " # ");
    } else if (grid_.adjacentMines == 0) {
        win_.wsprint(win_y, win_x, "   ");
    } else { // grid tells how many mines around it
        win_.wsprint(win_y, win_x, " " + std::to_string(grid_.adjacentMines) + " ");
    } return;
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void display_minefield(WINPAN &win_) { // display by calculated printing to a selected WINPAN
    for (int y_ = 0, x_; y_ < minefield_y; y_++) {
        for (x_ = 0; x_ < minefield_x; x_++) {
            print_per_grid(win_, y_, x_, false);
        }
    } return;
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void BFS_reveal(WINPAN &minesweeper_, const int&y_, const int &x_) {
    std::deque<std::pair<int, int>> search_these; // the QUEUE buffer
    // put the initial grid to search into 
    search_these.emplace_back(y_, x_); int i, check_y, check_x;
    while (!search_these.empty()) {
        // take the coordinates of the current grid being searched
        auto [this_y, this_x] = search_these.front();
        search_these.pop_front(); // pop this current grid out of the queue
        // search a 3x3 grid around each grid that is 0
        for (i = 0; i < 8; i++) {
            check_y = this_y + MATRIX_Y[i]; // calculate the coordinates of the 3x3 search
            if (check_y < 0 || check_y >= minefield_y) {continue;}
            check_x = this_x + MATRIX_X[i]; // condition checks to avoid accessing out of range
            if (check_x < 0 || check_x >= minefield_x) {continue;}

            // REFERENCE THE REAL GRID CURRENTLY BEING CHECKED
            GRID &grid_ = MINEFIELD[check_y][check_x];
            if (!grid_.isHidden) {continue;} // skip if this grid is already not hidden
            // if this grid has 0 adjacent mines, add it to the queue to 
            if (grid_.adjacentMines == 0) {search_these.emplace_back(check_y, check_x);}
            // reveal this grid, this as well prevents infinitely looping this queue
            grid_.isHidden = false;
            // display it on minefield
            print_per_grid(minesweeper_, check_y, check_x, false);
            update_panels(); doupdate();
            std::this_thread::sleep_for(std::chrono::microseconds(1000)); // aesthetic delay ...
        }
    } return;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DFS_reveal(WINPAN &minesweeper_, const int&y_, const int &x_) {
    std::deque<std::pair<int, int>> search_these; // the STACK buffer
    // put the initial grid to search into 
    search_these.emplace_front(y_, x_); int i, check_y, check_x;

    while (!search_these.empty()) {
        // take the coordinates of the current grid being searched
        auto [this_y, this_x] = search_these.front();
        search_these.pop_front(); // pop this current grid out of the stack
        // search a 3x3 grid around each grid that is 0
        for (i = 0; i < 8; i++) {
            check_y = this_y + MATRIX_Y[i]; // calculate the coordinates of the 3x3 search
            if (check_y < 0 || check_y >= minefield_y) {continue;}
            check_x = this_x + MATRIX_X[i]; // condition checks to avoid accessing out of range
            if (check_x < 0 || check_x >= minefield_x) {continue;}

            // REFERENCE THE REAL GRID CURRENTLY BEING CHECKED
            GRID &grid_ = MINEFIELD[check_y][check_x];
            if (!grid_.isHidden) {continue;} // skip if this grid is already not hidden
            // if this grid has 0 adjacent mines, add it to the stack to 
            if (grid_.adjacentMines == 0) {search_these.emplace_front(check_y, check_x);}
            // reveal this grid, this as well prevents infinitely looping this queue
            grid_.isHidden = false;
            // display it on minefield
            print_per_grid(minesweeper_, check_y, check_x, false);
            update_panels(); doupdate();
            std::this_thread::sleep_for(std::chrono::microseconds(1000)); // aesthetic delay ...
        }
    } return;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool player_won() {// check win condition
    for (const auto& mine_coord : MINE_COORDS) {
        GRID &grid_ = MINEFIELD[mine_coord.first][mine_coord.second];
        if (grid_.isMine && !grid_.isFlagged) {return false;} // if any mine weren't flagged, immidiately signal false win
    } return true;// if it were to reach here it means the player won !
}