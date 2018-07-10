# Flags for compiling
CC = gcc
CFLAGS = -Wall -std=c99 -g

# Compile all programs
all: tester

tester: tester.c tester.h
	$(CC) -Wall tester.c -o tester -lpthread -g

clean:
	rm tester *~ *.o *.txt
