// File:    shell_commands.c
// Author:  Eric Ekey
// Date:    2/22/2025
// Desc:    This file contains functions for executing built-in shell commands.

#include "shell_commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "bg_utils.h"

int change_directory(char** parsed_command) {
  // Check for number of arguments.
  size_t num_args = 0;
  while (parsed_command[num_args] != NULL) {
    num_args++;
  }

  // Set destination based on whether an argument is provided.
  char* destination;
  if (num_args > 2) {
    fprintf(stderr, "Usage: cd [directory]\tToo many arguments.\n");
    return CD_FAILURE;
  } else if (num_args == 2) {
    destination = parsed_command[1];
  } else if (num_args == 1) {
    destination = getenv(HOME_ENV);
  }

  // Change directory to the destination.
  if (chdir(destination) == -1) {
    perror("chdir error in change_directory().");
    return CD_FAILURE;
  }

  return 0;
}

int change_shell_prompt(char** parsed_command) {
  // Check for too few arguments.
  if (parsed_command[1] == NULL) {
    fprintf(stderr, "Usage: prompt [prompt]\tToo few arguments.\n");
    return CHANGE_PROMPT_FAILURE;
  }

  // Check for too many arguments.
  if (parsed_command[2] != NULL) {
    fprintf(stderr, "Usage: prompt [prompt]\tToo many arguments.\n");
    return CHANGE_PROMPT_FAILURE;
  }

  // Allocate memory for new prompt.
  free(shell_prompt);
  shell_prompt = strdup(parsed_command[1]);
  if (!shell_prompt) {
    perror("strdup error in change_shell_prompt()");
    return CHANGE_PROMPT_FAILURE;
  }

  return 0;
}

int execute_proc_command(char* proc_file_path) {
  FILE* proc_file;

  // Try to open the proc file.
  if ((proc_file = fopen(proc_file_path, "r")) == NULL) {
    perror("fopen error in execute_proc_command()");
    return EXEC_PROC_FAILURE;
  }

  char* line = NULL;
  size_t proc_file_length = 0;

  while (getline(&line, &proc_file_length, proc_file) != -1) {
    // Read and print the contents of the proc file.
    printf("%s", line);
  }

  free(line);
  fclose(proc_file);
  return 0;
}

int foreground_process(pid_t process_id) {
  if (process_id <= 0) {
    // Process id is not an integer greater than 0.
    fprintf(stderr, "Invalid process id %d. Enter an id greater than 0.\n",
            process_id);
    return FG_FAILURE;
  }

  // Check if process exists among background processes.
  int process_exists = 0;
  for (int i = 0; i < bg_processes->capacity; i++) {
    if (bg_processes->process_ids[i] == process_id) {
      process_exists = 1;
      break;
    }
  }

  if (!process_exists) {
    // Specified process does not exist among background processes.
    fprintf(stderr, "Process with id %d does not exist.\n", process_id);
  } else {
    // Foreground process and remove from list of background processes.
    if (remove_bg_process(process_id) == REMOVE_BG_FAILURE) {
      fprintf(stderr, "Error removing process %d from background processes.\n",
              process_id);
    }
    int status;
    if (waitpid(process_id, &status, 0) == -1) {
      perror("waitpid error in foreground_process()");
    }
  }

  return 0;
}

int list_bg_processes(void) {
  if (bg_processes == NULL) {
    // Global struct not initialized.
    return BG_FAILURE;
  } else {
    // List active background processes.
    if (bg_processes->num_processes == 0) {
      printf("No active background processes.\n");
    } else {
      for (int i = 0, cnt = 1; i < bg_processes->capacity; i++) {
        if (bg_processes->process_ids[i] > DEAD_PROCESS_ID) {
          printf("[%d]\t%d\n", cnt, bg_processes->process_ids[i]);
          cnt++;
        }
      }
    }
  }
  return 0;
}

int print_history() {
  FILE* history_file;

  // Try to open the history file.
  if ((history_file = fopen(history_file_path, "r")) == NULL) {
    perror("fopen error in print_history()");
    return PRINT_FAILURE;
  }

  char* line = NULL;
  char* lines[MAX_HISTORY_LINES];
  size_t line_count = 0;
  size_t length = 0;

  while (getline(&line, &length, history_file) != -1) {
    // Free old lines if we've already read the max number of lines.
    if (line_count == MAX_HISTORY_LINES) {
      free(lines[line_count % MAX_HISTORY_LINES]);
    }
    // Store the line.
    lines[line_count % MAX_HISTORY_LINES] = strdup(line);

    line_count++;
  }

  // Print the last 10 lines if available.
  int start =
      (line_count >= MAX_HISTORY_LINES) ? (line_count % MAX_HISTORY_LINES) : 0;
  int total_lines_to_print =
      (line_count < MAX_HISTORY_LINES) ? line_count : MAX_HISTORY_LINES;

  for (int i = 0; i < total_lines_to_print; i++) {
    int idx = (start + i) % MAX_HISTORY_LINES;
    printf("[%d]\t%s", i + 1, lines[idx]);
    free(lines[idx]);
  }

  free(line);
  fclose(history_file);
  return 0;
}