// main.c
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include "process_manager.h"

#define MAX_PROCESSES 100  // Adjust this value based on your needs

void display_processes(WINDOW *win, int pids[], char names[][256], double cpu_usages[], double mem_usages[], int processCount) {
    int row = 1;

    mvwprintw(win, 0, 0, "PID\tName\t\t\tCPU Usage\tMemory Usage");

    for (int i = 0; i < processCount; ++i) {
        mvwprintw(win, row++, 0, "%d\t%-20s\t%.2f%%\t\t%.2f%%", pids[i], names[i], cpu_usages[i], mem_usages[i]);
    }

    wrefresh(win); // Rafraîchir la fenêtre après l'ajout des informations
}

int main() {
    WINDOW *win;

    initscr(); // Initialiser ncurses
    start_color(); // Activer la prise en charge des couleurs
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Définir la paire de couleurs

    win = newwin(20, 80, 0, 0); // Créer une nouvelle fenêtre
    wbkgd(win, COLOR_PAIR(1)); // Définir la couleur de fond

    int pids[MAX_PROCESSES];  // Ajuster la taille en conséquence
    char names[MAX_PROCESSES][256];  // Ajuster la taille en conséquence
    double cpu_usages[MAX_PROCESSES];  // Ajuster la taille en conséquence
    double mem_usages[MAX_PROCESSES];  // Ajuster la taille en conséquence
    int processCount;

    while (1) {
        wclear(win); // Effacer la fenêtre
        GetProcessList(pids, names, cpu_usages, mem_usages, &processCount, win); // Obtenir et afficher les informations système
        // display_processes(win, pids, names, cpu_usages, mem_usages, processCount); // Ne pas appeler cette fonction ici
        wrefresh(win); // Rafraîchir la fenêtre
        timeout(1000); // Attendre 1 seconde avant de rafraîchir la fenêtre
    }

    endwin(); // Terminer ncurses
    return 0;
}
