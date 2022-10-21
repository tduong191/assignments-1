CC=gcc
% :: %.c
	$(CC) -g -Wall -Wvla -Werror $< -o $@

test_write_ppm : test_write_ppm.c read_ppm.c write_ppm.c
	$(CC) -g -Wall -Wvla -Werror read_ppm.c write_ppm.c $< -o $@

test_write_ppm_2d : test_write_ppm_2d.c read_ppm.c write_ppm.c
	$(CC) -g -Wall -Wvla -Werror read_ppm.c write_ppm.c $< -o $@

glitch : glitch.c read_ppm.c write_ppm.c
	$(CC) -g -Wall -Wvla -Werror read_ppm.c write_ppm.c $< -o $@

all : test_write_ppm test_write_ppm_2d bitmap glitch

clean :
	rm test_write_ppm test_write_ppm_2d bitmap glitch 2> /dev/null
