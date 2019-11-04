CC=gcc
CFLAGS=-Wall -Werror `pkg-config fuse3 --cflags --libs`

SRCS=hello.c

all:
	$(CC) $(SRCS) $(CFLAGS) -o hello

clean:
	rm -f $(OBJS)
