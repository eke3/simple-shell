#ifndef HISTORY_UTILS_H
#define HISTORY_UTILS_H

#define HISTORY_FILENAME ".421sh"
#define APPEND_FAILURE -1
#define CLEAR_FAILURE -1

#ifdef __cplusplus
extern "C" {
#endif

// Global variable for history file path. Ensures that the history is always in the same directory as the shell executable.
extern char* history_file_path;


extern int append_history(const char *command);

extern int clear_history(void);


#ifdef __cplusplus
}
#endif

#endif // HISTORY_UTILS_H