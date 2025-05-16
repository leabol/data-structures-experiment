CC = gcc
CFLAGS = -Wall -g

TARGET = main
SRCS = adj_graph.c  pri_queue.c queue.c
OBJS = $(SRCS:.c=.o)
HEADERS = pri_queue.h queue.h 

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)