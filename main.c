#include <stdio.h>
#include <ncurses.h>

int main(int argc, char ** argv) {
    initscr();        // initialize ncurses
    // Your code here
    refresh();

    getch();

    endwin();         // cleanup ncurses
    return 0;
}
