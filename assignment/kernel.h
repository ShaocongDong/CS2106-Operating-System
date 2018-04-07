#ifndef __KERNEL_H__
#define __KERNEL_H__

/* Configuration Parameters */

// Choose scheduler type
// 0 = LINUX
// 1 = RMS

#define SCHEDULER_TYPE 1

#define NUM_PROCESSES 	10
#define NUM_RUNS		2

#define PRIO_LEVELS		140
#define QUANTUM_STEP	2
#define QUANTUM_MIN		20

void initOS();
#if SCHEDULER_TYPE == 0
int addProcess(int priority);
#elif SCHEDULER_TYPE == 1
int addProcess(int p, int c);
#endif

void startOS();
#endif
