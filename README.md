# CMSC 421 Project 1

## Introduction
**Platforms** - Linux

This project is a simple custom shell designed to provide basic command-line functionality similar to standard Unix shells. The shell supports executing built-in and external commands and running background processes. The primary goals of this project include implementing a user-friendly shell interface, supporting fundamental shell commands and shell executions, and handling process creation and management.

### Features
* Handles and parses commands of any length
* Error handling for unsupported command-line arguments
* Command execution using absolute paths, relative paths, and system `$PATH`
* Built-in `exit` command to terminate shell
* Built-in `/proc` command to display file content from the proc filesystem
* Built-in `history` command to display the last ten commands entered in the session
* Memory management to prevent leaks and errors

**Extra Credit Features**
* Background process execution by passing `&` as the last argument to a command
* Built-in `cd` command to change current working directory in the shell session
* Signal handling to respond to the Ctrl+C interrupt without terminating
* User-configurable shell prompt via built-in command `prompt`
* Built-in `jobs` command to display active background processes
* Built-in `fg` command to bring a background process to the foreground
* Detailed error messaging/handling

## Contact
* **Contributor** - Eric Ekey
* **Student ID** - XR06051
* **Email** - eekey1@umbc.edu

## Installation and Setup
### Setup

Install up-to-date dependencies for building this program and going through the demo:

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

This demonstration shows that the shell will not start with extra command line arguments. It shows that commands can be executed using relative paths, absolute paths, and system `$PATH` with one or multiple arguments. It shows that the `cd` command allows the shell to change the current working directory. It shows that the `history` and `exit` commands are functional and only execute without additional command liune arguments.

```bash
eric@fedora:~/Documents/2024-25 Spring/cmsc421/Projects/proj01$ make
gcc -Wall -c main.c
gcc -Wall -c utils.c                       
gcc -Wall -c history_utils.c
gcc -Wall -c shell_commands.c
gcc -Wall main.o utils.o history_utils.o shell_commands.o -o simple_shell
echo 'These are the contents of text.txt' > text.txt
echo 'End of file' >> text.txt
rm -f main.o utils.o history_utils.o shell_commands.o
eric@fedora:~/Documents/2024-25 Spring/cmsc421/Projects/proj01$ ./simple_shell extra_arg
Usage: ./simple_shell   Command line arguments are not supported.
eric@fedora:~/Documents/2024-25 Spring/cmsc421/Projects/proj01$ make run
./simple_shell
/home/eric/Documents/2024-25 Spring/cmsc421/Projects/proj01$ cat text.txt
These are the contents of text.txt
End of file
/home/eric/Documents/2024-25 Spring/cmsc421/Projects/proj01$ /usr/bin/cat text.txt
These are the contents of text.txt
End of file
/home/eric/Documents/2024-25 Spring/cmsc421/Projects/proj01$ cd .. ..
Usage: cd [directory]   Too many arguments.
Error changing directory.
/home/eric/Documents/2024-25 Spring/cmsc421/Projects/proj01$ cd /usr/bin
/usr/bin$ ./cat /home/eric/Documents/2024-25\ Spring/cmsc421/Projects/proj01/text.txt
These are the contents of text.txt
End of file
/usr/bin$ cd
/home/eric$ exit 0
Usage: exit     Additional arguments are not supported.
/home/eric$ exit 0
Usage: exit     Additional arguments are not supported.
/home/eric$ exit 0
Usage: exit     Additional arguments are not supported.
/home/eric$ history 0
Usage: history  Additional arguments are not supported.
/home/eric$ eleventh_command
/home/eric$ history
[1]     /usr/bin/cat text.txt
[2]     cd .. ..
[3]     cd /usr/bin
[4]     ./cat /home/eric/Documents/2024-25\ Spring/cmsc421/Projects/proj01/text.txt
[5]     cd
[6]     exit 0
[7]     exit 0
[8]     exit 0
[9]     history 0
[10]    eleventh_command
/home/eric$ exit
eric@fedora:~/Documents/2024-25 Spring/cmsc421/Projects/proj01$ 

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
* When a child process attempts to execute a command and fails, no error message will be displayed. I chose to omit the error message because it was appearing after the next shell prompt, instead of before. This can be observed when trying to run `/proc` with no additional valid arguments or trying to execute a program that does not exist (e.g., `misspelledcommand`).

* When a command is entered by the user that is not recognized by the shell, no error message is produced. For the same reason as the above issue.

## References
### External Resources
* [Programiz](https://www.programiz.com/)
* [GeeksForGeeks](https://www.geeksforgeeks.org/)
* [cppreference](https://en.cppreference.com/) 
* [tutorialspoint](https://www.tutorialspoint.com/)
* [man7](https://www.man7.org/)