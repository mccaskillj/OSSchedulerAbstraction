/*
* Jordan McCaskill
* 1722645
* CMPT 360 Assignment 3
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "priority.c"

// Function prototypes
void resetFiles(void);
void printFile(void);
void makeTester(char *c, char *filename);
