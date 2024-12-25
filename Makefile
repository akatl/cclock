COMMON_CFLAGS=		-Wall -Wextra -std=c11 -pedantic

build: main.c
	$(CC) $(COMMON_CFLAGS) main.c -o cclock -lncurses

clean:
	rm ./cclock
