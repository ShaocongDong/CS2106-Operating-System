#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "llist.h"
#include "prioll.h"
#include "kernel.h"

/*

	STUDENT 1 NUMBER:
	STUDENT 1 NAME:

	STUDENT 2 NUMBER:
	STUDENT 2 NAME:

	STUDENT 3 NUMBER:
	STUDENT 3 NAME:

	*/

/* OS variables */

// Current number of processes
int procCount;

// Current timer tick
int timerTick=0;
int currProcess, currPrio;

#if SCHEDULER_TYPE == 0

/* Process Control Block for LINUX scheduler*/
typedef struct tcb
{
	int procNum;
	int prio;
	int quantum;
	int timeLeft;
} TTCB;

#elif SCHEDULER_TYPE == 1

/* Process Control Block for RMS scheduler*/

typedef struct tcb
{
	int procNum;
	int timeLeft;
	int deadline;
	int c;
	int p;
} TTCB;

#endif


TTCB processes[NUM_PROCESSES];

#if SCHEDULER_TYPE == 0

// Lists of processes according to priority levels.
TNode *queueList1[PRIO_LEVELS];
TNode *queueList2[PRIO_LEVELS];

// Active list and expired list pointers
TNode **activeList = queueList1;
TNode **expiredList = queueList2;

#elif SCHEDULER_TYPE == 1

// Ready queue and blocked queue
TPrioNode *readyQueue, *blockedQueue;

// This stores the data for pre-empted processes.
TPrioNode *suspended; // Suspended process due to pre-emption

// Currently running process
TPrioNode *currProcessNode; // Current process
#endif


#if SCHEDULER_TYPE == 0

// Searches the active list for the next priority level with processes
int findNextPrio(int currPrio)
{
	int i;

	for(i=0; i<PRIO_LEVELS; i++)
		if(activeList[i] != NULL)
			return i;

	return -1; 

}
int linuxScheduler()
{
	/* TODO: IMPLEMENT LINUX STYLE SCHEDULER
		FUNCTION SHOULD RETURN PROCESS NUMBER OF THE APPROPRIATE RUNNING PROCESS
		FOR THE CURRENT TIMERTICK.

		YOU CAN ACCESS THE timerTick GLOBAL VARIABLE.

		YOU HAVE TWO LISTS OF PROCESSES: queueList1 AND queueList2, AND
		TWO POINTERS actliveList AND expiredList.

		THERE IS ALSO A PROCESS TABLE CALLED processes WHICH IS SET UP
		FOR YOU AND CONTAINS PROCESS INFORMATION. SEE THE TTCB STRUCTURE
		FOR DETAILS.

		THIS FUNCTION SHOULD UPDATE THE VARIOUS QUEUES AS IS NEEDED
		TO IMPLEMENT SCHEDULING */
	return 0;
}
#elif SCHEDULER_TYPE == 1

int RMSScheduler()
{
	/* TODO: IMPLEMENT RMS  STYLE SCHEDULER
		FUNCTION SHOULD RETURN PROCESS NUMBER OF THE APPROPRIATE  RUNNING PROCESS.
		FOR THE CURRENT TIMER TICK.

		YOU CAN ACCESS THE timerTick GLOBAL VARIABLE.

		YOU HAVE A VARIABLE CALLED readyQueue WHICH HOLDS A LIST OF PROCESSES
		READY TO RUN, AND blockedQueue WHICH HOLDS A LIST OF PROCESSES THAT
		ARE BLOCKED. THERE IS A THIRD VARIABLE currProcessNode WHICH SHOULD
		POINT TO THE CURRENTLY RUNNING PROCESS DEQUEUED FROM readyQueue,
		AND A VARIABLE CALLED suspended WHICH IS USED TO STORE THE INFORMATION
		OF A PRE-EMPTED PROCESS.

		THERE IS ALSO A PROCESS TABLE CALLED processes WHICH IS SET UP 
		FOR YOU AND CONTAINS PROCESS INFORMATION. SEE TTCB STRUCTURE
		FOR MORE INFORMATION.

		THIS FUNCTION SHOULD UPDATE THE VARIOUS QUEUES AS IS NEEDED
		TO IMPLEMENT SCHEDULING */
	return 0;
}

#endif

void timerISR()
{

#if SCHEDULER_TYPE == 0
	currProcess = linuxScheduler();
#elif SCHEDULER_TYPE == 1
	currProcess = RMSScheduler();
#endif
	
#if SCHEDULER_TYPE == 0
	static int prevProcess=-1;

	// To avoid repetitiveness for hundreds of cycles, we will only print when there's
	// a change of processes
	if(currProcess != prevProcess)
	{

		// Print process details for LINUX scheduler
		printf("Time: %d Process: %d Prio Level: %d Quantum : %d\n", timerTick, processes[currProcess].procNum+1,
			processes[currProcess].prio, processes[currProcess].quantum);
		prevProcess=currProcess;
	}
#elif SCHEDULER_TYPE == 1

	// Print process details for RMS scheduler

	printf("Time: %d ", timerTick);
	if(currProcess == -1)
		printf("---\n");
	else
	{
		// If we have busted a processe's deadline, print !! first
		int bustedDeadline = (timerTick >= processes[currProcess].deadline);

		if(bustedDeadline)
			printf("!! ");

		printf("P%d Deadline: %d", currProcess+1, processes[currProcess].deadline);

		if(bustedDeadline)
			printf(" !!\n");
		else
			printf("\n");
	}

#endif

	// Increment timerTick. You will use this for scheduling decisions.
	timerTick++;
}

void startTimer()
{
	// In an actual OS this would make hardware calls to set up a timer
	// ISR, start an actual physical timer, etc. Here we will simulate a timer
	// by calling timerISR every millisecond

	int i;

#if SCHEDULER_TYPE==0
	int total = processes[currProcess].quantum;

	for(i=0; i<PRIO_LEVELS; i++)
	{
		total += totalQuantum(activeList[i]);
	}

	for(i=0; i<NUM_RUNS * total; i++)
	{
		timerISR();
		usleep(1000);
	}
#elif SCHEDULER_TYPE==1

	// Find LCM of all periods
	int lcm = prioLCM(readyQueue);

	for(i=0; i<NUM_RUNS*lcm; i++)
	{
		timerISR();
		usleep(1000);
	}
#endif
}

void startOS()
{
#if SCHEDULER_TYPE == 0
	// There must be at least one process in the activeList
	currPrio = findNextPrio(0);

	if(currPrio < 0)
	{
		printf("ERROR: There are no processes to run!\n");
		return;
	}

	// set the first process
	currProcess = remove(&activeList[currPrio]);

#elif SCHEDULER_TYPE == 1
	currProcessNode = prioRemove(&readyQueue);
	currProcess = currProcessNode->procNum;
#endif

	// Start the timer
	startTimer();

#if SCHEDULER_TYPE == 0
	int i;

	for(i=0; i<PRIO_LEVELS; i++)
		destroy(&activeList[i]);

#elif SCHEDULER_TYPE == 1
	prioDestroy(&readyQueue);
	prioDestroy(&blockedQueue);

	if(suspended != NULL)
		free(suspended);
#endif
}

void initOS()
{
	// Initialize all variables
	procCount=0;
	timerTick=0;
	currProcess = 0;
	currPrio = 0;
#if SCHEDULER_TYPE == 0
	int i;

	// Set both queue lists to NULL
	for(i=0; i<PRIO_LEVELS; i++)
	{
		queueList1[i]=NULL;
		queueList2[i]=NULL;
	}
#elif SCHEDULER_TYPE == 1

	// Set readyQueue and blockedQueue to NULL
	readyQueue=NULL;
	blockedQueue=NULL;

	// The suspended variable is used to store
	// which process was pre-empted.
	suspended = NULL;
#endif

}

#if SCHEDULER_TYPE == 0

// Returns the quantum in ms for a particular priority level.
int findQuantum(int priority)
{
	return ((PRIO_LEVELS - 1) - priority) * QUANTUM_STEP + QUANTUM_MIN;
}

// Adds a process to the process table
int addProcess(int priority)
{
	if(procCount >= NUM_PROCESSES)
		return -1;

	// Insert process data into the process table
	processes[procCount].procNum = procCount;
	processes[procCount].prio = priority;
	processes[procCount].quantum = findQuantum(priority);
	processes[procCount].timeLeft = processes[procCount].quantum;

	// Add to the active list
	insert(&activeList[priority], processes[procCount].procNum, processes[procCount].quantum);
	procCount++;
	return 0;
}
#elif SCHEDULER_TYPE == 1

// Adds a process to the process table
int addProcess(int p, int c)
{
	if(procCount >= NUM_PROCESSES)
		return -1;

		// Insert process data into the process table
		processes[procCount].p = p;
		processes[procCount].c = c;
		processes[procCount].timeLeft=c;
		processes[procCount].deadline = p;

		// And add to the ready queue.
		prioInsert(&readyQueue, procCount, p, p);
	procCount++;
	return 0;
}


#endif


