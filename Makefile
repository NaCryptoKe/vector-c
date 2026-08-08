CC = gcc
CFLAGS = -Wall -Wextra -std=c11

.PHONY: all clean

all: app
	bin/$<

main.o: include/vector.h
vector.o: include/vector.h

OBJS = obj/main.o obj/vector.o

app: $(OBJS)
	$(CC) $(CFLAGS) $^ -o bin/$@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f bin/* obj/*