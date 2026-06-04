CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c99 -O2
LDFLAGS = -lwiringPi

.PHONY: all blink clean

all: ad9854_test

ad9854_test: main.c ad9854.c ad9854.h
	$(CC) $(CFLAGS) -o $@ main.c ad9854.c $(LDFLAGS)

blink: blink.c
	$(CC) $(CFLAGS) -o blink blink.c $(LDFLAGS)

clean:
	rm -f ad9854_test blink
