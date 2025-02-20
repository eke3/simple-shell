/*
project: 01
author: 
email: 
student id: 
due date: 2/28/21 11:59:59 pm
description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
#include "utils.h"

#define GETLINE_FAILURE -1
#define EXIT_CMD "exit"
#define PROC_CMD "/proc"


/*
In this project, you are going to implement a number of functions to 
create a simple linux shell interface to perform basic linux commands
*/


// DEFINE THE FUNCTION PROTOTYPES
void user_prompt_loop();
char* get_user_command();
char** parse_command(char*);
// execute_command()


int main(int argc, char **argv) {
    /*
    Write the main function that checks the number of argument passed to ensure 
    no command-line arguments are passed; if the number of argument is greater 
    than 1 then exit the shell with a message to stderr and return value of 1
    otherwise call the user_prompt_loop() function to get user input repeatedly 
    until the user enters the "exit" command.
    */

    // Check for command-line arguments.
    if (argc > 1) {
        // Throw error if arguments are passed.
        fprintf(stderr, "Usage: %s\tCommand line arguments are not supported.\n", argv[0]);
        return 1;
    } else {
        // Start program by calling the user_prompt_loop() function.
        user_prompt_loop();
    }

    return 0;
}



void user_prompt_loop() {
    char* cmd = "";
    
    while (true) {
        printf(">> ");
        cmd = get_user_command(); // needs free()
        char** parsed_cmd = parse_command(cmd); // needs free()

        // First argument is "exit".
        if (strcmp(parsed_cmd[0], EXIT_CMD) == 0) {
            if (parsed_cmd[1] != NULL) {
                // Invalid exit command.
                fprintf(stderr, "Usage: exit\tAdditional arguments are not supported.\n");
            } else {
                // Valid exit command.
                free(cmd);
                for (int i = 0; parsed_cmd[i] != NULL; i++) {
                    free(parsed_cmd[i]);
                }
                free(parsed_cmd);
                exit(0);
            }
        }

        // First argument is "/proc"
        if (strcmp(parsed_cmd[0], PROC_CMD) == 0) {
            if (parsed_cmd[1] == NULL) {
                // Invalid /proc command.
                fprintf(stderr, "Usage: /proc /[filepath]\tNot enough arguments.\n");
            } else {
                // Valid /proc command.
            }
        }


            // 5. if the first element is "/proc" then you have the open the /proc file system 
            //    to read from it
            //     i) concat the full command:
            //         Ex: user input >>/proc /process_id_no/status
            //             concated output: /proc/process_id_no/status
            //     ii) read from the file line by line. you may user fopen() and getline() functions
            //     iii) display the following information according to the user input from /proc
            //         a) Get the cpu information if the input is /proc/cpuinfo
            //         - Cpu Mhz
            //         - Cache size
            //         - Cpu cores
            //         - Address sizes
            //         b) Get the number of currently running processes from /proc/loadavg
            //         c) Get how many seconds your box has been up, and how many seconds it has been idle from /proc/uptime
            //         d) Get the following information from /proc/process_id_no/status
            //         - the vm size of the virtual memory allocated the vbox 
            //         - the most memory used vmpeak 
            //         - the process state
            //         - the parent pid
            //         - the number of threads
            //         - number of voluntary context switches
            //         - number of involuntary context switches
            //         e) display the list of environment variables from /proc/process_id_no/environ
            //         f) display the performance information if the user input is /proc/process_id_no/sched
            // 7. otherwise pass the parsed command to execute_command() function 
    
        /*
        Functions you may need: 
            get_user_command(), parse_command(), execute_command(), strcmp(), strcat(), 
            strlen(), strncmp(), fopen(), fclose(), getline(), isdigit(), atoi(), fgetc(), 
            or any other useful functions
        */
    
       free(cmd);
       for (int i = 0; parsed_cmd[i] != NULL; i++) {
           free(parsed_cmd[i]);
       }
       free(parsed_cmd);
    }
}

/*
user_prompt_loop():
Get the user input using a loop until the user exits, prompting the user for a command.
Gets command and sends it to a parser, then compares the first element to the two
different commands ("/proc", and "exit"). If it's none of the commands, 
send it to the execute_command() function. If the user decides to exit, then exit 0 or exit 
with the user given value. 
*/

/*user_prompt_loop()*/
// {
    // initialize variables

    /*
    loop:
        1. prompt the user to type command by printing >>
        2. get the user input using get_user_command() function 
        3. parse the user input using parse_command() function 
        Example: 
            user input: "ls -la"
            parsed output: ["ls", "-la", NULL]

// }


get_user_command():
Take input of arbitrary size from the user and return to the user_prompt_loop()
*/

char* get_user_command() {
    /*
    Functions you may need: 
        malloc(), realloc(), getline(), fgetc(), or any other similar functions
    */

    // Initialize variables.
    char* user_command = NULL;
    size_t buffer_size = 0;
    ssize_t command_length = -1;

    // Dynamically allocate memory for the user command from stdin.
    command_length = getline(&user_command, &buffer_size, stdin);

    // Do nothing if getline fails.
    if (command_length == GETLINE_FAILURE) {
        perror("getline error in get_user_command()");
        free(user_command);
        user_command = "";
    }

    // Remove trailing newline character if getline succeeds.
    if (user_command[command_length - 1] == '\n') {
        user_command[command_length - 1] = '\0';
    }

    return user_command;
}

/*
parse_command():
Take command grabbed from the user and parse appropriately.
Example: 
    user input: "ls -la"
    parsed output: ["ls", "-la", NULL]
Example: 
    user input: "echo     hello                     world  "
    parsed output: ["echo", "hello", "world", NULL]
*/

char** parse_command(char* user_command) {
    /*
    Functions you may need: 
        malloc(), realloc(), free(), strlen(), first_unquoted_space(), unescape()
    */

    // Initialize variables.
    size_t arg_capacity = 10;
    size_t arg_count = 0;
    size_t command_length = strlen(user_command);
    int start_pos = 0;
    char** parsed_command = malloc(arg_capacity * sizeof(char*)); 

    // Skip leading whitespaces.
    while ((start_pos < command_length) && isspace((unsigned char) user_command[start_pos])) {
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
            char** temp_parsed_cmd = realloc(parsed_command, arg_capacity * sizeof(char*));
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
        int space_pos = (relative_space_pos == -1) ? -1 : start_pos + relative_space_pos;
        
        if (space_pos == -1) {
            // If there are no spaces after the current start position, add the rest of the command as the last argument.
            parsed_command[arg_count] = strndup(user_command + start_pos, command_length - start_pos);
            start_pos = command_length;
        } else {
            // Add the argument between the current start position and the next space as the next argument.
            parsed_command[arg_count] = strndup(user_command + start_pos, space_pos - start_pos);
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
        // printf("parsed_command[%d] = %s\n", i, parsed_command[i]); // FOR TESTING
    }

    return parsed_command;
}

/*
execute_command():
Execute the parsed command if the commands are neither /proc nor exit;
fork a process and execute the parsed command inside the child process
*/

/*execute_command()*/
// {
    /*
    Functions you may need: 
        fork(), execvp(), waitpid(), and any other useful function
    */

    /*
    ENTER YOUR CODE HERE
    */
// }
