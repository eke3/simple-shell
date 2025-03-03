// File:    main.c
// Author:  Eric Ekey
// Date:    2/22/2025
// Desc:    This file contains utility functions for managing the shell's
//          command history.

#include "history_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int append_history(const char* command) {
  FILE* history_file;

  // Open history file in append mode.
  if ((history_file = fopen(history_file_path, "a")) == NULL) {
    perror("fopen error in append_history()");
    return APPEND_FAILURE;
  };

  // Append command to history file.
  fprintf(history_file, "%s\n", command);
  fclose(history_file);
  return 0;
}

int clear_history() {
  FILE* history_file;

  // Opening in write mode clears the file. Check for unsuccessful open.
  if ((history_file = fopen(history_file_path, "w")) == NULL) {
    perror("fopen error in clear_history()");
    return CLEAR_FAILURE;
  }

  fclose(history_file);
  return 0;
}