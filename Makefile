CC = gcc
CFLAGS = -Wall

# Target executable
TARGET = simple_shell

OBJECTS = main.o utils.o history_utils.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) && rm -f $(OBJECTS)

main.o: main.c utils.h history_utils.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

history_utils.o: history_utils.c history_utils.h
	$(CC) $(CFLAGS) -c history_utils.c

run:
	./$(TARGET)

val:
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(TARGET)

clean_o:
	rm -f $(OBJECTS)

clean:
	rm -f $(TARGET) $(OBJECTS)


