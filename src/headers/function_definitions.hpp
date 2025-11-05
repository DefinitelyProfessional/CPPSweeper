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
int wcenter(const int &w) {
    return 0;
}