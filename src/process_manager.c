#include "process_manager.h"

void get_process_stat(long pid, ProcessInfo *process);
void get_process_statm(long pid, ProcessInfo *process);
void get_process_command(long pid, ProcessInfo *process);
long get_process_total_memory();


void display_tables_titles() {
    attron(COLOR_PAIR(3));
    mvprintw(0, 2, " PID ");
    mvprintw(0, 8, " CPU%% ");
    mvprintw(0, 14, " MEM%% ");
    mvprintw(0, 22, " TIME+ ");
    mvprintw(0, 32, " Command ");
    attroff(COLOR_PAIR(3));
}

void display_processes(WINDOW *window, int current_scroll_position, int *current_row) {
    DIR *dir;
    if ((dir = opendir("/proc")) == NULL) {
        mvwprintw(window, 1, 1, "Error: Could not open directory (src/pid_info.c:display_processes:directory)");
        return;
    }

    int row = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char *endptr;
            long pid = strtol(entry->d_name, &endptr, 10);
            long total_memory = get_process_total_memory();
            int row_position = current_scroll_position + row + 1;

            ProcessInfo processInfo;
            processInfo.pid = pid;
            get_process_stat(pid, &processInfo);
            get_process_statm(pid, &processInfo);
            get_process_command(pid, &processInfo);

            if (*endptr == '\0' && processInfo.command[0] != '\0') {
                mvwprintw(window, row_position, 2, "%ld", processInfo.pid);
                mvwprintw(window, row_position, 8, "%.1f", (float) (processInfo.utime + processInfo.stime) / processInfo.size * 100.0);
                mvwprintw(window, row_position, 14, "%.1f", (float) (processInfo.resident + processInfo.shared + processInfo.virt) / (processInfo.resident + processInfo.shared + processInfo.virt + total_memory) * 100.0);

                mvwprintw(window, row_position, 24, "%ld:%02ld.%02ld",
                          (processInfo.utime + processInfo.stime) / 6000,
                          ((processInfo.utime + processInfo.stime) % 6000) / 100,
                          (processInfo.utime + processInfo.stime) % 100);

                mvwprintw(window, row_position, 32, "%.*s", COLS - 32 - 1, processInfo.command);
                row++;
            }
        }
    }
    closedir(dir);
    if (current_row != NULL ) {
        *current_row = row;
    }
    return;
}

void get_process_statm(long pid, ProcessInfo *process) { // Get process memory info
    char path[256];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%ld/statm", pid);
    file = fopen(path, "r");
    if (file == NULL) {
        return;
    }

    fscanf(file, "%ld %ld %ld", &process->size, &process->resident, &process->shared);
    fclose(file);
    process->size *= 4;
    process->resident *= 4;
    process->shared *= 4;
    return;
}

long get_process_total_memory() { // Get total memory
    FILE *file;
    char buffer[256];
    long total_memory = -1;

    file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier /proc/meminfo");
        return total_memory;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strncmp(buffer, "MemTotal:", 9) == 0) {
            sscanf(buffer, "%*s %ld", &total_memory);
            break;
        }
    }

    fclose(file);
    return total_memory * 1024;
}

void get_process_stat(long pid, ProcessInfo *process) { // Get process info
    int column = 1;
    char *token;
    char path[256];
    char buffer[256];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%ld/stat", pid);
    file = fopen(path, "r");
    if (file == NULL) {
        return;
    }

    fgets(buffer, sizeof(buffer), file);
    token = strtok(buffer, " ");
    while (token) {
        if (column == 3) process->state = token[0];
        if (column == 14) process->utime = atol(token);
        if (column == 15) process->stime = atol(token);
        if (column == 18) process->priority = atoi(token);
        if (column == 19) process->nice = atoi(token);
        if (column == 23) process->virt = atoll(token);
        token = strtok(NULL, " ");
        column++;
    }
    fclose(file);
    return;
}
void get_process_command(long pid, ProcessInfo *process) { // Get process command
    char path[256];
    char buffer[256];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%ld/cmdline", pid);
    file = fopen(path, "r");
    if (file == NULL) {
        return;
    }

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        process->command[0] = '\0';
        return;
    }

    fclose(file);
    strcpy(process->command, buffer);
    return;
}

void kill_process() { // Function to kill a selected process
    int pid;
    char pid_str[10];
    WINDOW *input_win;

    // Create a window for the input
    input_win = newwin(3, 200, LINES - 5, 1);
    mvwprintw(input_win, 1, 1, "Enter PID to kill (F2 to cancel): ");
    wrefresh(input_win);

    int ch;
    int i = 0;
    while (i < 9) {
        ch = getch();

        // Handle enter and escape
        if (ch == '\n') {
            break;
        } else if (ch == KEY_F(2)) {
            i = 0;
            break;
        }
        // Handle backspace
        if (ch == KEY_BACKSPACE && i > 0) {
            --i;
            mvwdelch(input_win, 1, 35 + i);
            wrefresh(input_win);
            continue;
        }
        // Only display digits
        if (isdigit(ch)) {
            pid_str[i++] = ch;
            waddch(input_win, ch);
            wrefresh(input_win);
        }
    }
    pid_str[i] = '\0';
    pid = atoi(pid_str);

    if (kill(pid, SIGTERM) == 0) {
        attron(COLOR_PAIR(3));
        mvprintw(LINES - 2, 2, " Process %d killed successfully. ", pid);
        attroff(COLOR_PAIR(3));
    } else {
        attron(COLOR_PAIR(3));
        mvprintw(LINES - 2, 2, " Failed to kill process %d. ", pid);
        attroff(COLOR_PAIR(3));
    }

    getch();
    delwin(input_win);
    endwin();
}