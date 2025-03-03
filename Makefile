CC = gcc
CFLAGS = -g -Wall -O0 -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes -s

TARGET = simple_shell
SOURCES = main.c utils.c history_utils.c shell_commands.c bg_utils.c
OBJECTS = $(SOURCES:.c=.o)

TESTING_TEXT_FILE = text.txt
HISTORY_FILE = .421sh

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	echo 'These are the contents of $(TESTING_TEXT_FILE)' > ${TESTING_TEXT_FILE}
	echo 'End of file' >> ${TESTING_TEXT_FILE}
	rm -f $(OBJECTS)

main.o: main.c utils.o history_utils.o shell_commands.o bg_utils.o
	$(CC) $(CFLAGS) -c main.c $(LDFLAGS)

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c $(LDFLAGS)

history_utils.o: history_utils.c history_utils.h
	$(CC) $(CFLAGS) -c history_utils.c $(LDFLAGS)

bg_utils.o: bg_utils.c bg_utils.h
	$(CC) $(CFLAGS) -c bg_utils.c $(LDFLAGS)

shell_commands.o: shell_commands.c shell_commands.h bg_utils.o
	$(CC) $(CFLAGS) -c shell_commands.c $(LDFLAGS)


.PHONY: run val clean

run:
	./$(TARGET)

val:
	valgrind $(VALGRIND_FLAGS) ./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS) ${TESTING_TEXT_FILE} ${HISTORY_FILE} core


