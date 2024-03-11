// process_manager.h
#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <ctype.h>
#include <dirent.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long pid;           // Process ID
    int priority;       // Priority
    int nice;           // Nice value
    long size;          // Virtual memory size
    long total_size;    // Total program size
    long resident;      // Resident Set Size
    long shared;        // Shared pages
    long utime;         // User time
    long stime;         // Kernel time
    long long virt;     // Virtual memory size
    char state;         // State
    char command[256];  // Command
} ProcessInfo;

void display_tables_titles();
void display_processes(WINDOW *window, int current_scroll_position, int *current_row);
void kill_process();

#endif