#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#define BG_FAILURE -1
#define CD_FAILURE -1
#define CHANGE_PROMPT_FAILURE -1
#define EXEC_PROC_FAILURE -1
#define FG_FAILURE -1
#define HOME_ENV "HOME"
#define MAX_HISTORY_LINES 10
#define PRINT_FAILURE -1

#include <unistd.h>

extern struct bg_processes_t* bg_processes;
extern char* history_file_path;
extern char* shell_prompt;

#ifdef __cplusplus
extern "C" {
#endif

// int change_directory(char**)
// Description: Changes the current working directory.
// Preconditions: A non-null command is provided as an argument.
// Postconditions: The current working directory is changed according to the
// command.
// Return: 0 on success, -1 on failure.
extern int change_directory(char**);

// int change_shell_prompt(char**)
// Description: Changes the shell prompt.
// Preconditions: A non-null command is provided as an argument.
// Postconditions: The shell prompt is changed according to the command.
// Return: 0 on success, -1 on failure.
extern int change_shell_prompt(char**);

// int execute_proc_command(char*)
// Description: Executes a proc command.
// Preconditions: A non-null command is provided as an argument.
// Postconditions: The proc command is executed by displaying the contents of
// the proc file.
// Return: 0 on success, -1 on failure.
extern int execute_proc_command(char*);

// int foreground_process(pid_t)
// Description: Moves a background process to the foreground.
// Preconditions: The bg_processes struct is initialized and a process id is 
// provided as an argument.
// Postconditions: The process is moved to the foreground.
// Return: 0 on success, -1 on failure.
extern int foreground_process(pid_t);

// int list_bg_processes()
// Description: Lists the active background processes.
// Preconditions: The bg_processes struct is initialized.
// Postconditions: The active background processes are printed to stdout.
// Return: 0 on success, -1 on failure.
extern int list_bg_processes(void);

// int print_history()
// Description: Prints the command history.
// Preconditions: history_file_path is set. History file exists.
// Postconditions: The 10 most recent commands are printed to stdout.
// Return: 0 on success, -1 on failure.
extern int print_history(void);

#ifdef __cplusplus
}
#endif

#endif