#include "bg_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int append_bg_process(pid_t process_id) {
  if (process_id <= 0) {
    // Process not active.
    return 0;
  }

  if (bg_processes == NULL) {
    // Global struct not initialized.
    return CLEAR_BG_FAILURE;
  }

  if (bg_processes->num_processes >= bg_processes->capacity) {
    // Array is full, allocate more memory.
    size_t old_capacity = bg_processes->capacity;
    bg_processes->capacity *= 2;

    pid_t* temp_pids = realloc(bg_processes->process_ids,
                               (sizeof(pid_t) * bg_processes->capacity));

    if (temp_pids == NULL) {
      perror("realloc error in append_bg_process()");
      return CLEAR_BG_FAILURE;
    }
    bg_processes->process_ids = temp_pids;

    for (int i = old_capacity; i < bg_processes->capacity; i++) {
      bg_processes->process_ids[i] = DEAD_PROCESS_ID;
    }
  }

  // Add process id to array.
  int i = 0;
  while (bg_processes->process_ids[i] != DEAD_PROCESS_ID) {
    i++;
  }
  bg_processes->process_ids[i] = process_id;
  bg_processes->num_processes++;

  return 0;
}

int clear_bg_processes(void) {
  if (bg_processes == NULL) {
    // Global struct not initialized.
    return CLEAR_BG_FAILURE;
  }

  // Reset and free array of process ids.
  for (int i = 0; i < bg_processes->capacity; i++) {
    bg_processes->process_ids[i] = DEAD_PROCESS_ID;
  }

  free(bg_processes->process_ids);
  bg_processes->num_processes = 0;
  return 0;
}

int remove_bg_process(pid_t process_id) {
  if (bg_processes == NULL || bg_processes->process_ids == NULL) {
    // Global struct or process array not initialized.
    return REMOVE_BG_FAILURE;
  }

  // Find and remove process id from array.
  for (int i = 0; i < bg_processes->capacity; i++) {
    if (bg_processes->process_ids[i] == process_id) {
      bg_processes->process_ids[i] = DEAD_PROCESS_ID;
      bg_processes->num_processes--;
      return 0;
    }
  }

  // Process id not found.
  return REMOVE_BG_FAILURE;
}

int remove_dead_processes(void) {
  if (bg_processes == NULL || bg_processes->process_ids == NULL) {
    // Global struct or process array not initialized.
    return CLEAR_BG_FAILURE;
  }

  for (int i = 0; i < bg_processes->capacity; i++) {
    // Check if process is still active.
    if ((waitpid(bg_processes->process_ids[i], NULL, WNOHANG) != 0)) {
      // Process is no longer active. Set id to default value.
      bg_processes->process_ids[i] = DEAD_PROCESS_ID;
      bg_processes->num_processes--;
    }
  }

  return 0;
}

int set_up_bg_processes(void) {
  if (bg_processes == NULL) {
    // Global struct not initialized.
    return SETUP_FAILURE;
  }

  // Initialize members and allocate array of process ids with values
  // initialized to 0.
  if ((bg_processes->process_ids =
           calloc((bg_processes->capacity = 10), sizeof(pid_t))) == NULL) {
    perror("calloc error in set_up_bg_processes()");
    return SETUP_FAILURE;
  }
  bg_processes->num_processes = 0;

  return 0;
}
