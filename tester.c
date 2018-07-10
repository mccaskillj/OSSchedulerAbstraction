/*
* Jordan McCaskill
* 1722645
* CMPT 360 Assignment 3
*/
#include "tester.h"

/*
* Main testing function for the program, deals with the random creation of
* jobs and their addition to the queue
*/
int main(int argc, char *argv[])
{
	int i;
	int rando;
	char *filename;
	int thrNum = 26;
	int readInd = 0;
	int writeInd = 0;
	// There is a checkpatch warning here but it wont compile if I change it
	// to a const
	char *letters[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i",
	"j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
	"u", "v", "w", "x", "y", "z"};
	struct prioritySorter *sorter = malloc(sizeof(struct prioritySorter));

	initialize(sorter);
	resetFiles();

	srand(8);
	for (i = 0; i < thrNum; i++) {
		rando = rand() % 2;
		if (rando == 0) {
			filename = malloc(15 * sizeof(char));
			strcpy(filename, "");
			strcat(filename, "output");
			strcat(filename, letters[readInd]);
			strcat(filename, ".txt");
			addReader(sorter, filename);
			readInd++;
		} else {
			filename = malloc(15 * sizeof(char));
			strcpy(filename, "");
			strcat(filename, "testfile");
			strcat(filename, letters[writeInd]);
			strcat(filename, ".txt");
			makeTester(letters[writeInd], filename);
			addWriter(sorter, filename);
			writeInd++;
		}
	}

	execute(sorter);
	destroyPriority(sorter);
	remove("main.txt");

	return 0;
}

/*
* reset the main.txt file
*/
void resetFiles(void)
{
	int i;

	FILE *fp = fopen("main.txt", "w+");

	for (i = 0; i < 1000; ++i)
		fputc('a', fp);

	fsync(fileno(fp));
	fclose(fp);
}

/*
* print the main.txt file for debugging
*/
void printFile(void)
{
	FILE *fp = fopen("main.txt", "r");
	char c = fgetc(fp);

	putchar(c);

	while (c != EOF) {
		putchar(c);
		c = fgetc(fp);
	}

	fclose(fp);
}

/*
* Generate a testing file
*/
void makeTester(char *c, char *filename)
{
	int i;
	int num = (*c - 'a' + 1) * 1000;
	FILE *fp = fopen(filename, "w");

	for (i = 0; i < num; i++)
		fputc(*c, fp);
	fclose(fp);
}
