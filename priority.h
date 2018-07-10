/*
* Jordan McCaskill
* 1722645
* CMPT 360 Assignment 3
*/
#include "queue.c"
#include <semaphore.h>

// Priority sorter struct
struct prioritySorter {
	struct head *readers;
	struct head *writers;
	int count;
	int next;
};

int readNum;
sem_t count;
sem_t mutread;
sem_t mutwrite;
sem_t mutnewreads;

//Function prototypes

void initialize(struct prioritySorter *jobs);
void addReader(struct prioritySorter *jobs, char *filename);
void addWriter(struct prioritySorter *jobs, char *filename);
int nextItem(struct prioritySorter *jobs);
int process(struct prioritySorter *jobs);
void execute(struct prioritySorter *jobs);
void *writer(void *argum);
void *reader(void *argum);
char *getFilename(struct prioritySorter *jobs);
void checkSuccess(char *filename);
void destroyPriority(struct prioritySorter *sorter);
