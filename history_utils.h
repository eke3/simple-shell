#ifndef HISTORY_UTILS_H
#define HISTORY_UTILS_H

#define HISTORY_FILENAME ".421sh"
#define MAX_HISTORY_LINES 10
#define APPEND_FAILURE -1
#define PRINT_FAILURE -1
#define CLEAR_FAILURE -1

#ifdef __cplusplus
extern "C" {
#endif


extern int append_history(const char *command);

extern int print_history(void);

extern int clear_history(void);


#ifdef __cplusplus
}
#endif

#endif // HISTORY_UTILS_H