CC = gcc
CFLAGS = -Wall

# Target executable
TARGET = main

SOURCES = main.c utils.c

main: $(SRCS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run:
	./$(TARGET)

val:
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(TARGET)

clean:
	rm -f $(TARGET)
