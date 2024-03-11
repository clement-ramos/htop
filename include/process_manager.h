// process_manager.h
#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <ncurses.h>

#define MAX_PROCESSES 100  // Adjust this value based on your needs

void GetProcessList(int pids[], char names[][256], double cpu_usages[], double mem_usages[], int *processCount, WINDOW *win);

// Autres déclarations de fonction...

#endif // PROCESS_MANAGER_H
