#ifndef BG_UTILS_H
#define BG_UTILS_H

#define CLEAR_BG_FAILURE -1
#define DEAD_PROCESS_ID 0
#define REMOVE_BG_FAILURE -1
#define SETUP_FAILURE -1

#include <unistd.h>

// Struct holding info for background process management.
struct bg_processes_t {
    pid_t* process_ids;
    size_t num_processes;
    size_t capacity;
};

extern struct bg_processes_t* bg_processes;

#ifdef __cplusplus
extern "C" {
#endif

// int append_bg_process(pid_t)
// Description: Adds a process id to the bg_processes struct.
// Preconditions: bg_processes struct is initialized.
// Postconditions: The process id is added to the array of background processes.
// Return: 0 on success, -1 on failure.
extern int append_bg_process(pid_t);

// int clear_bg_processes()
// Description: Resets the bg_processes struct.
// Preconditions: bg_processes struct is initialized.
// Postconditions: The bg_processes struct members are reset and freed.
// Return: 0 on success, -1 on failure.
extern int clear_bg_processes(void);

// int remove_bg_process(pid_t)
// Description: Removes a process id from the bg_processes struct.
// Preconditions: bg_processes struct is initialized.
// Postconditions: The process id is removed from the array of background processes.
// Return: 0 on success, -1 on failure.
extern int remove_bg_process(pid_t);

// int remove_dead_processes()
// Description: Removes dead processes from the bg_processes struct.
// Preconditions: bg_processes struct is initialized.
// Postconditions: Dead processes are removed from the array of background processes.
// Return: 0 on success, -1 on failure.
extern int remove_dead_processes(void);

// int set_up_bg_processes()
// Description: Initializes defaults for the bg_processes struct.
// Preconditions: bg_processes struct exists is allocated.
// Postconditions: The bg_processes struct members are initialized.
// Return: 0 on success, -1 on failure.
extern int set_up_bg_processes(void);

#ifdef __cplusplus
}
#endif




#endif // BG_UTILS_H