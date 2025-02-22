#ifndef HISTORY_UTILS_H
#define HISTORY_UTILS_H

#define APPEND_FAILURE -1
#define CLEAR_FAILURE -1
#define HISTORY_FILENAME ".421sh"

extern char* history_file_path;

#ifdef __cplusplus
extern "C" {
#endif

// int append_history(const char*)
// Description: Appends the provided command to the history file.
// Preconditions: history_file_path is set. A non-null command is provided as an argument.
// Postconditions: The command is appended to the history file. History file is created if it does not exist.
// Return: 0 on success, -1 on failure.
extern int append_history(const char*);

// int clear_history()
// Description: Clears the history file.
// Preconditions: history_file_path is set.
// Postconditions: The history file is cleared. 
// Return: 0 on success, -1 on failure.
extern int clear_history(void);

#ifdef __cplusplus
}
#endif

#endif // HISTORY_UTILS_H