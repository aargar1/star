CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g

TARGET = star
SRCS = star.c gen.c lex.c parse.c queue.c tree.c util.c
OBJS = $(SRCS:.c=.o)

HEADERS = gen.h lex.h parse.h queue.h tree.h util.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	mv star ..
	rm -rf $(OBJS) $(TARGET)

.PHONY: all clean