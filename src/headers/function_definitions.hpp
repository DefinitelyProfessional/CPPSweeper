// FUNCTION DEFINITIONS ARE HERE !!!

#pragma once // header files good practice

#define PDC_STATIC // somewhat important for include curses.h
#define PDC_RGB // somewhat important as well
#include <curses.h>
#include <panel.h>
#include "variable_definitions.hpp"

// HELPER FUNCTION FOR prompt_int()
void display_prompt(WINDOW* win, const int &row_, const int &col_, const std::string& message){
    mvwprintw(win, row_, col_, "%s   ", message.c_str()); // print message prompt
    int cursor_x = col_ + (int)message.size(); // calculate where the cursor starts
    wmove(win, row_, cursor_x); // position the cursor at the end of the message
    wrefresh(win); return; // display the prompt and exit
}
// PROMPT INTEGER INPUTS
int prompt_int(WINDOW* win, const int &row_, const int &col_,
const std::string& message, const int &low_, const int &high_) {
    std::string input_; // this is the buffer to receive inputs
    constexpr int MAX_DIGITS = 3;

    display_prompt(win, row_, col_, message);
    
    keypad(win, TRUE);// Ensure keypad for special keys and immediate input

    int ch, message_length = message.size(); 
    // THIS IS THE JANKY SINGLE CHARACTER BY CHARACTER INPUT
    while (true) {
        ch = wgetch(win); // This is what prompts for single key press inputs, blocking.

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
            catch (...) {input_ = ""; display_prompt(win, row_, col_, message); continue;}
        }
        // HANDLE BACKSPACE DELETES
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (!input_.empty()) {
                input_.pop_back();
                // redraw last char space and move cursor
                mvwaddch(win, row_, col_ + message_length + (int)input_.size(), ' ');
                wmove(win, row_, col_ + message_length + (int)input_.size());
                // wrefresh(win);
            }
            continue;
        }
        // HANDLE ACCEPTABLE CHARACTERS
        else if (std::isdigit(ch) && (int)input_.size() < MAX_DIGITS) {
            input_.push_back((char)ch);
            // print the new char and advance cursor
            mvwprintw(win, row_, col_ + message_length, "%s", input_.c_str());
            wmove(win, row_, col_ + message_length + (int)input_.size());
            // wrefresh(win);
            continue;
        }
        update_panels(); doupdate();
    }
}