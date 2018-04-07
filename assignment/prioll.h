#ifndef __PRIOLL_H__
#define __PRIOLL_H__

// Node that lets us sort by priority
typedef struct t
{
	struct t *prev, *next;
	int procNum;
	int prio;
	int p;

} TPrioNode;

// Insert a new process with its process number, period and priority
// into a queue. Note that generally you can set p and prio to
// be the same
void prioInsert(TPrioNode **head, int procNum, int p, int prio);

// Insert a node into the list
void prioInsertNode(TPrioNode **head, TPrioNode *node);

// Remove a node from any position in the list
// node must be in the list.
TPrioNode *prioRemoveNode(TPrioNode **head, TPrioNode *node);

// Remove the first item from the list
TPrioNode *prioRemove(TPrioNode **head);

// Check if there are any items in the list that are ready
// for execution
TPrioNode *checkReady(TPrioNode *head, int timerTick);

// Print the entire list
void printList(TPrioNode *head);

// Look at the first item in the list without removing it
TPrioNode *peek(TPrioNode *head);

// Find LCM of all periods in the list
int prioLCM(TPrioNode *head);

// Delete entire list
void prioDestroy(TPrioNode **head);

#endif
