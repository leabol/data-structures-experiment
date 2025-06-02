.PHONY: all clean run

CC = gcc
CFLAGS = -Wall -g

TARGET = sort_system
INCLUDE = -I./include
SRC = src/main.c src/sort_algorithms.c

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(INCLUDE) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
