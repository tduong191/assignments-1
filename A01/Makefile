CC=gcc
% :: %.c
	$(CC) -g $< -o $@

cypher : cypher.c 
	$(CC) -g -Wall -Wvla -Werror cypher.c -o $@

restaurants : restaurants.c 
	$(CC) -g -Wall -Wvla -Werror restaurants.c -o $@

wordle : wordle.c 
	$(CC) -g -Wall -Wvla -Werror wordle.c -o $@

all : cypher restaurants wordle

clean :
	rm cypher restaurants wordle 2> /dev/null

