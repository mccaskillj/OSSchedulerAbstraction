/*
* Jordan McCaskill
* 1722645
* CMPT 360 Assignment 3
*/
#include "priority.h"

/*
* Function is used to initialize everything in the structs as well as
* the semaphores used
*/
void initialize(struct prioritySorter *jobs)
{
	jobs->readers = malloc(sizeof(struct head));
	jobs->readers->first = NULL;
	jobs->writers = malloc(sizeof(struct head));
	jobs->writers->first = NULL;
	jobs->count = 1;
	jobs->next = -1;

	sem_init(&count, 0, 1);
	sem_init(&mutread, 0, 0);
	sem_init(&mutwrite, 0, 0);
	sem_init(&mutnewreads, 0, 0);
}

/*
* Function to add a reader to the reader queue
*/
void addReader(struct prioritySorter *jobs, char *filename)
{
	append(jobs->readers, jobs->count, filename);
	jobs->count++;
	if (jobs->next == -1) {
		sem_post(&mutread);
		sem_post(&mutnewreads);
		jobs->next = 1;
	}
}

/*
* Function to add a writer to the writer queue
*/
void addWriter(struct prioritySorter *jobs, char *filename)
{
	append(jobs->writers, 0, filename);
	jobs->count = 1;

	if (jobs->next == -1) {
		sem_post(&mutread);
		sem_post(&mutwrite);
		jobs->next = 0;
	}
}

/*
* Function to check if the lists in the reader and writer lists are empty
*/
int isEmpty(struct prioritySorter *jobs)
{
	if (check(jobs->readers) != -1)
		return 1;
	if (check(jobs->writers) != -1)
		return 1;
	return 0;
}

/*
* Function to check what the next item is that is coming through
* 0 is for writers, 1+ for readers and -1 for nothing
*/
int nextItem(struct prioritySorter *jobs)
{
	return jobs->next;
}

/*
* Function to process the next available job and determine what is
* the next thing to be processed, this is the main scheduler loop
*/
int process(struct prioritySorter *jobs)
{
	int next = jobs->next;

	if (jobs->next == -1) {
		return -1;
	} else if (jobs->next == 0) {
		pop(jobs->writers);
		if (check(jobs->readers) != -1) {
			sem_post(&mutnewreads);
			jobs->next++;
		} else if (check(jobs->writers) != -1) {
			sem_post(&mutwrite);
			jobs->next = 0;
		} else
			jobs->next = -1;
	} else {
		pop(jobs->readers);
		if (check(jobs->readers) > 1) {
			sem_post(&mutnewreads);
			jobs->next++;
		} else if (check(jobs->writers) != -1)
			jobs->next = 0;
		else
			jobs->next = -1;
	}
	return next;
}

/*
* Starts the execution of the server and the processing of jobs
* once all the tests have been added to the queue
*/
void execute(struct prioritySorter *jobs)
{
	int procReturn;
	char *filename;

	int thrNum = 26;
	int countW = 0;
	int countR = 0;
	int i = 0;
	pthread_t *threads = calloc(thrNum, sizeof(threads));

	while (nextItem(jobs) != -1) {
		filename = getFilename(jobs);
		procReturn = process(jobs);
		if (procReturn == 0) {
			sem_wait(&mutread);
			sem_wait(&mutwrite);
			printf("writerENTER\n");
			pthread_create(&threads[i], NULL,
				writer, (void *)filename);
			countW++;
			i++;
		} else if (procReturn > 0) {
			if (readNum == 0)
				sem_wait(&mutread);
			sem_wait(&count);
			readNum++;
			sem_post(&count);
			sem_wait(&mutnewreads);
			printf("readerENTER\n");
			pthread_create(&threads[i], NULL,
				reader, (void *) filename);
			countR++;
			i++;
		}
	}

	for (i = 0; i < thrNum; ++i)
		pthread_join(threads[i], NULL);

	free(threads);
}

/*
* Thread loop for writers which takes a filename and writes to main.txt
*/
void *writer(void *argum)
{
	FILE *fd;
	FILE *testFile;
	char c;

	char *filename = argum;

	testFile = fopen(filename, "r");
	fd = fopen("main.txt", "w");

	c = fgetc(testFile);

	while (c != EOF) {
		fputc(c, fd);
		c = fgetc(testFile);
	}

	printf("writerEXIT\n");

	fclose(fd);
	fclose(testFile);
	remove(filename);

	free(filename);

	sem_post(&mutread);

	pthread_exit(NULL);
}

/*
* Thread loop for readers which takes a filename and writes to it from main.txt
*/
void *reader(void *argum)
{
	char out;

	char *filename = argum;
	FILE *readFile = fopen("main.txt", "r");
	FILE *writeFile = fopen(filename, "w");

	out = fgetc(readFile);

	while (out != EOF) {
		fputc(out, writeFile);
		out = fgetc(readFile);
	}

	fclose(readFile);
	fclose(writeFile);

	checkSuccess(filename);

	printf("reader EXIT\n");

	sem_wait(&count);
	readNum--;
	sem_post(&count);
	if (readNum == 0) {
		sem_post(&mutwrite);
		sem_post(&mutread);
	}

	remove(filename);
	free(filename);

	pthread_exit(NULL);
}

/*
* Gets the filename associated with the next item to be processed
*/
char *getFilename(struct prioritySorter *jobs)
{
	char *filename;

	int next = nextItem(jobs);

	if (next == -1) {
		return NULL;
	} else if (next == 0) {
		filename = malloc(sizeof(char) * 15);

		strcpy(filename, jobs->writers->first->filename);
	} else {
		filename = malloc(sizeof(char) * 15);

		strcpy(filename, jobs->readers->first->filename);
	}
	return filename;
}

/*
* Checks if writing the files was successful
*/
void checkSuccess(char *filename)
{
	FILE *fp = fopen(filename, "r");
	int count = 0;
	char c = fgetc(fp);
	char first = c;

	while (c != EOF) {
		if (c != first)
			printf("your file has incorrect content\n");
		count++;
		c = fgetc(fp);
	}
	if (count == (first - 'a' + 1) * 1000)
		printf("your file has correct content\n");
	else
		printf("your file has incorrect content\n");

	fclose(fp);
}

/*
* destroys the struct at the end of execution
*/
void destroyPriority(struct prioritySorter *sorter)
{
	destroy(sorter->readers);
	destroy(sorter->writers);
	free(sorter);
	sem_destroy(&mutread);
	sem_destroy(&mutwrite);
	sem_destroy(&mutnewreads);
}
