CC=gcc
% :: %.c
	$(CC) -g -Wall -Wvla -Werror $< -o $@

all: memstats unit_tests

memstats: memstats.c mylloc_list.c sbrk.c rand.c
	$(CC) -g -Wall -Wvla -Werror memstats.c mylloc_list.c sbrk.c rand.c -o $@ -lm

unit_tests: unit_tests.c mylloc_list.c sbrk.c rand.c
	$(CC) -g -Wall -Wvla -Werror unit_tests.c mylloc_list.c sbrk.c rand.c -o $@ -lm

clean: 
	rm memstats unit_tests

