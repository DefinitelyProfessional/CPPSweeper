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
    // MINESWEEPER MAIN LOOP
    while(true) {
        // the player choose how big the field they want to work with.
        prompt_minefield_assignment();
        // setup the minefield
        generate_minefield();
        compute_minefield();
        // START THE GAME
        WINPAN minesweeper(minefield_y+2, minefield_x*3+2, 0, 0, true);
        WINPAN side_scr(LINES-minefield_y-2, minefield_x*3+2, minefield_y+2, 0, true);
        side_scr.set_style(C_RED, A_BOLD); side_scr.wsprint(1, 1, "PRESS -M- to open the MANUAL book.");
        side_scr.set_style(C_ORANGE, A_BOLD); side_scr.wsprint(2, 1, "PRESS -W A S D- to move the cursor.");
        side_scr.set_style(C_MAGENTA, A_BOLD); side_scr.wsprint(3, 1, "PRESS -F- to FLAG the MINES !");
        side_scr.set_style(C_GOLD, A_BOLD); side_scr.wsprint(4, 1, "PRESS -ENTER- to DIG FOR MINES !");
        side_scr.set_style(C_WHITE, A_BOLD); side_scr.wsprint(5, 1, "< G O O D L U C K > you'll need it.");
        // display the first minefield empty
        display_minefield(minesweeper);
        update_panels(); doupdate();
        // create the global cursor that 
        GRIDCURSOR MINEFIELD_CURSOR(minefield_y, minefield_x, minesweeper);
        minesweeper.set_style(6, 0);MINEFIELD_CURSOR.move(0,0);// show the cursor on minesweeper
        // GAME LOOP
        while (true)
        {
            minesweeper.set_style(6, 0); // for the cursor
            switch (minesweeper.input())
            {
            case 'w': // PRESS W TO MOVE UPWARDS
                MINEFIELD_CURSOR.move(-1, 0);
                break;
            case 'a': // PRESS A TO MOVE LEFT
                MINEFIELD_CURSOR.move(0, -1);
                break;
            case 's': // PRESS S TO MOVE DOWNWARDS
                MINEFIELD_CURSOR.move(1, 0);
                break;
            case 'd': // PRESS D TO MOVE RIGHT
                MINEFIELD_CURSOR.move(0, 1);
                break;
            case '\n': case '\r': case KEY_ENTER: // PRESS ENTER TO DIG FOR MINES !
                break;
            case 'f': // PRESS F TO FLAG THE MINES
                break;
            case 'm': // PRESS M TO OPEN MANUAL
                break;
            default: // 
                break;
            }
        }
        

        side_scr.input();
        minesweeper.input();
        break;
    }

    endwin(); // the end of it all, clean up and exit Curses mode
    std::cout << "\nEnd of Session. Thanks for playing :)\n" << std::endl;
    return 0;
}
