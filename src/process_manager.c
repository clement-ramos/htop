// process_manager.c
#include "process_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GetProcessList(int pids[], char names[][256], double cpu_usages[], double mem_usages[], int *processCount, WINDOW *win) {
    FILE *fp = popen("ps -e -o pid,comm,%cpu,%mem --no-headers", "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    while (fscanf(fp, "%d %s %lf %lf", &pids[count], names[count], &cpu_usages[count], &mem_usages[count]) == 4 && count < MAX_PROCESSES) {
        count++;
    }

    *processCount = count;

    wprintw(win, "PID\tName\t\t\tCPU Usage\tMemory Usage\n");

    for (int i = 0; i < count; i++) {
        wprintw(win, "%d\t%-20s\t%.2f%%\t\t%.2f%%\n", pids[i], names[i], cpu_usages[i], mem_usages[i]);
    }

    pclose(fp);
}

