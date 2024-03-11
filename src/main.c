#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#include "process_manager.h"

// Function declarations
void display_footers_keys();
void error(char *message);

int main(void) {
    // Variable declarations
    int ch;
    int current_scroll_position = 0;
    int current_row = 0;
    WINDOW *processus_win; // Declare a window where processes will be displayed

    // Initialize ncurses
    initscr();
    curs_set(0); // Hide the cursor
    noecho(); // Don't echo any keypresses
    keypad(stdscr, TRUE); // Enable function and arrow keys
    start_color(); // Enable color
    use_default_colors(); // Use terminal's default colors

    // Define color pairs
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_CYAN, -1);
    init_pair(3, COLOR_RED, -1);

    timeout(1000); // Refresh every 1s

    do {
        clear();
        int inverse_current_row = current_row * -1;
        // Create a window for displaying processes
        processus_win = subwin(stdscr, LINES - 4, COLS - 1, 0, 1);

        // Display processes in the window
        display_processes(processus_win, current_scroll_position, &current_row);

        // Draw a box around the window and display titles and keys
        box(processus_win, 0, 0);
        display_tables_titles();
        display_footers_keys();
        wrefresh(processus_win);
        refresh();

        // Get user input
        ch = getch();
        switch (ch) {
            case KEY_DOWN:
                if (current_scroll_position > inverse_current_row + LINES)
                    current_scroll_position--;
                break;
            case KEY_UP:
                if (current_scroll_position < 0)
                    current_scroll_position++;
                break;
            case KEY_F(1):
                kill_process(); // Function to kill a selected process
                break;
        }
    } while (ch != KEY_F(2)); // Press F2 to exit

    // Clean up and exit
    delwin(processus_win); // Delete the window
    endwin(); // End ncurses
    return EXIT_SUCCESS;
}

// Function to display keys at the bottom of the screen
void display_footers_keys() {
    mvprintw(LINES - 1, 10, "F1");
    mvprintw(LINES - 1, 18, "F2");

    attron(COLOR_PAIR(1));
    mvprintw(LINES - 1, 12, "Kill  ");
    mvprintw(LINES - 1, 20, "Quit\t\t\t\t\t\t\t");
    attroff(COLOR_PAIR(1));
}

// Function to handle errors and exit
void error(char *message) {
    endwin();
    perror(message);
}
