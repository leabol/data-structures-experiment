CC = gcc
CFLAGS = -Wall -g

TARGET = main
SRCS = main.c poly_arr.c poly_list.c input.c array.c
OBJS = $(SRCS:.c=.o)
HEADERS = poly_arr.h poly_list.h input.h array.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)