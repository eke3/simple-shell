#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#define CD_FAILURE -1
#define CHANGE_PROMPT_FAILURE -1
#define EXEC_PROC_FAILURE -1
#define MAX_HISTORY_LINES 10
#define PRINT_FAILURE -1

#ifdef __cplusplus
extern "C" {
#endif

// Global variable for history file path. Ensures that the history is always in the same directory as the shell executable.
extern char* history_file_path;
extern char* shell_prompt;


extern int change_directory(char**);

extern int change_shell_prompt(char**);

extern int execute_proc_command(char*);

extern int print_history(void);



#ifdef __cplusplus
}
#endif

#endif