CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm -fsanitize=address
FILES=btree.c ../btree.c ../test_util.c ../test.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)

clean:
	rm -f test
