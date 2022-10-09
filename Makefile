COMMON_CFLAGS=		-Wall -Wextra -std=c11 -pedantic -lncurses
PREFIX?=		/usr/local
INSTALL?=		install

build: main.c
	$(CC) $(COMMON_CFLAGS) main.c -o cclock 

clean:
	rm ./cclock

