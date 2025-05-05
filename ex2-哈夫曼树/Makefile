CC = gcc
CFLAGS = -Wall -g

TARGET = main
SRCS = test.c pri_queue.c read_file.c
OBJS = $(SRCS:.c=.o)
HEADERS = 

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)