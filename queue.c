/*
* Jordan McCaskill
* 1722645
* CMPT 360 Assignment 3
*/
#include "queue.h"

/*
* Creates a new node for the queue
*/
struct item *create_node(int pos, char *filename)
{
	struct item *new_node = malloc(sizeof(struct item));

	// Check the memory allocation for success
	if (new_node == NULL)
		exit(1);

	new_node->pos = pos;
	new_node->filename = filename;
	new_node->next = NULL;

	return new_node;
}

/*
* Appends an item to the end of the queue
*/
int append(struct head *myQueue, int num, char *filename)
{
	// If the list is empty set the first node to newNode
	if (myQueue->first == NULL)
		myQueue->first = create_node(num, filename);
	// If the list is not empty set the last node next value to the newNode
	else {
		struct item *pos = myQueue->first;

		for (; pos->next != NULL; pos = pos->next)
			;

		pos->next = create_node(num, filename);
	}

	return 0;
}

/*
* Checks the value of the first item in the queue
*/
int check(struct head *myQueue)
{
	if (myQueue->first == NULL)
		return -1;
	return myQueue->first->pos;
}

/*
* pop the first item from the queue
*/
void pop(struct head *myQueue)
{
	if (myQueue->first == NULL)
		return;
	struct item *temp = myQueue->first;

	myQueue->first = myQueue->first->next;

	free(temp->filename);
	free(temp);
}

/*
* Destroy the queue
*/
void destroy(struct head *myQueue)
{
	while (check(myQueue) != -1)
		pop(myQueue);
	free(myQueue);
}

/*
* print out the jobs for the queue, mainly for debugging
*/
void printQueue(struct head *myQueue)
{
	struct item *posi = myQueue->first;

	if (posi == NULL)
		return;

	for (; posi != NULL; posi = posi->next)
		printf("%d\n", posi->pos);
}
