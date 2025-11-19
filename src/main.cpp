// MINESWEEPER CLI by StephenSQ made with PDCursesMod by Bill Gray
// g++ -std=c++17 -Iinclude -Isrc/headers src/main.cpp -Llib -static lib/pdcurses.a lib/panel.a -lwinmm -o build/minesweeper.exe
// ⠀⣞⢽⢪⢣⢣⢣⢫⡺⡵⣝⡮⣗⢷⢽⢽⢽⣮⡷⡽⣜⣜⢮⢺⣜⢷⢽⢝⡽⣝
// ⠸⡸⠜⠕⠕⠁⢁⢇⢏⢽⢺⣪⡳⡝⣎⣏⢯⢞⡿⣟⣷⣳⢯⡷⣽⢽⢯⣳⣫⠇
// ⠀⠀⢀⢀⢄⢬⢪⡪⡎⣆⡈⠚⠜⠕⠇⠗⠝⢕⢯⢫⣞⣯⣿⣻⡽⣏⢗⣗⠏⠀
// ⠀⠪⡪⡪⣪⢪⢺⢸⢢⢓⢆⢤⢀⠀⠀⠀⠀⠈⢊⢞⡾⣿⡯⣏⢮⠷⠁⠀⠀
// ⠀⠀⠀⠈⠊⠆⡃⠕⢕⢇⢇⢇⢇⢇⢏⢎⢎⢆⢄⠀⢑⣽⣿⢝⠲⠉⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⡿⠂⠠⠀⡇⢇⠕⢈⣀⠀⠁⠡⠣⡣⡫⣂⣿⠯⢪⠰⠂⠀⠀⠀⠀
// ⠀⠀⠀⠀⡦⡙⡂⢀⢤⢣⠣⡈⣾⡃⠠⠄⠀⡄⢱⣌⣶⢏⢊⠂⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⢝⡲⣜⡮⡏⢎⢌⢂⠙⠢⠐⢀⢘⢵⣽⣿⡿⠁⠁⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠨⣺⡺⡕⡕⡱⡑⡆⡕⡅⡕⡜⡼⢽⡻⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⣼⣳⣫⣾⣵⣗⡵⡱⡡⢣⢑⢕⢜⢕⡝⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⣴⣿⣾⣿⣿⣿⡿⡽⡑⢌⠪⡢⡣⣣⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⡟⡾⣿⢿⢿⢵⣽⣾⣼⣘⢸⢸⣞⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠁⠇⠡⠩⡫⢿⣝⡻⡮⣒⢽⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
#ifdef _WIN32 // they say this is important
#define main main // this as well though I forgot why...
#endif

#include "headers/gamestages.hpp"

int main(void) {
    std::cout << "\n\nThis program is... buggy, DO NOT RESIZE THE SCREEN WHILE RUNNING THE PROGRAM" << "\n";
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
        side_scr.set_style(C_MAGENTA, A_BOLD); side_scr.wsprint(6, 1, "0  / " + std::to_string(mine_count) + " MINES are flagged... supposedly.");
        // display the first minefield empty
        display_minefield(minesweeper);
        // create the global cursor that 
        GRIDCURSOR MINEFIELD_CURSOR(minefield_y, minefield_x, minesweeper);
        minesweeper.set_style(6, 0);MINEFIELD_CURSOR.move(0,0);// show the cursor on minesweeper
        update_panels(); doupdate();
        // GAME LOOP
        bool playing_ = true, player_WON = false;

        // returns a std::chrono::time_point<Clock> object which requires std::chrono::duration<>
        auto start_record_gameplay = std::chrono::high_resolution_clock::now(); // TIME RECORDING THE START OF GAMEPLAY

        while (playing_)
        {
            // flash(); epilepsy warning
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
            // >>>>> PRESS ENTER TO DIG FOR MINES !!!
            case '\n': case '\r': case KEY_ENTER: 
                if (MINEFIELD_CURSOR.dig_this()) {
                    game_over(minesweeper, side_scr, MINEFIELD_CURSOR.y_cursor, MINEFIELD_CURSOR.x_cursor);
                    playing_ = false;
                } break;
            // >>>>> PRESS F TO FLAG THE MINES !!!
            case 'f': 
                if (!MINEFIELD_CURSOR.flag_this()) {
                    side_scr.set_style(C_GOLD, A_BOLD); side_scr.wsprint(3, 1,    "!!! < CAUTION > !!! YOU'RE OUT OF FLAGS !!!");}
                else {
                    side_scr.set_style(C_MAGENTA, A_BOLD); side_scr.wsprint(3, 1, "PRESS -F- to FLAG the MINES !              ");}
                // display how many mines are flagged
                side_scr.wsprint(6, 1, std::to_string(flagged_mines) + " ");
                MINEFIELD_CURSOR.move(0, 0); // return the cursor back to the minesweeper screen then break
                // CHECK WIN CONDITION EACH GAME LOOP, indicate true if won and exit the game loop
                if (player_won()) {player_WON = true; playing_ = false;} break;
            // >>>>> PRESS M TO OPEN MANUAL
            case 'm':
                display_manual_book();  break;
            }
            update_panels(); doupdate(); // update display first to ensure proper display
        }

        auto end_record_gameplay = std::chrono::high_resolution_clock::now(); // TIME RECORDING THE END OF GAMEPLAY
        // record elapsed time as playtime in miliseconds which is 0.001 second accuracy,
        // playtime_ is a duration object that requirees .count() to get the actual value.
        std::chrono::duration<double, std::milli> playtime_ = end_record_gameplay - start_record_gameplay - time_penalty;

        // reward in prayer if player spent their finite lifetime trying out this program.
        if (player_WON) {win_screen(playtime_);}
        // ask to play again or quit
        if (prompt_replay()) {continue;} // restart the game from the very beginning
        else {break;} // exit and end the gameplay
    }
    endwin(); // the end of it all, clean up and exit Curses mode
    std::cout << "\n\nEnd of Session. Thanks for playing :)\n\n" << std::endl;
    std::cout << "Source code available at https://github.com/DefinitelyProfessional/CPPSweeper\n\n" << std::endl;
    return 0;
}
