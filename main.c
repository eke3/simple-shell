// File:    main.c
// Author:  Eric Ekey
// Date:    2/22/2025
// Desc:    This file contains the main function of a simple linux shell
//          designed to perform basic linux commands.

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "history_utils.h"
#include "shell_commands.h"
#include "utils.h"

#define AMPERSAND "&"
#define CD_CMD "cd"
#define CHANGE_PROMPT_CMD "prompt"
#define EXECUTE_FAILURE -1
#define EXIT_CMD "exit"
#define FWD_SLASH "/"
#define HISTORY_CMD "history"
#define PROC_CMD "/proc/"
#define PROC_CMD1 "/proc"

// Global variables.
char* history_file_path;
char* shell_directory;
char* shell_prompt;

#pragma region Prototypes

// Function prototypes.

// int execute_command(char**)
// Description: Executes the provided command.
// Preconditions: A non-null command is provided as an argument.
// Postconditions: The command is executed.
// Return: 0 on success, -1 on failure.
int execute_command(char**);

// char* get_user_command()
// Description: Gets user input from stdin.
// Preconditions: None.
// Postconditions: None.
// Return: A string containing the user input.
char* get_user_command(void);

// char** parse_command(char*)
// Description: Parses the user input into an array of command arguments.
// Preconditions: A non-null command is provided as an argument.
// Postconditions: None.
// Return: An array of command arguments.
char** parse_command(char*);

// void handle_sigint(int)
// Description: Handles the Ctrl+C signal interrupt.
// Preconditions: Ctrl+C is pressed.
// Postconditions: The interrupt is ignored and a message is printed to stdout.
// Return: None.
void handle_sigint(int);

// void print_cwd()
// Description: Prints the current working directory and shell prompt.
// Preconditions: shell_prompt is set.
// Postconditions: The current working directory and shell prompt are printed to
// stdout.
// Return: None.
void print_cwd(void);

// void set_up()
// Description: Sets up the shell environment.
// Preconditions: None.
// Postconditions: Memory is allocated for global variables and global variables
// are set.
// Return: None.
void set_up(void);

// void tear_down()
// Description: Tears down the shell environment.
// Preconditions: None.
// Postconditions: Memory is freed for global variables and command history is
// cleared. Exits process.
// Return: None.
void tear_down(void);

// void user_prompt_loop()
// Description: Gets user input repeatedly until the user enters the "exit"
// command. Manages command execution based on user input.
// Preconditions: Shell environment is set up.
// Postconditions: Commands are executed and appended to history according to 
//user input.
// Return: None.
void user_prompt_loop(void);

#pragma endregion Prototypes

int main(int argc, char** argv) {
  // Check for command-line arguments.
  if (argc > 1) {
    // Throw error if arguments are passed.
    fprintf(stderr, "Usage: %s\tCommand line arguments are not supported.\n",
            argv[0]);
    return 1;
  } else {
    // NOTE: Extra credit - detailed error messaging/handling throughout program.
    // Start program by calling the user_prompt_loop() function.
    set_up();
    user_prompt_loop();

    // Tear down the shell environment if the program somehow escapes the
    // user_prompt_loop() function without exiting.
    tear_down();
  }

  return 0;
}

#pragma region Implementations

void set_up() {
  // NOTE: Extra credit - implementing Ctrl+C signal interrupt.
  // Set Ctrl+C (SIGINT) signal interrupt handler.
  if (signal(SIGINT, handle_sigint) == SIG_ERR) {
    fprintf(stderr, "Error setting Ctrl+C signal handler\n");
  }

  if ((shell_directory = getcwd(NULL, 0)) == NULL) {
    perror("getcwd error in set_up()");
    exit(EXIT_FAILURE);
  }

  // Print full history file path to a global variable.
  if ((history_file_path = malloc(strlen(HISTORY_FILENAME) + strlen(FWD_SLASH) +
                                  strlen(shell_directory) + 1)) == NULL) {
    perror("history_file_path malloc error in set_up()");
    exit(EXIT_FAILURE);
  }
  if (snprintf(history_file_path,
               strlen(shell_directory) + strlen(FWD_SLASH) +
                   strlen(HISTORY_FILENAME) + 1,
               "%s%s%s", shell_directory, FWD_SLASH, HISTORY_FILENAME) < 0) {
    perror("snprintf error in set_up()");
    exit(EXIT_FAILURE);
  }

  // Print shell prompt to a global variable.
  if ((shell_prompt = malloc(strlen("$") + 1)) == NULL) {
    perror("shell_prompt malloc error in set_up()");
    exit(EXIT_FAILURE);
  }
  strcpy(shell_prompt, "$");
}

void tear_down() {
  // Clear command history.
  if (clear_history() == CLEAR_FAILURE) {
    fprintf(stderr, "Error clearing history file.\n");
    exit(EXIT_FAILURE);
  }

  // Free memory allocated for global variables.
  free(history_file_path);
  free(shell_directory);
  free(shell_prompt);
  exit(EXIT_SUCCESS);
}

void user_prompt_loop() {
  char* cmd = "";
  // Get user input repeatedly until the user enters the "exit" command.
  while (1) {
    // Always display the current working directory with the shell prompt.
    print_cwd();

    cmd = get_user_command();
    char** parsed_cmd = parse_command(cmd);
    int is_shell_cmd = 0;

    if (parsed_cmd != NULL) {
      // First argument is "exit".
      if (strcmp(parsed_cmd[0], EXIT_CMD) == 0) {
        if (parsed_cmd[1] != NULL) {
          // Invalid exit command.
          fprintf(stderr,
                  "Usage: exit\tAdditional arguments are not supported.\n");
        } else {
          // Valid exit command.
          free(cmd);
          for (int i = 0; parsed_cmd[i] != NULL; i++) {
            free(parsed_cmd[i]);
          }
          free(parsed_cmd);
          tear_down();
        }
        is_shell_cmd = 1;
      }

      // First argument starts with "/proc/".
      if (strncmp(parsed_cmd[0], PROC_CMD, strlen(PROC_CMD)) == 0) {
        // Case when command is passed as a single argument.
        if (parsed_cmd[1] != NULL) {
          // Invalid /proc command.
          fprintf(stderr,
                  "Usage: /proc/[filepath]\tAdditional arguments are "
                  "not supported.\n");
        } else {
          // Valid /proc command.
          if (execute_proc_command(parsed_cmd[0]) == EXEC_PROC_FAILURE) {
            fprintf(stderr, "Error executing /proc command.\n");
          }
        }
        is_shell_cmd = 1;
      } else if ((strcmp(parsed_cmd[0], PROC_CMD1) == 0) &&
                 (parsed_cmd[1] != NULL)) {
        // Case when command is passed as 2 arguments (e.g., "/proc /filepath").
        if (strncmp(parsed_cmd[1], FWD_SLASH, strlen(FWD_SLASH)) == 0) {
          if (parsed_cmd[2] == NULL) {
            // Valid /proc command that needs to be concatenated.
            char* proc_cmd_concat;
            if ((proc_cmd_concat = malloc(
                     (strlen(parsed_cmd[0]) + strlen(parsed_cmd[1]) + 1) *
                     sizeof(char))) == NULL) {
              perror("proc_cmd_concat malloc error in user_prompt_loop()");
              exit(EXIT_FAILURE);
            }
            strcpy(proc_cmd_concat, parsed_cmd[0]);
            strcat(proc_cmd_concat, parsed_cmd[1]);
            if (execute_proc_command(proc_cmd_concat) == EXEC_PROC_FAILURE) {
              fprintf(stderr, "Error executing /proc command.\n");
            }
            free(proc_cmd_concat);
          } else {
            fprintf(stderr,
                    "Usage: /proc/[filepath]\tAdditional arguments are "
                    "not supported.\n");
          }
        }
        is_shell_cmd = 1;
      }

      // First argument is "history".
      if (strcmp(parsed_cmd[0], HISTORY_CMD) == 0) {
        if (parsed_cmd[1] != NULL) {
          // Invalid history command.
          fprintf(stderr,
                  "Usage: history\tAdditional arguments are not supported.\n");
        } else {
          // Valid history command. Check for successsful execution.
          if (print_history() == PRINT_FAILURE) {
            fprintf(stderr, "Error printing command history.\n");
          }
        }
        is_shell_cmd = 1;
      }

      // NOTE: Extra credit - changes working directory.
      // First argument is "cd".
      if (strcmp(parsed_cmd[0], CD_CMD) == 0) {
        if (change_directory(parsed_cmd) == CD_FAILURE) {
          fprintf(stderr, "Error changing directory.\n");
        }
        is_shell_cmd = 1;
      }

      // NOTE: Extra credit - changes shell prompt.
      // First argument is "prompt".
      if (strcmp(parsed_cmd[0], CHANGE_PROMPT_CMD) == 0) {
        if (change_shell_prompt(parsed_cmd) == CHANGE_PROMPT_FAILURE) {
          fprintf(stderr, "Error changing shell prompt.\n");
        }
        is_shell_cmd = 1;
      }

      // Other commands for program executions.
      if (!is_shell_cmd) {
        if (execute_command(parsed_cmd) == EXECUTE_FAILURE) {
          fprintf(stderr, "Error executing command.\n");
        }
      }

      // Append latest command to history file.
      if (append_history(cmd) == APPEND_FAILURE) {
        fprintf(stderr, "Error appending to history file\n");
      }

      for (int i = 0; parsed_cmd[i] != NULL; i++) {
        free(parsed_cmd[i]);
      }
      free(parsed_cmd);
    }
    free(cmd);
  }
}

char* get_user_command() {
  // Initialize variables.
  char* user_command = NULL;
  size_t buffer_size = 0;
  ssize_t command_length = -1;

  // Dynamically allocate memory for the user command from stdin.
  if ((command_length = getline(&user_command, &buffer_size, stdin)) == -1) {
    perror("getline error in get_user_command()");
    free(user_command);
    if ((user_command = malloc(1)) == NULL) {
      perror("user_command malloc error in get_user_command()");
      exit(EXIT_FAILURE);
    }
    user_command[0] = '\0';
    return user_command;
  }

  // Remove trailing newline character if getline succeeds.
  if (user_command[command_length - 1] == '\n') {
    user_command[command_length - 1] = '\0';
  }
  return user_command;
}

char** parse_command(char* user_command) {
  // Initialize variables.
  size_t arg_capacity = 10;
  size_t arg_count = 0;
  size_t command_length = strlen(user_command);
  int start_pos = 0;
  char** parsed_command = malloc(arg_capacity * sizeof(char*));

  // Skip leading whitespaces.
  while ((start_pos < command_length) &&
         isspace((unsigned char)user_command[start_pos])) {
    start_pos++;
  }

  // If the command is empty, return NULL.
  if ((!command_length) || (start_pos == command_length)) {
    free(parsed_command);
    return NULL;
  }

  // Parse the command into arguments.
  while (start_pos < command_length) {
    // Reallocate more memory to hold the parsed command as necessary.
    if (arg_count >= arg_capacity) {
      arg_capacity *= 2;
      char** temp_parsed_cmd =
          realloc(parsed_command, arg_capacity * sizeof(char*));
      if (temp_parsed_cmd == NULL) {
        perror("realloc error in parse_command()");
        for (int i = 0; i < arg_count; i++) {
          free(parsed_command[i]);
        }
        free(parsed_command);
        return NULL;
      }
      parsed_command = temp_parsed_cmd;
    }

    int relative_space_pos = first_unquoted_space(user_command + start_pos);
    int space_pos =
        (relative_space_pos == -1) ? -1 : start_pos + relative_space_pos;

    if (space_pos == -1) {
      // If there are no spaces after the current start position, add the rest
      // of the command as the last argument.
      parsed_command[arg_count] =
          strndup(user_command + start_pos, command_length - start_pos);
      start_pos = command_length;
    } else {
      // Add the argument between the current start position and the next space
      // as the next argument.
      parsed_command[arg_count] =
          strndup(user_command + start_pos, space_pos - start_pos);
      start_pos = (space_pos + 1);
    }
    arg_count++;
  }
  parsed_command[arg_count] = NULL;

  // Expand the escape characters in the parsed command arguments.
  for (int i = 0; parsed_command[i] != NULL; i++) {
    char* temp_unescaped_arg = unescape(parsed_command[i], stderr);
    free(parsed_command[i]);
    parsed_command[i] = temp_unescaped_arg;
    temp_unescaped_arg = NULL;
  }

  return parsed_command;
}

int execute_command(char** parsed_command) {
  // NOTE: Extra credit - implementing background process execution.
  // Check if the command should start a background process.
  int is_background = 0;
  int i = 0;
  while (parsed_command[i] != NULL) {
    i++;
  }
  if (strcmp(parsed_command[i - 1], AMPERSAND) == 0) {
    free(parsed_command[i - 1]);
    parsed_command[i - 1] = NULL;
    is_background = 1;
  }

  // Create child process.
  pid_t process_id = fork();

  // Check for error in child process creation.
  if (process_id < 0) {
    perror("fork error in execute_command()");
    return EXECUTE_FAILURE;
  }

  if (process_id == 0) {
    // Child process.
    // Child executes the parsed command.
    if (execvp(parsed_command[0], parsed_command) == -1) {
      exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
  } else {
    // Parent process.
    if (!is_background) {
      // Wait for child process to terminate if it is not a background process.
      int status;
      if (waitpid(process_id, &status, 0) == -1) {
        perror("waitpid error in execute_command()");
        return EXECUTE_FAILURE;
      }
    }
  }

  return 0;
}

void handle_sigint(int sig) {
  // Ignore Ctrl+C interrupt.
  printf("\nInterrupt ignored. Type `exit` to quit.\n");
  print_cwd();
  fflush(stdout);
}

void print_cwd() {
  char* working_directory;

  if ((working_directory = getcwd(NULL, 0)) == NULL) {
    // Print the shell prompt by itself if the current working directory cannot
    // be obtained.
    perror("getcwd error in user_prompt_loop()");
    printf("%s ", shell_prompt);
  } else {
    // Print the current working directory in blue with the shell prompt.
    printf("\033[0;34m%s\033[0m%s ", working_directory, shell_prompt);
    free(working_directory);
  }
}

#pragma endregion Implementations