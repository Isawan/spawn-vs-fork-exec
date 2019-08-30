.PHONY: all
all: spawn fork

spawn: main.c
	gcc -Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE=199309L -Dtime_spawn -O2 main.c -o spawn
fork: main.c
	gcc -Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE=199309L -Dtime_fork -O2 main.c -o fork
