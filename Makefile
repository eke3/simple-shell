CC = gcc
CFLAGS = -Wall -std=c99 -D_POSIX_C_SOURCE=200809L

# Target executable
TARGET = simple_shell

OBJECTS = main.o utils.o history_utils.o shell_commands.o

TESTING_TEXT_FILE = text.txt
HISTORY_FILE = .421sh

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)
	echo 'These are the contents of $(TESTING_TEXT_FILE)' > ${TESTING_TEXT_FILE}
	echo 'End of file' >> ${TESTING_TEXT_FILE}
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
	rm -f $(TARGET) $(OBJECTS) ${TESTING_TEXT_FILE} ${HISTORY_FILE}


