#ifndef __LLIST__
#define __LLIST__

// This file implements a FIFO queue
typedef struct ll
{
	struct ll *next, *prev;
	int procNum;
	int quantum;
}TNode;

// Insert a new process into a list, with its process number and time quantum
void insert(TNode **head, int procNum, int quantum);

// Remove next item from list
int remove(TNode **head);

// Computes total quantum within a list
int totalQuantum(TNode *head);

// Destroy a list freeing all its elements
void destroy(TNode **head);
#endif
