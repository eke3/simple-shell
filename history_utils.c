#include "history_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variable for history file path. Ensures that the history is always in the same directory as the shell executable.
extern char* history_file_path;

int append_history(const char *command) {
    FILE* history_file = fopen(history_file_path, "a");

    // Check for unsuccessful open.
    if (history_file == NULL) {
        perror("error opening .421sh history file");
        return APPEND_FAILURE;
    }

    // Append command to history file.
    fprintf(history_file, "%s\n", command);
    fclose(history_file);

    return 0;
}


int print_history() {
    FILE* history_file = fopen(history_file_path, "r");

    // Check for unsuccessful open.
    if (history_file == NULL) {
        perror("error opening .421sh history file");
        return PRINT_FAILURE;
    }

    // Print last 10 lines from file.
    char* line = NULL;
    char* lines[MAX_HISTORY_LINES];
    int line_count = 0;
    size_t length = 0;
    size_t chars_read;

    while ((chars_read = getline(&line, &length, history_file)) != -1) {
        // Free old lines if we've already read the max number of lines.
        if (line_count == MAX_HISTORY_LINES) {
            free(lines[line_count % MAX_HISTORY_LINES]);
        }
        // Store the line.
        lines[line_count % MAX_HISTORY_LINES] = strdup(line);
        line_count++;
    }

    // Print the last 10 lines if available.
    int start = (line_count >= MAX_HISTORY_LINES) ? (line_count % MAX_HISTORY_LINES) : 0;
    int total_lines_to_print = (line_count < MAX_HISTORY_LINES) ? line_count : MAX_HISTORY_LINES;

    for (int i = 0; i < total_lines_to_print; i++) {
        int idx = (start + i) % MAX_HISTORY_LINES;
        printf("[%d]\t%s", i+1, lines[idx]);
        free(lines[idx]);  // Free each line after printing
    }

    free(line);
    fclose(history_file);
    return 0;
}   


int clear_history() {
    // Opening in write mode clears the file.
    FILE* history_file = fopen(history_file_path, "w");

    // Check for unsuccessful open.
    if (history_file == NULL) {
        perror("error opening .421sh history file");
        return CLEAR_FAILURE;
    }

    fclose(history_file);
    return 0;
}