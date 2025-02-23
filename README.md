# CMSC 421 Project 1

## Introduction
**Platforms** - Linux

This project is a simple custom shell designed to provide basic command-line functionality similar to standard Unix shells. The shell supports executing built-in and external commands and running background processes. The primary goals of this project include implementing a user-friendly shell interface, supporting fundamental shell commands and shell executions, and handling process creation and management.

### Features
* Handles and parsed commands of any length
* Error handling for unsupported command-line arguments
* Command execution using absolute paths, relative paths, and system `$PATH`
* Built-in `exit` command to terminate shell
* Built-in `/proc` command to display file content from the proc filesystem
* Built-in `history` command to display the last ten commands entered in the session
* Memory management to prevent leaks and errors
* Background process execution by passing `&` as the last argument to a command
* Built-in `cd` command to change current working directory in the shell session
* Signal handling to respond to the Ctrl+C interrupt without terminating
* Support for executing simple shell scripts in sequence
* User-configurable shell prompt via built-in command `prompt`

## Contact
* **Contributor** - Eric Ekey
* **Student ID** - XR06051
* **Email** - eekey1@umbc.edu

## Installation and Setup
### Setup

Install up-to-date dependencies for building this program:

**Debian**
```bash
sudo apt update
sudo apt install gcc make valgrind gedit coreutils
```

### Build and Compile
Compile the shell executable using the Makefile:
```bash
make
```
Run the shell executable using the provided makefile:
```bash
make run
```
or run using Valgrind for memory error detection:
```bash
make val
```

## Testing Strategy
### Test Cases
**Testing Command Execution**

This demonstration shows that the shell will not start with extra command line arguments. It shows that commands can be executed using relative paths, absolute paths, and system `$PATH` with one or multiple arguments. It shows that the `cd` command allows the shell to change the current working directory. It shows that the shell can run simple bash scripts, and that the `history` and `exit` commands are functional.

```bash
eric@fedora:~/Documents/2024-25 Spring/cmsc421/proj01$ make
gcc -Wall -c main.c
gcc -Wall -c utils.c
gcc -Wall -c history_utils.c
gcc -Wall -c shell_commands.c
gcc -Wall main.o utils.o history_utils.o shell_commands.o -o simple_shell
echo 'ls --color=auto' > run_ls.sh
echo 'whoami' >> run_ls.sh
chmod +x run_ls.sh
rm -f main.o utils.o history_utils.o shell_commands.o
eric@fedora:~/Documents/2024-25 Spring/cmsc421/proj01$ ./simple_shell extra_arg
Usage: ./simple_shell   Command line arguments are not supported.
eric@fedora:~/Documents/2024-25 Spring/cmsc421/proj01$ make run
./simple_shell
/home/eric/Documents/2024-25 Spring/cmsc421/proj01$ ls
history_utils.c  main.c    README.md  shell_commands.c  simple_shell  utils.h
history_utils.h  Makefile  run_ls.sh  shell_commands.h  utils.c
/home/eric/Documents/2024-25 Spring/cmsc421/proj01$ /bin/cat run_ls.sh
ls --color=auto
whoami
/home/eric/Documents/2024-25 Spring/cmsc421/proj01$ ./run_ls.sh
history_utils.c  main.c    README.md  shell_commands.c  simple_shell  utils.h
history_utils.h  Makefile  run_ls.sh  shell_commands.h  utils.c
eric
/home/eric/Documents/2024-25 Spring/cmsc421/proj01$ cd .. ..
Usage: cd [directory]   Too many arguments.
Error changing directory.
/home/eric/Documents/2024-25 Spring/cmsc421/proj01$ cd ../..
/home/eric/Documents/2024-25 Spring$ exit 0
Usage: exit     Additional arguments are not supported.
/home/eric/Documents/2024-25 Spring$ history
[1]     ls
[2]     /bin/cat run_ls.sh
[3]     ./run_ls.sh
[4]     cd .. ..
[5]     cd ../..
[6]     exit 0
/home/eric/Documents/2024-25 Spring$ exit
eric@fedora:~/Documents/2024-25 Spring/cmsc421/proj01$ 
```

**Testing Proc Command**

The `/proc` command displays the contents of a file in the proc filesystem. You can confirm its functionality by entering a proc filepath. For example, by entering the path to `cpuinfo`:
```bash
/proc /cpuinfo
```
OR
```bash
/proc/cpuinfo
```

**Testing Interrupt Handling, Background Processes, and Shell Prompt**

This demonstration shows that the shell will not terminate when it detects a Ctrl+C interrupt. It shows that a process (`gedit`) can be started in the background without pausing further shell prompts by using `&` as the last argument in the command. It shows that the user can change the shell prompt with the built-in command `prompt`.

```bash
eric@fedora:~/Documents/2024-25 Spring/cmsc421/proj01$ make run
./simple_shell
/home/eric/Documents/2024-25 Spring/cmsc421/proj01$ ^C
Interrupt ignored. Type `exit` to quit.
/home/eric/Documents/2024-25 Spring/cmsc421/proj01$ gedit &
/home/eric/Documents/2024-25 Spring/cmsc421/proj01$ prompt !!!
/home/eric/Documents/2024-25 Spring/cmsc421/proj01!!! 
```

## Troubleshooting
### Known Issues
* When a child process attempts to execute a command and fails, no error message will be displayed. I chose to omit the error message because it was appearing after the next shell prompt, instead of before. I could not figure out how to get the parent process to react to a child process exiting with a failure code. This can be observed when trying to run `/proc` with no additional valid arguments or trying to execute a program that does not exist (e.g., `misspelledcommand`).

## References
### External Resources
* [Programiz](https://www.programiz.com/)
* [GeeksForGeeks](https://www.geeksforgeeks.org/)
* [cppreference](https://en.cppreference.com/) 
