CC = gcc
CFLAGS = -Wall -Wextra -Wpointer-arith -std=c11

.PHONY: all clean

all: app

main.o: include/vector.h
vector.o: include/vector.h

OBJS = obj/main.o obj/vector.o

app: $(OBJS)
	$(CC) $(CFLAGS) $^ -o bin/$@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f bin/* obj/*