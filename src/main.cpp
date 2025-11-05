// MINESWEEPER CLI by StephenSQ made with PDCursesMod by Bill Gray
// cd "C:\Users\locke\DATA\Projects\cpp_with_curses\CPPSweeper"
// g++ -std=c++17 -Iinclude -Isrc/headers src/main.cpp -Llib -static lib/pdcurses.a lib/panel.a -lwinmm -o build/minesweeper.exe

#ifdef _WIN32
#define main main
#endif

#include "headers/gamestages.hpp"

int main(void) {
    std::cout << "This program is... buggy, DO NOT RESIZE THE SCREEN WHILE RUNNING THE PROGRAM" << "\n";
    std::cout << "Thus for your comfort, resize this terminal window to your liking before continuing" << "\n";
    std::cout << "press ENTER to START "; std::string s; std::getline(std::cin, s); s = ""; // awkward method
    
    initialize_cursed_environment(); // initscr() and start_color() is called here 

    welcome_screen(); // simply prompt the gamestart screen.
    // MINESWEEPER GAME MAIN LOOP
    while(true) {
        // the player choose how big the field they want to work with.
        prompt_minefield_assignment();
        // START THE GAME
        WINPAN minesweeper(minefield_y, minefield_x*3, 0, 0, true);
        WINPAN side_scr(LINES-minefield_y, minefield_x*3, minefield_y, 0, true);
        
        update_panels(); doupdate();
        side_scr.input();
        minesweeper.input();
        break;
    }

    endwin(); // the end of it all, clean up and exit Curses mode
    std::cout << "\nEnd of Session. Thanks for playing :)\n" << std::endl;
    return 0;
}
