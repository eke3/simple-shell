CC = gcc
CFLAGS = -Wall

# Target executable
TARGET = simple_shell

OBJECTS = main.o utils.o history_utils.o shell_commands.o

LS_TEST_SCRIPT = run_ls.sh
HISTORY_FILE = .421sh

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)
	echo 'ls --color=auto' > ${LS_TEST_SCRIPT}
	echo 'whoami' >> run_ls.sh
	chmod +x ${LS_TEST_SCRIPT}
	rm -f $(OBJECTS)

main.o: main.c utils.h history_utils.h shell_commands.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

history_utils.o: history_utils.c history_utils.h
	$(CC) $(CFLAGS) -c history_utils.c

shell_commands.o: shell_commands.c shell_commands.h
	$(CC) $(CFLAGS) -c shell_commands.c

run:
	./$(TARGET)

val:
	valgrind ./$(TARGET)

test:
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS) ${LS_TEST_SCRIPT} ${HISTORY_FILE}


