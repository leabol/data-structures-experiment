CC = gcc
CFLAGS = -Wall -g

TARGET = main
SRCS = main.c pri_queue.c read_file.c huffman.c
OBJS = $(SRCS:.c=.o)
HEADERS = pri_queue.h read_file.h sort.h huffman.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)