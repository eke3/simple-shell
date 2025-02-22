// File:    main.c
// Author:  Eric Ekey
// Date:    2/22/2025
// Desc:    This file contains utility functions for managing the shell's command history.

#include "history_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int append_history(const char* command) {
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