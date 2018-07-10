/*
* Jordan McCaskill
* 1722645
* CMPT 360 Assignment 3
*/

// Head struct for queue
struct head {
	struct item *first;
};

// node struct for queue
struct item {
	struct item *next;
	int pos;
	char *filename;
};

// function prototypes
struct item *create_node(int pos, char *filename);
int append(struct head *myQueue, int pos, char *filename);
int check(struct head *myQueue);
void pop(struct head *myQueue);
void destroy(struct head *myQueue);
